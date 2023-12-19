#include "SDCWrapper.hpp"
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

int32_t SDCWrapper::clone(uint32_t target, uint32_t source)
{
    info.paths[3].flags |= DISPLAYCONFIG_PATH_ACTIVE;
    info.paths[3].sourceInfo.modeInfoIdx = info.paths[1].sourceInfo.modeInfoIdx;   //same source
    info.paths[3].sourceInfo.id = info.paths[1].sourceInfo.id;       //same source

    LONG result = SetDisplayConfig(info.paths.size(), info.paths.data(), info.modes.size(), info.modes.data(), (SDC_APPLY | SDC_USE_SUPPLIED_DISPLAY_CONFIG | SDC_ALLOW_CHANGES | SDC_SAVE_TO_DATABASE));

    if (result != ERROR_SUCCESS)
    {
        std::cerr << "Error setting display configuration. " << result << std::endl;
        return result;
    }

    std::cout << "Display configuration updated successfully." << std::endl;
    return 0;
}

int32_t SDCWrapper::disconnect(uint32_t target)
{
    info.paths[target].flags = 0;

    LONG result = SetDisplayConfig(info.paths.size(), info.paths.data(), info.modes.size(), info.modes.data(), (SDC_APPLY | SDC_USE_SUPPLIED_DISPLAY_CONFIG | SDC_ALLOW_CHANGES | SDC_SAVE_TO_DATABASE));

    if (result != ERROR_SUCCESS)
    {
        std::cerr << "Error setting display configuration. " << result << std::endl;
        return result;
    }

    std::cout << "Display configuration updated successfully." << std::endl;
    return 0;
}
