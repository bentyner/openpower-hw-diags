#include <common/interfaces.hpp>

#include <gmock/gmock.h>

class MockUtils : public UtilSource
{
  public:
    MOCK_METHOD(bool, autoRebootEnabled, (), (override));
};
