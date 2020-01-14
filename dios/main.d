module main;

import core.dsymbols;

import core.stdio;
import core.error;

import core.multiboot;
import core.video;
import core.console;
import core.stdarg;
import core.port;

extern(C):

multiboot_info* getMultibootInfo(uint addr, uint magic)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        kPanic("Invalid Multiboot magic number");
    }

    multiboot_info* mbi = cast(multiboot_info*)addr;

    return mbi;
}

__gshared string MemAvailable = "Available";
__gshared string MemReserved = "Reserved";

void switchDisplayMode40x25()
{
    asm
    {
        mov AH, 0;
        int 0x10;
    }
}

void restartKeyboard()
{    
   ubyte data;
   kPortReadByte(0x61, data);     
   kPortWriteByte(0x61, data | 0x80); //Disables the keyboard  
   kPortWriteByte(0x61, data & 0x7F); //Enables the keyboard  
}

ubyte getScancode()
{
    ubyte data;
    kPortReadByte(0x60, data);
    return data;
}

void main(uint addr, uint magic) 
{
    Console.init();
    //multiboot_info* mbi = checkMultiboot(addr, magic);

    kprintf("DIOS 0.0.1\n");
    kprintf("---------------\n");

    multiboot_info* mbi = getMultibootInfo(addr, magic);
    kprintf("Multiboot info:\n");
    kprintf("Magic: %x\n", magic);

    if (checkFlag(mbi.flags, 2))
        kprintf("Arguments: %s\n", cast(char*)mbi.cmdline);

    kprintf("Boot loader: %s\n", cast(char*)mbi.boot_loader_name);

    kprintf("Memory:\n");
    uint lowerMemory = mbi.mem_lower; // between 0 and 640KB
    uint upperMemory = mbi.mem_upper; // from 1MB
    uint totalMemory = (1024 + mbi.mem_upper) / 1024 + 1;
    kprintf("Total memory: %u MB\n", totalMemory);

    uint upperMemAdr = 0;
    uint upperMemLen = 0;

    kprintf("Memory map:\n");
    uint entryNum = 0;
    if (checkFlag(mbi.flags, 6))
    {
        multiboot_memory_map_t* mmap = cast(multiboot_memory_map_t*)(mbi.mmap_addr);

        for (mmap = cast(multiboot_memory_map_t*) mbi.mmap_addr;
             cast(ulong)mmap < mbi.mmap_addr + mbi.mmap_length;
             mmap = cast(multiboot_memory_map_t*)(cast(ulong)mmap + mmap.size + mmap.size.sizeof))
        {
            kprintf(" Entry %u: ", entryNum);

            kprintf("address: %x, ", mmap.addr_low);

            if (mmap.length_low >= 1024 * 1024)
                kprintf("length: %u MB, ", (mmap.length_low / 1024) / 1024);
            else if (mmap.length_low >= 1024)
                kprintf("length: %u KB, ", mmap.length_low / 1024);
            else
                kprintf("length: %u B, ", mmap.length_low);

            kprintf("type: %s\n", (mmap.type == 1)? cast(char*)MemAvailable : cast(char*)MemReserved);

            if ((mmap.type == 1) && (mmap.length_low > upperMemLen))
            {
                upperMemAdr = mmap.addr_low;
                upperMemLen = mmap.length_low;
            }

            entryNum++;
        }
    }

    kprintf("Available memory:\n");
    kprintf(" address: %x\n", upperMemAdr);
    if (upperMemLen >= 1024 * 1024)
        kprintf(" length: %u MB\n", (upperMemLen / 1024) / 1024);
    else if (upperMemLen >= 1024)
        kprintf(" length: %u KB\n", upperMemLen / 1024);
    else
        kprintf(" length: %u B\n", upperMemLen);

    //core.memory.Initialize();
    //kprintf("kernelstart = %x\n", kernelstart);
    //kprintf("kernelend = %x\n", kernelend);

    //restartKeyboard();

    for (;;) 
    {
    }
}

