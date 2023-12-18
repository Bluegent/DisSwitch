#pragma once

#include <CDSWrapper.h>
#include <SDCWrapper.h>

class WinapiHelper
{
private:
    CDSWrapper cds;
    SDCWrapper sdc;
public:
    void list();
    void clone();
    void disconnect();
    void extend();
    bool isCorrectDisplayCount();
};