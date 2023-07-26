#ifndef REGMAP_MANAGER_REGISTERSMAPMANAGER_H
#define REGMAP_MANAGER_REGISTERSMAPMANAGER_H

#include <memory>
#include <utility>
#include <mutex>
#include "RegisterInterface.h"
#include "Registers.h"

class RegistersMapManager {
public:
    explicit RegistersMapManager(std::shared_ptr<RegisterInterface> reg_interface)
        : register_interface_(std::move(reg_interface)) {
    }
    ~RegistersMapManager() = default;

    [[nodiscard]] uint32_t getValue(REG reg);
    [[nodiscard]] uint8_t setValue(REG reg, uint32_t value);
    [[nodiscard]] uint8_t resetValue(REG reg);
    [[nodiscard]] uint8_t clearValue(REG reg);
    [[nodiscard]] uint8_t setBit(REG reg, uint8_t bit_index);
    [[nodiscard]] uint8_t clearBit(REG reg, uint8_t bit_index);
    [[nodiscard]] uint8_t getNibble(REG reg, uint8_t nibble_index);
    [[nodiscard]] uint8_t setNibble(REG reg, uint8_t nibble_index, uint8_t nibble_value);
    [[nodiscard]] uint8_t resetAll();
    [[nodiscard]] uint8_t clearAll();
private:
    std::shared_ptr<RegisterInterface> register_interface_;
    std::mutex mtx_;
};

#endif //REGMAP_MANAGER_REGISTERSMAPMANAGER_H
