#include "weather_reporter_core.h"

void WeatherReporterCore::SendPutRequest(std::string base, std::string path, std::string payload)
{
    if (!DetectRetail())
    {
        return;
    }

    m_TaskSystem.schedule([this, base, path, payload]() {
        try
        {
            httplib::Client cli(base);
            std::ignore = cli.Put(path, payload, "text");
        }
        catch (std::exception e)
        {
            // TODO: Error logging
        }
    });
}

bool WeatherReporterCore::DetectRetail()
{
    return GetModuleHandleA("polhook.dll") != NULL;
}
