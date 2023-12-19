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

struct SDCDisplayInfo
{
    Paths paths;
    Modes modes;
    /// <summary>
    /// Initializes path info and mode info structures
    /// </summary>
    /// <param name="counts">path and mode info counts, see getCounts()</param>
    SDCDisplayInfo(const Counts& counts);
    /// <summary>
    /// Reloads path info and mode info structures
    /// </summary>
    void reload();
    /// <summary>
    /// Prints path info structures
    /// </summary>
    void printPaths() const;
    /// <summary>
    ///
    /// </summary>
    /// <returns> Returns the count of path info and mode info entries</returns>
    static Counts getCounts();
};