#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "../source/RegisterMapManager/RegistersMapManager.h"
#include "RegistersMap.h"
#include <thread>

class RegisterMock : public RegisterInterface {
public:
    MOCK_METHOD1(get, uint32_t(uint32_t address));
    MOCK_METHOD2(set, uint8_t(uint32_t address, uint32_t value));
};

class RegisterMapManagerTest : public testing::Test {
public:
    RegisterMapManagerTest() :
            p_register_interface(std::make_shared<RegisterMock>()),
            p_registers_manager(std::make_shared<RegistersMapManager>(p_register_interface, p_registers_map)) {}
    std::shared_ptr<RegisterMock> p_register_interface;
    std::shared_ptr<RegistersMapManager> p_registers_manager;
};

TEST_F(RegisterMapManagerTest, GetRegisterValue) {
    EXPECT_CALL(*p_register_interface, get(testing::_))
            .WillOnce(testing::Return(0xFFFF'FFFF));

    EXPECT_EQ(p_registers_manager->getValue(REG::VIDEO), 0xFFFF'FFFF);
}

TEST_F(RegisterMapManagerTest, SetRegisterValue) {
    EXPECT_CALL(*p_register_interface, set(testing::_, 0xFFFF'FFFF))
            .WillOnce(testing::Return(0));

    EXPECT_EQ(p_registers_manager->setValue(REG::VIDEO, 0xFFFF'FFFF), 0);
}

TEST_F(RegisterMapManagerTest, ResetRegisterToDefault) {
    EXPECT_CALL(*p_register_interface, set(testing::_, testing::_))
            .WillOnce(testing::Return(0));

    EXPECT_EQ(p_registers_manager->resetValue(REG::VIDEO), 0);
}

TEST_F(RegisterMapManagerTest, ClearRegister) {
    EXPECT_CALL(*p_register_interface, set(testing::_, 0))
            .WillOnce(testing::Return(0));

    EXPECT_EQ(p_registers_manager->clearValue(REG::VIDEO), 0);
}

TEST_F(RegisterMapManagerTest, SetBit) {
    EXPECT_CALL(*p_register_interface, get(testing::_))
            .WillOnce(testing::Return(0x0000'0000));
    EXPECT_CALL(*p_register_interface, set(testing::_, 0x0000'0001))
            .WillOnce(testing::Return(0));

    EXPECT_EQ(p_registers_manager->setBit(REG::VIDEO, 0), 0);
}

TEST_F(RegisterMapManagerTest, ClearBit) {
    EXPECT_CALL(*p_register_interface, get(testing::_))
            .WillOnce(testing::Return(0xFFFF'FFFF));
    EXPECT_CALL(*p_register_interface, set(testing::_, 0xFFFF'FFFE))
            .WillOnce(testing::Return(0));

    EXPECT_EQ(p_registers_manager->clearBit(REG::VIDEO, 0), 0);
}

TEST_F(RegisterMapManagerTest, GetOrSetBitLargerThan31) {
    EXPECT_CALL(*p_register_interface, get(testing::_)).Times(0);
    EXPECT_CALL(*p_register_interface, set(testing::_, testing::_)).Times(0);

    EXPECT_EQ(p_registers_manager->setBit(REG::VIDEO, 32), 1);
    EXPECT_EQ(p_registers_manager->clearBit(REG::VIDEO, 32), 1);
}

TEST_F(RegisterMapManagerTest, GetNibble) {
    EXPECT_CALL(*p_register_interface, get(testing::_))
            .WillOnce(testing::Return(0xFFFF'FFFF));

    EXPECT_EQ(p_registers_manager->getNibble(REG::VIDEO, 0), 0xF);
}

TEST_F(RegisterMapManagerTest, SetNibble) {
    EXPECT_CALL(*p_register_interface, get(testing::_))
            .WillOnce(testing::Return(0x0000'0000));
    EXPECT_CALL(*p_register_interface, set(testing::_, 0x0000'000F))
            .WillOnce(testing::Return(0));

    EXPECT_EQ(p_registers_manager->setNibble(REG::VIDEO, 0, 0xf), 0);
}

TEST_F(RegisterMapManagerTest, GetSetWrongNibbleIndex) {
    EXPECT_CALL(*p_register_interface, get(testing::_)).Times(0);
    EXPECT_CALL(*p_register_interface, set(testing::_, testing::_)).Times(0);

    EXPECT_EQ(p_registers_manager->getNibble(REG::VIDEO, 8), 1);
    EXPECT_EQ(p_registers_manager->setNibble(REG::VIDEO, 8, 0xf), 1);
}

TEST_F(RegisterMapManagerTest, SetWrongNibbleValue) {
    EXPECT_CALL(*p_register_interface, get(testing::_)).Times(0);
    EXPECT_CALL(*p_register_interface, set(testing::_, testing::_)).Times(0);

    EXPECT_EQ(p_registers_manager->setNibble(REG::VIDEO, 0, 0xff), 1);
}

TEST_F(RegisterMapManagerTest, ResetAllToDefault) {
    EXPECT_CALL(*p_register_interface, set(testing::_, testing::_))
            .WillRepeatedly(testing::Return(0));

    EXPECT_EQ(p_registers_manager->resetAll(), 0);
}

TEST_F(RegisterMapManagerTest, ClearAllRegisters) {
    EXPECT_CALL(*p_register_interface, set(testing::_, testing::_))
            .WillRepeatedly(testing::Return(0));

    EXPECT_EQ(p_registers_manager->clearAll(), 0);
}

TEST_F(RegisterMapManagerTest, ResetAllToDefaultFails) {
    EXPECT_CALL(*p_register_interface, set(testing::_, testing::_))
            .WillRepeatedly(testing::Return(0));

    EXPECT_CALL(*p_register_interface, set(testing::_, testing::_))
            .WillOnce(testing::Return(1));

    EXPECT_EQ(p_registers_manager->resetAll(), 1);
}

TEST_F(RegisterMapManagerTest, ClearAllRegistersFails) {
    EXPECT_CALL(*p_register_interface, set(testing::_, testing::_))
            .WillRepeatedly(testing::Return(0));

    EXPECT_CALL(*p_register_interface, set(testing::_, testing::_))
            .WillOnce(testing::Return(1));

    EXPECT_EQ(p_registers_manager->clearAll(), 1);
}

TEST_F(RegisterMapManagerTest, SetRegisterValueThreadSafety) {
    auto inside_interface_set_func = std::make_shared<std::atomic<bool>>(false);

    ON_CALL(*p_register_interface, set(testing::_, testing::_))
            .WillByDefault([inside_interface_set_func](int address, uint32_t value) mutable {
                if (*inside_interface_set_func) {
                    return 1;
                }
                *inside_interface_set_func = true;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                *inside_interface_set_func = false;
                return 0;
            });

    std::atomic<bool> error_flag(false);

    std::thread thread1([&]() {
        for (int i = 0; i < 1000; ++i) {
            if (p_registers_manager->setValue(REG::VIDEO, 0xFFFF'FFFF)) {
                error_flag.store(true);
            }
        }
    });

    std::thread thread2([&]() {
        for (int i = 0; i < 1000; ++i) {
            if (p_registers_manager->setValue(REG::VIDEO, 0x0000'0000)) {
                error_flag.store(true);
            }
        }
    });

    thread1.join();
    thread2.join();

    EXPECT_EQ(error_flag.load(), false);
}

TEST_F(RegisterMapManagerTest, GetDefaultRegisterValue) {
    EXPECT_EQ(p_registers_manager->getDefaultValue(REG::VIDEO), 0x1);
}

TEST_F(RegisterMapManagerTest, ResisterManagersCanReceiveDifferentMaps) {
    std::shared_ptr<RegistersMap> p_different_registers_map = std::make_shared<RegistersMap>(RegistersMap{
        {REG::VIDEO, {0x80000001, 0xA}},
        {REG::IMU, {0x80000002, 0xB}},
        {REG::LRF, {0x80000003, 0xC}},
    });

    auto p_different_registers_manager = std::make_shared<RegistersMapManager>(p_register_interface, p_different_registers_map);

    EXPECT_NE(p_registers_manager->getDefaultValue(REG::VIDEO), p_different_registers_manager->getDefaultValue(REG::VIDEO));
}
