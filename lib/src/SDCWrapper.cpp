#include "SDCWrapper.hpp"
#include "PrintHelper.hpp"



SDCWrapper::SDCWrapper()
    : info(SDCDisplayInfo::getCounts())
{
}

void SDCWrapper::print()
{
    info.printPaths();
}

void SDCWrapper::reload()
{
    info.reload();
}

const SDCDisplayInfo& SDCWrapper::getInfo() const
{
    return info;
}

int32_t SDCWrapper::clone(uint32_t target, uint32_t source)
{
    info.paths[3].flags |= DISPLAYCONFIG_PATH_ACTIVE;
    info.paths[3].sourceInfo.modeInfoIdx = info.paths[1].sourceInfo.modeInfoIdx;   //same source
    info.paths[3].sourceInfo.id = info.paths[1].sourceInfo.id;       //same source

    return applySettings((SDC_APPLY | SDC_USE_SUPPLIED_DISPLAY_CONFIG | SDC_ALLOW_CHANGES | SDC_SAVE_TO_DATABASE));
}

int32_t SDCWrapper::disconnect(uint32_t target)
{
    info.paths[target].flags = 0;
    return applySettings((SDC_APPLY | SDC_USE_SUPPLIED_DISPLAY_CONFIG | SDC_ALLOW_CHANGES | SDC_SAVE_TO_DATABASE));
}

int32_t SDCWrapper::applySettings(uint32_t flags)
{
    LONG result = SetDisplayConfig(static_cast<UINT32>(info.paths.size()), 
        info.paths.data(), static_cast<UINT32>(info.modes.size()), 
        info.modes.data(), 
        flags);
    if (result != ERROR_SUCCESS)
    {
        std::cerr << "Error setting display configuration. " << result << std::endl;
        return result;
    }

    std::cout << "Display configuration updated successfully." << std::endl;
    return result;
}