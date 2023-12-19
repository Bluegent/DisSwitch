#pragma once
#include "SDCDisplayInfo.hpp"


class SDCWrapper
{
private:
    SDCDisplayInfo info;
public:
    SDCWrapper();
    void print();
    void reload();
    const SDCDisplayInfo& getInfo() const;
    int32_t clone(uint32_t target, uint32_t source);
    int32_t disconnect(uint32_t target);
    int32_t applySettings(uint32_t flags);
};

