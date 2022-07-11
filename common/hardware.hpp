#include <common/interfaces.hpp>
#include <util/pdbg.hpp>

class Hardware : public HardwareSource
{
  public:
    virtual ~Hardware() {}
    virtual int getCfam(pdbg_target* i_trgt, uint32_t i_addr,
                        uint32_t& o_val) override
    {
        return util::pdbg::getCfam(i_trgt, i_addr, o_val);
    }
};
