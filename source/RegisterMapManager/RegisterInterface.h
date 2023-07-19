#ifndef REG_MANAGER_REGISTERINTERFACE_H
#define REG_MANAGER_REGISTERINTERFACE_H

#include <cstdint>

class RegisterInterface {
public:
    RegisterInterface() = default;
    ~RegisterInterface() = default;
    virtual uint32_t get(uint32_t address) = 0;
    virtual uint8_t set(uint32_t address, uint32_t value) = 0;
};

#endif //REG_MANAGER_REGISTERINTERFACE_H
