#include "SDCDisplayInfo.hpp"
#include "PrintHelper.hpp"

void printMode(const DISPLAYCONFIG_MODE_INFO& mode, const std::string& prefix)
{
    printItem(prefix + ".mode.id", mode.id);
    printItem(prefix + ".mode.src.h", mode.sourceMode.height);
}

void printPathInfo(const DISPLAYCONFIG_PATH_INFO& path, const Modes& modes)
{
    printItem("tar.id", path.targetInfo.id);
    printItem("tar.mode", path.targetInfo.modeInfoIdx);
    printItem("tar.ref", path.targetInfo.refreshRate.Numerator / path.targetInfo.refreshRate.Denominator);
    printMode(modes[path.targetInfo.modeInfoIdx], "tar");
    printItem("src.mode", path.sourceInfo.modeInfoIdx);
    printMode(modes[path.sourceInfo.modeInfoIdx], "src");
}

SDCDisplayInfo::SDCDisplayInfo(const Counts& counts)
    : paths(counts.paths)
    , modes(counts.modes)
{
    Counts local = counts;
    if (QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &local.paths, paths.data(), &local.modes, modes.data(), NULL) != ERROR_SUCCESS)
    {
        std::cerr << "Error querying display configuration\n";
    }
}

void SDCDisplayInfo::reload()
{
    paths.clear();
    modes.clear();
    Counts local = SDCDisplayInfo::getCounts();
    paths.resize(local.paths);
    modes.resize(local.modes);
    if (QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &local.paths, paths.data(), &local.modes, modes.data(), NULL) != ERROR_SUCCESS)
    {
        std::cerr << "Error querying display configuration\n";
    }
}

void SDCDisplayInfo::printPaths() const
{
    for (size_t i = 0; i < paths.size(); ++i)
    {
        std::cout << "Path" << i << ":\n";
        printPathInfo(paths[i], modes);
    }
}

Counts SDCDisplayInfo::getCounts()
{
    uint32_t pathCount = 0, modeCount = 0;
    if (GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &pathCount, &modeCount) != ERROR_SUCCESS)
    {
        std::cerr << "Error getting display configuration buffer sizes\n";
    }
    return Counts(pathCount, modeCount);
}
