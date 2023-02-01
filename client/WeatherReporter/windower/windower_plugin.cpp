/*
WeatherReporter by zach2good, based upon:

XIPivot
Copyright © 2019-2022, Renee Koecher
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met :

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
* Neither the name of XIPivot nor the
  names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

extern "C"
{
#define LUA_BUILD_AS_DLL

#include "lauxlib.h"
#include "lua.h"
}

#include <sol/sol.hpp>

#include "../weather_reporter_core.h"

namespace
{
    std::unique_ptr<sol::state_view> lua;

    void Print(std::string str)
    {
        (*lua)["print"](str);
    }

    void SendPutRequest(sol::table table, std::string data)
    {
        static WeatherReporterCore wrCore;

        auto id = ref<uint16_t>(data.data(), 0) & 0x1FF;

        std::string URL = "http://35.209.198.215";

        // Zone In
        if (id == 0x00A)
        {
            unsigned short zone    = ref<unsigned short>(data.data(), 0x30);
            unsigned char weather  = data.data()[0x68];
            unsigned long long utc = static_cast<unsigned long>(std::time(0));
            std::string payload    = std::to_string(zone) + "," + std::to_string(weather) + "," + std::to_string(utc);
            wrCore.SendPutRequest(URL, "/weather", payload);
        }
        // Weather Change
        else if (id == 0x057)
        {
            unsigned short zone = table["zone"];
            if (!zone)
            {
                return;
            }
            unsigned char weather  = data.data()[0x08];
            unsigned long long utc = static_cast<unsigned long>(std::time(0));
            std::string payload    = std::to_string(zone) + "," + std::to_string(weather) + "," + std::to_string(utc);
            wrCore.SendPutRequest(URL, "/weather", payload);
        }
    }
} // namespace

extern "C" __declspec(dllexport) int luaopen__WeatherReporter(lua_State* L)
{
    lua = std::make_unique<sol::state_view>(L);

    (*lua).create_table("_WeatherReporter");
    (*lua)["_WeatherReporter"]["print"]  = &::Print;
    (*lua)["_WeatherReporter"]["submit"] = &::SendPutRequest;

    return 1;
}
