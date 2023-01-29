/**
 * WeatherReporter by zach2good, based upon:
 *
 * Ashita Example Plugin - Copyright (c) Ashita Development Team
 * Contact: https://www.ashitaxi.com/
 * Contact: https://discord.gg/Ashita
 *
 * This file is part of Ashita Example Plugin.
 *
 * Ashita Example Plugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ashita Example Plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Ashita Example Plugin.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "WeatherReporter.hpp"

#pragma comment(lib, "Psapi.lib")
#include <Psapi.h>

#include "task_system.hpp"

WeatherReporter::WeatherReporter(void)
    : m_AshitaCore{nullptr}
    , m_LogManager{nullptr}
    , m_PluginId(0)
    , m_WorldId(0)
{
}

WeatherReporter::~WeatherReporter(void)
{
}

const char* WeatherReporter::GetName(void) const
{
    return "WeatherReporter";
}

const char* WeatherReporter::GetAuthor(void) const
{
    return "zach2good";
}

const char* WeatherReporter::GetDescription(void) const
{
    return "";
}

const char* WeatherReporter::GetLink(void) const
{
    return "";
}

double WeatherReporter::GetVersion(void) const
{
    return 1.0f;
}

double WeatherReporter::GetInterfaceVersion(void) const
{
    return ASHITA_INTERFACE_VERSION;
}

int32_t WeatherReporter::GetPriority(void) const
{
    return 0;
}

uint32_t WeatherReporter::GetFlags(void) const
{
    return (uint32_t)Ashita::PluginFlags::UsePackets;
}

bool WeatherReporter::Initialize(IAshitaCore* core, ILogManager* logger, const uint32_t id)
{
    this->m_AshitaCore = core;
    this->m_LogManager = logger;
    this->m_PluginId   = id;

    // Thanks atom0s!:
    // Ashita::Memory::FindPattern(...)
    // A1 ?? ?? ?? ?? 66 8B 4C 24 04 66 89 88 ?? ?? ?? ?? E8
    // +0x01 is pointer to the pGcMainSys object which holds the current set world id.
    // +0x0D is the offset into pGcMainSys where its stored in the event it ever shifts from an update. (Unlikely at this point.)

    // Find FFXiMain.dll module for base and size info..
    MODULEINFO mod{};
    if (!::GetModuleInformation(::GetCurrentProcess(), ::GetModuleHandleA("FFXiMain.dll"), &mod, sizeof(MODULEINFO)))
    {
        // Failed to find FFXiMain.dll..
        return false;
    }

    // Find the pGcMainSys pointer..
    const auto base = Ashita::Memory::FindPattern((uintptr_t)mod.lpBaseOfDll, mod.SizeOfImage, "A1????????668B4C2404668988????????E8", 0x01, 0);
    if (base == 0)
    {
        // Failed to find base pointer..
        return false;
    }

    // Read the pointer..
    const auto ptr = *(uint32_t*)base;
    if (ptr == 0)
    {
        // Failed to read base pointer..
        return false;
    }

    // Read the world id..
    m_WorldId = *(uint32_t*)(ptr + 0x130);

    this->m_TaskSystem = std::make_unique<ts::task_system>(1);

    return true;
}

void WeatherReporter::Release(void)
{
    // Tiny sleep, just to make sure the Task System is clear
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
}

bool WeatherReporter::HandleIncomingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked)
{
    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(data);
    UNREFERENCED_PARAMETER(sizeChunk);
    UNREFERENCED_PARAMETER(dataChunk);
    UNREFERENCED_PARAMETER(injected);
    UNREFERENCED_PARAMETER(blocked);

    std::string URL = "http://35.209.198.215";

    // Zone In
    if (id == 0x00A)
    {
        unsigned short zone    = ref<unsigned short>(modified, 0x30);
        unsigned char weather  = modified[0x68];
        unsigned long long utc = static_cast<unsigned long>(std::time(0));
        std::string payload    = std::to_string(zone) + "," + std::to_string(weather) + "," + std::to_string(utc);
        SendPutRequest(URL, "/weather", payload);
    }
    // Weather Change
    else if (id == 0x057)
    {
        unsigned short zone = m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberZone(0);
        if (!zone)
        {
            return false;
        }
        unsigned char weather  = modified[0x08];
        unsigned long long utc = static_cast<unsigned long>(std::time(0));
        std::string payload    = std::to_string(zone) + "," + std::to_string(weather) + "," + std::to_string(utc);
        SendPutRequest(URL, "/weather", payload);
    }

    return false;
}

void WeatherReporter::SendPutRequest(std::string base, std::string path, std::string payload)
{
    if (!DetectRetail())
    {
        return;
    }

    m_LogManager->Log(4, "WeatherReporter", std::string(base + path + " :: " + payload).c_str());
    this->m_TaskSystem->schedule([this, base, path, payload]() {
        try
        {
            httplib::Client cli(base);
            std::ignore = cli.Put(path, payload, "text");
        }
        catch (std::exception e)
        {
            m_LogManager->Log(2, "WeatherReporter", e.what());
        }
    });
}

bool WeatherReporter::DetectRetail()
{
    // https://github.com/atom0s/XiPackets/blob/main/lobby/Notes.md#worldid
    // 0x62 - Undine
    // 0x64 - Bahamut
    // 0x65 - Shiva
    // 0x68 - Phoenix
    // 0x69 - Carbuncle
    // 0x6A - Fenrir
    // 0x6B - Sylph
    // 0x6C - Valefor
    // 0x6E - Leviathan
    // 0x6F - Odin
    // 0x73 - Quetzalcoatl
    // 0x74 - Siren
    // 0x77 - Ragnarok
    // 0x7A - Cerberus
    // 0x7C - Bismarck
    // 0x7E - Lakshmi
    // 0x7F - Asura

    // TODO: Test and hook this up
    auto notRetailId = m_WorldId >= 0x62 && m_WorldId <= 0x7F;
    std::ignore      = notRetailId;

    return GetModuleHandleA("polhook.dll") != NULL; //&& notRetailId;
}

__declspec(dllexport) IPlugin* __stdcall expCreatePlugin(const char* args)
{
    UNREFERENCED_PARAMETER(args);
    return new WeatherReporter();
}

__declspec(dllexport) double __stdcall expGetInterfaceVersion(void)
{
    return ASHITA_INTERFACE_VERSION;
}
