#include "weather_reporter_core.h"

void WeatherReporterCore::SendPutRequest(std::string path, std::string payload)
{
    if (!DetectRetail())
    {
        return;
    }

    // clang-format off
    m_TaskSystem.schedule([this, path, payload]()
    {
        // If multiple tasks are created they might all try to create/use the
        // http client, so wrap each task in a lock_guard and mutex.
        std::lock_guard<std::mutex> lg(m_CLIMutex);
        try
        {
            // Lazy load the http client
            if (m_CLI == nullptr)
            {
                m_CLI = std::make_unique<httplib::Client>(m_URL);
            }

            // PUT
            std::ignore = m_CLI->Put(path, payload, "text");
        }
        catch (std::exception e)
        {
            // TODO: Error logging
        }
    });
    // clang-format on
}

bool WeatherReporterCore::DetectRetail()
{
    return GetModuleHandleA("polhook.dll") != NULL;
}

void WeatherReporterCore::HandlePacketData(CharacterInfo const& info, uint8_t* data, uint32_t dataSz)
{
    auto id = ref<uint16_t>(data, 0) & 0x1FF;

    // Zone In
    if (id == 0x00A)
    {
        unsigned short zone    = ref<unsigned short>(data, 0x30);
        unsigned char weather  = data[0x68];
        unsigned long long utc = static_cast<unsigned long>(std::time(0));
        std::string payload    = std::to_string(zone) + "," + std::to_string(weather) + "," + std::to_string(utc);
        SendPutRequest("/weather", payload);
    }
    // Weather Change
    else if (id == 0x057)
    {
        if (!info.zoneId)
        {
            return;
        }
        unsigned char weather  = data[0x08];
        unsigned long long utc = static_cast<unsigned long>(std::time(0));
        std::string payload    = std::to_string(info.zoneId) + "," + std::to_string(weather) + "," + std::to_string(utc);
        SendPutRequest("/weather", payload);
    }
}
