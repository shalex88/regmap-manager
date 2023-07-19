#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "../source/RegisterMapManager/RegisterInterface.h"

// TEST PLAN
// + Get 32bit register value from address
// + Set 32bit register value on address, return 1 if ok

class RegisterMock : public RegisterInterface {
public:
    MOCK_METHOD1(get, uint32_t(uint32_t address));
    MOCK_METHOD2(set, uint8_t(uint32_t address, uint32_t value));
};

TEST(RegisterInterfaceTest, GetRegisterValue) {
    auto spi_interface = std::make_shared<RegisterMock>();

    EXPECT_CALL(*spi_interface, get(0x0000'0001))
            .WillOnce(testing::Return(0xFFFF'FFFF));
	ASSERT_EQ(spi_interface->get(0x0000'0001),0xFFFF'FFFF);
}

TEST(RegisterInterfaceTest, SetRegisterValue) {
    auto spi_interface = std::make_shared<RegisterMock>();

    EXPECT_CALL(*spi_interface, set(0x0000'0001, 0xFFFF'FFFF))
            .WillOnce(testing::Return(1));
	ASSERT_EQ(spi_interface->set(0x0000'0001, 0xFFFF'FFFF), 1);
}
