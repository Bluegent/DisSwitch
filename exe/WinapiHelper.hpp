#pragma once

#include <CDSWrapper.hpp>
#include <SDCWrapper.hpp>

class WinapiHelper
{
private:
    SDCWrapper cds;
    CDSWrapper sdc;
public:
    void list();
    void clone();
    void disconnect();
    void extend();
    bool isCorrectDisplayCount();
};