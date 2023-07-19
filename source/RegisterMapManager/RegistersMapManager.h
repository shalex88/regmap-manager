#ifndef REG_MANAGER_REGISTERSMAPMANAGER_H
#define REG_MANAGER_REGISTERSMAPMANAGER_H

#include <memory>
#include <utility>
#include "RegisterInterface.h"
#include "Registers.h"

class RegistersMapManager {
public:
    explicit RegistersMapManager(std::shared_ptr<RegisterInterface> reg_interface)
        : register_interface_(std::move(reg_interface)) {
    }
    ~RegistersMapManager() = default;

    [[nodiscard]] uint32_t getValue(REG reg) const;
    [[nodiscard]] uint8_t setValue(REG reg, uint32_t value) const;
    [[nodiscard]] uint8_t resetValue(REG reg) const;
    [[nodiscard]] uint8_t clearValue(REG reg) const;
    [[nodiscard]] uint8_t setBit(REG reg, uint8_t bit_index) const;
    [[nodiscard]] uint8_t clearBit(REG reg, uint8_t bit_index) const;
    [[nodiscard]] uint8_t getNibble(REG reg, uint8_t nibble_index) const;
    [[nodiscard]] uint8_t setNibble(REG reg, uint8_t nibble_index, uint8_t nibble_value) const;
    [[nodiscard]] uint32_t resetAll() const;
    [[nodiscard]] uint32_t clearAll() const;
private:
    std::shared_ptr<RegisterInterface> register_interface_;
};

#endif //REG_MANAGER_REGISTERSMAPMANAGER_H
