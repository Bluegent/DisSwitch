#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>


using Paths = std::vector<DISPLAYCONFIG_PATH_INFO>;
using Modes = std::vector<DISPLAYCONFIG_MODE_INFO>;

void printMode(const DISPLAYCONFIG_MODE_INFO& mode, const std::string& prefix);
void printPathInfo(const DISPLAYCONFIG_PATH_INFO& path, const Modes& modes);

struct Counts
{
    uint32_t paths;
    uint32_t modes;
    Counts(uint32_t paths, uint32_t modes)
        : paths{ paths }
        , modes{ modes }
    {

    }
};

struct DisplayInfo
{
    Paths paths;
    Modes modes;
    DisplayInfo(const Counts & counts)
        : paths(counts.paths)
        , modes(counts.modes)
    {
        Counts local = counts;
        if (QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &local.paths, paths.data(), &local.modes, modes.data(), NULL) != ERROR_SUCCESS)
        {
            std::cerr << "Error querying display configuration\n";
        }
    }

    void reload()
    {
        paths.clear();
        modes.clear();
        Counts local = DisplayInfo::getCounts();
        paths.resize(local.paths);
        modes.resize(local.modes);
        if (QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &local.paths, paths.data(), &local.modes, modes.data(), NULL) != ERROR_SUCCESS)
        {
            std::cerr << "Error querying display configuration\n";
        }
    }
    void printPaths() const
    {
        for (size_t i = 0; i < paths.size(); ++i)
        {
            std::cout << "Path" << i << ":\n";
            printPathInfo(paths[i], modes);
        }
    }
    static Counts getCounts()
    {
        uint32_t pathCount, modeCount;
        if (GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &pathCount, &modeCount) != ERROR_SUCCESS)
        {
            std::cerr << "Error getting display configuration buffer sizes\n";
        }
        return Counts(pathCount,modeCount);
    }
};


class CDSWrapper
{
private:
    DisplayInfo info;
public:
    CDSWrapper()
        : info(DisplayInfo::getCounts())
    {
    }

    void print()
    {
        info.printPaths();
    }

    void reload()
    {
        info.reload();
    }
    const DisplayInfo& getInfo() const
    {
        return info;
    }
    int32_t clone(uint32_t target, uint32_t source);
    int32_t disconnect(uint32_t target);

};

