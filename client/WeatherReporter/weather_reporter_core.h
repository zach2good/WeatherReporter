#pragma once

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include "httplib.h"
#include "task_system.h"

#include <string>

class WeatherReporterCore
{
public:
    WeatherReporterCore()  = default;
    ~WeatherReporterCore() = default;

    WeatherReporterCore(const WeatherReporterCore& other)       = delete;
    WeatherReporterCore(WeatherReporterCore&& other)            = delete;
    WeatherReporterCore& operator=(WeatherReporterCore&& other) = delete;

    void SendPutRequest(
        std::string base, std::string path, std::string payload, std::function<void(std::string)> errorCallback);
    bool DetectRetail();

private:
    ts::task_system m_TaskSystem;
};