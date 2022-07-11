#include <common/interfaces.hpp>
#include <util/dbus.hpp>

class Utils : public UtilSource
{
  public:
    virtual ~Utils() {}
    virtual bool autoRebootEnabled() override
    {
        return util::dbus::autoRebootEnabled();
    }
};
