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
};