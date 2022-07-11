#include <libpdbg.h>

#include <attn/attn_config.hpp>
#include <attn/attn_handler.hpp>
#include <common/interfaces.hpp>
#include <mock_hardware.hpp>
#include <mock_utils.hpp>

#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Ref;
using ::testing::Return;
using ::testing::SetArgReferee;

TEST(AttentionTest, End2End)
{
    MockHardware hardwareSource = MockHardware();
    MockUtils utilSource        = MockUtils();
    Interfaces interfaces       = Interfaces(hardwareSource, utilSource);

    // first proc with special attention active
    EXPECT_CALL(hardwareSource, getCfam(_, 0x1007, _))
        .Times(AtLeast(1))
        .WillOnce(DoAll(SetArgReferee<2>(0xa0000000), Return(0)))
        .WillRepeatedly(DoAll(SetArgReferee<2>(0x00000000), Return(0)));

    // all proc with attentions unmasked
    EXPECT_CALL(hardwareSource, getCfam(_, 0x100d, _))
        .Times(AtLeast(1))
        .WillRepeatedly(DoAll(SetArgReferee<2>(0x60000002), Return(0)));

    pdbg_targets_init(nullptr);
    attn::Config config;
    attn::attnHandler(&config, interfaces);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
