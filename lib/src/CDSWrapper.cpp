#include "CDSWrapper.hpp"
#include <iostream>
#include <iomanip>  
#include <assert.h>


LONG setSettings(const std::string& name, DEVMODEA* devMode)
{
    LONG result = ChangeDisplaySettingsExA(name.c_str(), devMode, NULL, (CDS_UPDATEREGISTRY | CDS_NORESET), NULL);
    if (result != DISP_CHANGE_SUCCESSFUL)
    {
        std::cerr << "Error changing settings of " << name << " : " << result << '\n';
        return result;
    }
    std::cout << "Successfully set settings for " << name << '\n';
    ChangeDisplaySettingsEx(NULL, NULL, NULL, NULL, NULL);
    return result;
}


CDSWrapper::CDSWrapper()
    : monitors{}
{
    int deviceIndex = 0;
    DISPLAY_DEVICEA displayDevice;
    displayDevice.cb = sizeof(DISPLAY_DEVICEA);
    while (EnumDisplayDevicesA(NULL, deviceIndex, &displayDevice, 0))
    {
        monitors.emplace_back(Monitor(displayDevice.DeviceName, displayDevice.StateFlags));
        deviceIndex++;
    }
}

uint32_t CDSWrapper::extendDisplay(int32_t targetIndex, int32_t sourceIndex, int32_t posX, int32_t posY)
{
    DEVMODEA targetMode = getDevMode(getMonitor(sourceIndex).name);

    targetMode.dmSize = sizeof(DEVMODEA);
    targetMode.dmPosition.x = posX;
    targetMode.dmPosition.y = posY;
    targetMode.dmFields = DM_POSITION | DM_PELSHEIGHT | DM_PELSWIDTH | DM_DISPLAYFLAGS;
    return setSettings(getMonitor(targetIndex).name, &targetMode);
}

uint32_t CDSWrapper::getDisplayCount() const
{
    return monitors.size();
}

DEVMODEA CDSWrapper::getDevMode(const std::string& displayName)
{
    DEVMODEA devMode = {0};
    devMode.dmSize = sizeof(DEVMODEA);

    // Get the current display settings of the source monitor
    if (!EnumDisplaySettingsA(displayName.c_str(), ENUM_CURRENT_SETTINGS, &devMode))
    {
        std::stringstream ss;      
        ss << "Could not get devmode for monitor \"" << std::string(displayName) << "\"";
        throw std::exception(ss.str().c_str());
    }
    return devMode;
}

void CDSWrapper::printDisplayInfo() const
{
    for (const auto& monitor : monitors)
    {
        try
        {
            printMonitor(monitor);
        }
        catch (std::exception e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void CDSWrapper::setRefreshRate(int32_t targetIndex, int32_t rate)
{
    const Monitor& target = getMonitor(targetIndex);

    DEVMODEA devmode = getDevMode(target.name);
    devmode.dmDisplayFrequency = rate;
    setSettings(target.name, &devmode);
     
}

int32_t CDSWrapper::getRefreshRate(int32_t targetIndex)
{
    const Monitor& target = getMonitor(targetIndex);
    DEVMODEA devmode = getDevMode(target.name);
    return devmode.dmDisplayFrequency;
}

const Monitor& CDSWrapper::getMonitor(int32_t index) const
{
    if (index >= static_cast<int32_t>(monitors.size()) || index < 0)
    {
        std::stringstream ss;
        ss << "Index " << index << " exceeds monitor count " << monitors.size();
        std::cerr << ss.str() << '\n';
        throw std::invalid_argument(ss.str().c_str());
    }
    return monitors[index];
}

void CDSWrapper::printDevMode(const DEVMODEA& devMode) const
{
    std::cout << " pos: {" << std::setw(5) << devMode.dmPosition.x << ":" << std::setw(5) << devMode.dmPosition.y << "}";
    std::cout << " resolution: " << devMode.dmPelsWidth << " x " << devMode.dmPelsHeight;
    std::cout << " refresh rate: " << std::setw(3) << devMode.dmDisplayFrequency;
}

void CDSWrapper::printMonitor(const Monitor & monitor) const
{
    printMonitor(monitor, getDevMode(monitor.name));
}

void CDSWrapper::printMonitor(const Monitor& monitor, const DEVMODEA& devMode) const
{
    std::cout << monitor.name;
    printDevMode(devMode);
    std::cout << " state: " << monitor.state;
    std::cout << '\n';
}
