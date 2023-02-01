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

#include "../weather_reporter_core.h"

namespace
{
    int Print(lua_State* L, const char* str)
    {
        lua_getglobal(L, "print");
        if (lua_isfunction(L, -1))
        {
            lua_pushstring(L, str);
            lua_pcall(L, 1, 0, 0);
        }

        return 0;
    }

    int SendPutRequest(lua_State* L)
    {
        static WeatherReporterCore wrCore;

        // TODO: FIXME
        lua_gettable(L, 1);
        lua_getfield(L, -1, "zone");
        int zone = lua_tonumber(L, -1);
        lua_pop(L, 1);
        lua_pop(L, 1);

        auto data = lua_tostring(L, 2);

        int weather = 0;

        int epoch = 0;

        std::string payload = std::to_string(zone) + "," + std::to_string(weather) + "," + std::to_string(epoch);

        wrCore.SendPutRequest("http://35.209.198.215", "/weather", payload, [&L](std::string errString) {
            Print(L, errString.c_str());
        });

        return 0;
    }
} // namespace

extern "C" __declspec(dllexport) int luaopen__WeatherReporter(lua_State* L)
{
    struct luaL_reg api[] = {
        {"submit", ::SendPutRequest},
        {NULL, NULL}};

    luaL_register(L, "_WeatherReporter", api);

    return 1;
}
