
#include "WinapiHelper.hpp"

const int32_t CorrectDisplayCount = 4;

void WinapiHelper::connectDisplay3()
{
    cdsw.printDisplayInfo();
    if (display3Connected())
    {
        std::cout << "Display 3 already connected:\n";
        cdsw.printMonitor(cdsw.getMonitor(3));
    }
    else
    {
        cdsw.extendDisplay(3, 1, -3840);
    }
}


void WinapiHelper::list()
{
    cdsw.printDisplayInfo();
    sdcw.print();
}

void WinapiHelper::clone()
{
    if (isInCloneMode())
    {
        std::cout << " Already in clone mode.\n";
        return;
    }
    if (!display3Connected())
    {
        connectDisplay3();
    }
    sdcw.reload();
    std::cout << "Cloning display 1 onto display 3 and setting refresh rate of both to 60...\n";
    sdcw.clone(3, 1);
    if (cdsw.getRefreshRate(1) != 60)
    {
        cdsw.setRefreshRate(1, 60);
    }
    cdsw.printDisplayInfo();
    sdcw.reload();
}

void WinapiHelper::disconnect()
{
    int32_t disconnectTarget = 3;
    std::cout << "Disconnecting display 3...\n";
    if (isInCloneMode())
    {
        disconnectTarget = getDisplay3Index();
        if (disconnectTarget == -1)
        {
            std::cerr << "Display 3 seems to not be connected or not in clone mode.\n";
            return;
        }
    }
    else if (!display3Connected())
    {
        std::cout << "Not in clone mode and display 3 is not connected.\n";
        return;
    }
    sdcw.disconnect(disconnectTarget);
    sdcw.reload();
    cdsw.printDisplayInfo();
}

void WinapiHelper::extend()
{
    if (isInCloneMode())
    {
        std::cout << "In clone mode, disconnecting display 3...\n";
        int32_t display3Index = getDisplay3Index();
        if (display3Index == -1)
        {
            std::cerr << "Display 3 seems to not be connected or not in clone mode.\n";
            return;
        }
        sdcw.disconnect(display3Index);
    }
    connectDisplay3();
    sdcw.reload();
    cdsw.printDisplayInfo();
}

bool WinapiHelper::isCorrectDisplayCount()
{
    return cdsw.getDisplayCount() == CorrectDisplayCount;
}

bool WinapiHelper::isInCloneMode()
{
    const auto& info = sdcw.getInfo();
    //if only 3 displays are connected, you have 6 modes
    if (info.modes.size() == 6)
    {
        return false;
    }
    //if only 7 modes are available, one display is a clone <- this seems like a strange exception I've only seen sometimes
    if (info.modes.size() == 7)
    {
        return true;
    }

    // otherwise, mode 7 will have its type set to 0 instead of TARGET/SOURCE
    return info.modes[7].infoType == 0 ? true : false;
}

bool WinapiHelper::display3ConnectedSDC()
{
    const auto& info = sdcw.getInfo();
    // if there are 6 modes, that means either 3 monitors in extended mode 
    // or unlikely cases like 2 monitors extended + 1 cloned which we don't care about
    return info.modes.size() > 6;
}

bool WinapiHelper::display3ConnectedCDS()
{
    try
    {
        const Monitor& display3 = cdsw.getMonitor(3);
        DEVMODEA d3Mode = cdsw.getDevMode(display3.name);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool WinapiHelper::display3Connected(bool useCDS)
{
    // meh...
    return useCDS ?
        display3ConnectedCDS() :
        display3ConnectedSDC();
}

int32_t WinapiHelper::getDisplay3Index()
{
    // if we are in clone mode we have to look for the correct Id
    // idk how it works but usually the id's I've seen are 576** when a display is not cloned
    const auto& info = sdcw.getInfo();
    for (size_t i = 0; i < info.paths.size(); ++i)
    {
        if (info.paths[i].targetInfo.id >= 60000)
        {
            return static_cast<int32_t>(i);
        }
    }
    return -1;
}
