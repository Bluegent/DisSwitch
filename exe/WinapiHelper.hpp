#pragma once

#include <CDSWrapper.hpp>
#include <SDCWrapper.hpp>

class WinapiHelper
{
private:
    SDCWrapper sdcw;
    CDSWrapper cdsw;
public:
    void list();
    void clone();
    void disconnect();
    void extend();
    bool isCorrectDisplayCount();
private:
    bool isInCloneMode();
    bool display3ConnectedSDC();
    bool display3ConnectedCDS();
    bool display3Connected(bool useCDS = false);
    int32_t getDisplay3Index();
    void connectDisplay3();
    
};