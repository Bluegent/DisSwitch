#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>

class Monitor
{
public:
    std::string name;
    uint32_t state;
    Monitor(const std::string& name, uint32_t state)
        : name{ name }
        , state{ state }
    {

    }
    std::string toString() const
    {
        std::stringstream stream;
        stream << "Name: " << name << " State: " << state;
        return stream.str();
    }
};
using Monitors = std::vector<Monitor>;

class CDSWrapper
{
private:
    Monitors monitors;
public:
    /// <summary>
    /// Retrieves a list of DEVMODE structures for all plugged in displays
    /// </summary>
    /// 
    CDSWrapper();
    /// <summary>
    /// extends desktop to the disconnected display at targetIndex
    /// </summary>
    /// <param name="targetIndex"> index of display to connect  </param>
    /// <param name="sourceIndex"> index of display for which settings are duplicated </param>
    /// <returns></returns>
    uint32_t extendDisplay(int32_t targetIndex, int32_t sourceIndex, int32_t posX = 0, int32_t posY = 0);

    /// <summary>
    /// 
    /// </summary>
    /// <returns> count of plugged in displays </returns>
    uint32_t getDisplayCount() const;

    /// <summary>
    /// retrieves display info
    /// </summary>
    /// <param name="displayName"> name of the display as returned by EnumDisplayDevices</param>
    /// <returns> DEVMODE structure associated with the display</returns>
    static DEVMODEA getDevMode(const std::string& displayName);

    /// <summary>
    /// prints information about all plugged in display to stdout
    /// </summary>
    void printDisplayInfo() const;


    void setRefreshRate(int32_t targetIndex, int32_t rate);

    int32_t getRefreshRate(int32_t targetIndex);

    const Monitor& getMonitor(int32_t index) const;

    void printMonitor(const Monitor& monitor) const;
    void printMonitor(const Monitor& monitor, const DEVMODEA& mode) const;
    void printDevMode(const DEVMODEA& mode) const;
};

