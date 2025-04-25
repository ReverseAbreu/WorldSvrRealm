#pragma once
#pragma once

enum class eHookTypes
{
    JMP32,
    JMP64,
    CALL
};

class Hook
{
private:
    static constexpr size_t HookSizes[] = { 5, 14, 5 };  // Tamanhos dos hooks: JMP32, JMP64, CALL

public:

    static int SetProtect(uintptr_t address, int prot)
    {
        static UINT64 pg_size = 0;

        if (pg_size == 0)
            pg_size = sysconf(_SC_PAGESIZE);

        return mprotect(((void*)(address & ~(pg_size - 1))), pg_size, prot);
    }

    template <typename T>
    static uintptr_t CalculateRelativeAddress(uintptr_t from, T to, eHookTypes hookType)
    {
        return reinterpret_cast<uintptr_t>(to) - (from + HookSizes[(int)hookType]);
    }

    template <typename T>
    static void SetHook(T function, uintptr_t address, eHookTypes hookType)
    {
        Hook::SetProtect(address, PROT_READ | PROT_WRITE | PROT_EXEC);

        unsigned char buff[14] = { 0 };

        switch (hookType)
        {
        case eHookTypes::JMP32:
            buff[0] = 0xE9;
            *reinterpret_cast<uintptr_t*>(&buff[1]) = CalculateRelativeAddress(address, function, hookType);
            break;
        case eHookTypes::JMP64:
            buff[0] = 0xFF;
            buff[1] = 0x25;
            *reinterpret_cast<uintptr_t*>(&buff[2]) = 0;
            *reinterpret_cast<uint64_t*>(&buff[6]) = reinterpret_cast<uint64_t>(function);
            break;
        case eHookTypes::CALL:
            buff[0] = 0xE8;
            *reinterpret_cast<uintptr_t*>(&buff[1]) = CalculateRelativeAddress(address, function, hookType);
            break;
        default:
            return;
        }

        std::memcpy(reinterpret_cast<void*>(address), buff, HookSizes[static_cast<int>(hookType)]);
    }

    template <typename T>
    static void Write(uintptr_t address, T value)
    {
        Hook::SetProtect(address, PROT_READ | PROT_WRITE | PROT_EXEC);

        std::memcpy(reinterpret_cast<void*>(address), &value, sizeof(T));
    }

};
