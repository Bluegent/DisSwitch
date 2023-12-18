#include <SDCWrapper.h>
#include <CDSWrapper.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <chrono>
#include <thread>

bool display3Connected(SDCWrapper& sdc)
{
    try
    {
        const Monitor& display3 = sdc.getMonitor(3);
        DEVMODEA d3Mode = sdc.getDevMode(display3.name);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool isinCloneMode(const CDSWrapper& cds)
{
    const auto& info = cds.getInfo();
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

void connectDisplay3(SDCWrapper& sdc)
{
    sdc.printDisplayInfo();
    if (display3Connected(sdc))
    {
        std::cout << "Display 3 already connected:\n";
        sdc.printMonitor(sdc.getMonitor(3));
    }
    else
    {
        sdc.extendDisplay(3, 1, -3840);
    }
}

void cloneDisplay3(CDSWrapper& cds, SDCWrapper& sdc)
{
    if (isinCloneMode(cds))
    {
        std::cout << " Already in clone mode.\n";
        return;
    }
    if (!display3Connected(sdc))
    {
        connectDisplay3(sdc);
    }
    cds.reload();
    std::cout << "Cloning display 1 onto display 3 and setting refresh rate of both to 60...\n";
    cds.clone(3, 1);
    if (sdc.getRefreshRate(1) != 60)
    {
        sdc.setRefreshRate(1, 60);
    }
    sdc.printDisplayInfo();
    cds.reload();
}


void disconnectDisplay3(CDSWrapper& cds, SDCWrapper& sdc)
{
    int32_t disconnectTarget = 3;
    if (isinCloneMode(cds))
    {
        //if we are in clone mode we have to disconnect Path2, not Path3!!!
        disconnectTarget = 2;
    }
    else if (!display3Connected(sdc))
    {
        std::cout << "Not in clone mode and display 3 is not connected.\n";
        return;
    }
    cds.disconnect(disconnectTarget);
    cds.reload();
    sdc.printDisplayInfo();
}

void extendDisplay3(CDSWrapper& cds, SDCWrapper& sdc)
{
    if (isinCloneMode(cds))
    {
        std::cout << "In clone mode, disconnecting display 3...\n";
        cds.disconnect(2);
    }
    connectDisplay3(sdc);
    cds.reload();
    sdc.printDisplayInfo();
}


const std::string cloneCommand("cln");
const std::string extendCommand("ext");
const std::string disconnectCommand("dis");



int main(int argc, const char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Commands:\n";
        std::cout << "\t" << cloneCommand << " - connects display 4 and clones display 2 onto it\n";
        std::cout << "\t" << extendCommand << " - connects display 4 in extended mode\n";
        std::cout << "\t" << disconnectCommand << " - disconnects display 4\n\n";
        std::cout << "All the above assume you have 4 displays connected because this program is for my super specific setup :)\n";
        return 0;
    }

    if (argc != 2)
    {
        std::cout << "Oh no baby, what is you doing???(incorrect argument count)\n";
        return -1;
    }

    SDCWrapper sdc;
    CDSWrapper cds;

    if (sdc.getDisplayCount() != 4)
    {
        std::cout << "Oh no baby, what is you doing???(incorrect monitor count)\n";
        return -1;
    }
    sdc.printDisplayInfo();
    std::string command(argv[1]);
    //std::string command = cloneCommand;

    if (command == cloneCommand)
    {
        cloneDisplay3(cds, sdc);
    }
    else if (command == extendCommand)
    {
        extendDisplay3(cds, sdc);
    }
    else if (command == disconnectCommand)
    {
        disconnectDisplay3(cds, sdc);
    }
    else
    {
        std::cout << "Oh no baby, what is you doing???(unknown command)\n";
    }

    return 0;
}
