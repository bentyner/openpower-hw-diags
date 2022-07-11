#include <common/interfaces.hpp>
#include <util/pdbg.hpp>

#include <gmock/gmock.h>

class MockHardware : public HardwareSource
{
  public:
    MOCK_METHOD(int, getCfam,
                (pdbg_target * i_trgt, uint32_t i_addr, uint32_t& o_val),
                (override));
};
