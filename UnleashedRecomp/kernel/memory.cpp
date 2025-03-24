#include <stdafx.h>
#include "memory.h"

Memory::Memory()
{
#ifdef _WIN32
    base = (uint8_t*)VirtualAlloc(nullptr, PPC_MEMORY_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    DWORD oldProtect;
    VirtualProtect(base, 4096, PAGE_NOACCESS, &oldProtect);
#else
    base = (uint8_t*)mmap(NULL, PPC_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    mprotect(base, 4096, PROT_NONE);
#endif

    for (size_t i = 0; PPCFuncMappings[i].guest != 0; i++)
    {
        if (PPCFuncMappings[i].host != nullptr)
            InsertFunction(PPCFuncMappings[i].guest, PPCFuncMappings[i].host);
    }
}

void* MmGetHostAddress(uint32_t ptr)
{
    return g_memory.Translate(ptr);
}
