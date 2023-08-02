#include "RegistersMapManager.h"

constexpr uint8_t kMaxBitIndex = 31;
constexpr uint8_t kMaxNibbleValue = 0xF;
constexpr uint8_t kMaxNibbleIndex = 7;

uint32_t RegistersMapManager::getDefaultValue(REG reg) {
    return (*p_registers_map_)[reg].default_value;
}

uint32_t RegistersMapManager::getValue(REG reg) {
    std::lock_guard<std::mutex> lock(mtx_);
    return p_register_interface_->get((*p_registers_map_)[reg].address);
}

uint8_t RegistersMapManager::setValue(REG reg, uint32_t value) {
    std::lock_guard<std::mutex> lock(mtx_);
    return p_register_interface_->set((*p_registers_map_)[reg].address, value);
}

uint8_t RegistersMapManager::resetValue(REG reg) {
    return setValue(reg, (*p_registers_map_)[reg].default_value);
}

uint8_t RegistersMapManager::clearValue(REG reg) {
    return setValue(reg, 0);
}

uint8_t RegistersMapManager::setBit(REG reg, uint8_t bit_index) {
    if(bit_index > kMaxBitIndex) {
        return 1;
    }
    auto reg_value = getValue(reg);
    reg_value |= 1UL << bit_index;

    return setValue(reg, reg_value);
}

uint8_t RegistersMapManager::clearBit(REG reg, uint8_t bit_index) {
    if(bit_index > kMaxBitIndex) {
        return 1;
    }

    auto reg_value = getValue(reg);
    reg_value &= ~(1UL << bit_index);

    return setValue(reg, reg_value);
}

uint8_t RegistersMapManager::getNibble(REG reg, uint8_t nibble_index) {
    if(nibble_index > kMaxNibbleIndex) {
        return 1;
    }

    auto reg_value = getValue(reg);

    return (reg_value >> (nibble_index * 4)) & 0xF;
}

uint8_t RegistersMapManager::setNibble(REG reg, uint8_t nibble_index, uint8_t nibble_value) {
    if(nibble_index > kMaxNibbleIndex || nibble_value > kMaxNibbleValue) {
        return 1;
    }

    auto reg_value = getValue(reg);
    reg_value = (reg_value & ~(0xF << (nibble_index * 4))) | (nibble_value << (nibble_index * 4));

    return setValue(reg, reg_value);
}

uint8_t RegistersMapManager::resetAll() {
    uint8_t result{};

    for(auto reg : *p_registers_map_) {
        uint8_t error = setValue(reg.first, (*p_registers_map_)[reg.first].default_value);
        if (error) {
            result++;
            break;
        }
    }

    return result;
}

uint8_t RegistersMapManager::clearAll() {
    uint8_t result{};

    for(auto reg : *p_registers_map_) {
        uint8_t error = setValue(reg.first, 0);
        if (error) {
            result++;
            break;
        }
    }

    return result;
}
