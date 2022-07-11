#pragma once

#include <util/pdbg.hpp>

#include <memory>

class HardwareSource
{
  public:
    virtual ~HardwareSource() {}
    virtual int getCfam(pdbg_target* i_trgt, uint32_t i_addr,
                        uint32_t& o_val) = 0;
};

class UtilSource
{
  public:
    virtual ~UtilSource() {}
    virtual bool autoRebootEnabled() = 0;
};

class Interfaces
{
  public:
    HardwareSource& hardware;
    UtilSource& util;

    Interfaces(HardwareSource& hardwareService, UtilSource& utilService) :
        hardware(hardwareService), util(utilService)
    {}
};
