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

#include "ashita_plugin.h"

#pragma comment(lib, "psapi.lib")
#include <psapi.h>

WeatherReporter::WeatherReporter(void)
    : m_AshitaCore{nullptr}
    , m_LogManager{nullptr}
    , m_PluginId(0)
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
    return 1.1f;
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

    this->wrCore = std::make_unique<WeatherReporterCore>();

    return true;
}

void WeatherReporter::Release(void)
{
    this->wrCore = nullptr;
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
        this->wrCore->SendPutRequest(URL, "/weather", payload);
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
        this->wrCore->SendPutRequest(URL, "/weather", payload);
    }

    return false;
}

extern "C" __declspec(dllexport) IPlugin* __stdcall expCreatePlugin(const char* args)
{
    UNREFERENCED_PARAMETER(args);
    return new WeatherReporter();
}

extern "C" __declspec(dllexport) double __stdcall expGetInterfaceVersion(void)
{
    return ASHITA_INTERFACE_VERSION;
}
