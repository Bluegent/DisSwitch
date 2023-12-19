
#include "WinapiHelper.hpp"

const int32_t CorrectDisplayCount = 4;

bool display3Connected(CDSWrapper& cdsw)
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

bool isinCloneMode(const SDCWrapper& CDSWrapper)
{
    const auto& info = CDSWrapper.getInfo();
    if (info.modes.size() == 6)
    {
        return false;
    }
    if (info.modes.size() == 7)
    {
        return true;
    }
    return info.modes[7].infoType == 0 ? true : false;
}

void connectDisplay3(CDSWrapper& cdsw)
{
    cdsw.printDisplayInfo();
    if (display3Connected(cdsw))
    {
        std::cout << "Display 3 already connected:\n";
        cdsw.printMonitor(cdsw.getMonitor(3));
    }
    else
    {
        cdsw.extendDisplay(3, 1, -3840);
    }
}

void cloneDisplay3(SDCWrapper& sdcw, CDSWrapper& cdsw)
{
    if (isinCloneMode(sdcw))
    {
        std::cout << " Already in clone mode.\n";
        return;
    }
    if (!display3Connected(cdsw))
    {
        connectDisplay3(cdsw);
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

int32_t getDisconnectTarget(SDCWrapper& CDSWrapper)
{
    //if we are in clone mode we have to look for the correct Id
    // idk how it works but usually the id's I've seen are 576** when a display is not cloned
    const auto& info = CDSWrapper.getInfo();
    for (size_t i = 0; i < info.paths.size(); ++i)
    {
        if (info.paths[i].targetInfo.id >= 60000)
        {
            return static_cast<int32_t>(i);
        }
    }
    return -1;
}

void disconnectDisplay3(SDCWrapper& sdcw, CDSWrapper& cdsw)
{
    int32_t disconnectTarget = 3;
    std::cout << "Disconnecting display 3...\n";
    if (isinCloneMode(sdcw))
    {      
        disconnectTarget = getDisconnectTarget(sdcw);        
    }
    else if (!display3Connected(cdsw))
    {
        std::cout << "Not in clone mode and display 3 is not connected.\n";
        return;
    }
    sdcw.disconnect(disconnectTarget);
    sdcw.reload();
    cdsw.printDisplayInfo();
}

void extendDisplay3(SDCWrapper& sdcw, CDSWrapper& cdsw)
{
    if (isinCloneMode(sdcw))
    {
        std::cout << "In clone mode, disconnecting display 3...\n";
        sdcw.disconnect(getDisconnectTarget(sdcw));
    }
    connectDisplay3(cdsw);
    sdcw.reload();
    cdsw.printDisplayInfo();
}


void WinapiHelper::list()
{
    cdsw.printDisplayInfo();
    sdcw.print();
}

void WinapiHelper::clone()
{
    cloneDisplay3(sdcw, cdsw);
}

void WinapiHelper::disconnect()
{
    disconnectDisplay3(sdcw, cdsw);
}

void WinapiHelper::extend()
{
    extendDisplay3(sdcw, cdsw);
}

bool WinapiHelper::isCorrectDisplayCount()
{
    return cdsw.getDisplayCount() == CorrectDisplayCount;
}
