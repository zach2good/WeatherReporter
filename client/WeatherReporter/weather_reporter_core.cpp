#include "weather_reporter_core.h"

void WeatherReporterCore::SendPutRequest(std::string base, std::string path, std::string payload, std::function<void(std::string)> errorCallback)
{
    if (!DetectRetail())
    {
        return;
    }

    m_TaskSystem.schedule([this, base, path, payload, errorCallback]() {
        try
        {
            httplib::Client cli(base);
            std::ignore = cli.Put(path, payload, "text");
        }
        catch (std::exception e)
        {
            errorCallback(e.what());
        }
    });
}

// TODO: Make this more robust
bool WeatherReporterCore::DetectRetail()
{
    return GetModuleHandleA("polhook.dll") != NULL;
}
