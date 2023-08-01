#pragma once

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include "httplib.h"
#include "task_system.h"

#include <string>
#include <memory>

template <typename T, typename U>
T& ref(U* buf, std::size_t index)
{
    return *reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(buf) + index);
}

struct CharacterInfo
{
    std::string name;
    uint16_t    zoneId;

    // TODO: in-game time
    // TODO: character pos
};

class WeatherReporterCore
{
public:
    WeatherReporterCore()  = default;
    ~WeatherReporterCore() = default;

    WeatherReporterCore(const WeatherReporterCore& other)       = delete;
    WeatherReporterCore(WeatherReporterCore&& other)            = delete;
    WeatherReporterCore& operator=(WeatherReporterCore&& other) = delete;

    void SendPutRequest(std::string path, std::string payload);
    bool DetectRetail();

    void HandlePacketData(CharacterInfo const& info, uint8_t* data, uint32_t dataSz);

private:
    ts::task_system m_TaskSystem;

    std::unique_ptr<httplib::Client> m_CLI;

    std::mutex m_CLIMutex;

    // TODO: Make configurable
    std::string m_URL = "http://35.209.198.215";
};
