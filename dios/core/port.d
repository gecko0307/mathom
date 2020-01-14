module core.port;

extern(C):

void kPortWriteByte(uint port, ubyte b)
{
    ubyte* p = cast(ubyte*)port;
    *p = b;
}

void kPortReadByte(uint port, out ubyte b)
{
    b = *cast(ubyte*)port;
}

/*
void kPortReadByte(in ushort _port, out ubyte _value)
{
    asm
    {
        in AL, DX;
        mov _value, AL;
    }
}

void kPortWriteByte(ushort _port, ubyte _value)
{
    asm
    {
        mov DX,_port;
        mov AL,_value;
        out DX, AL;
    }
}

void kPortWriteBytes(ushort _port, ubyte[] _value)
{
    ubyte* valueptr = _value.ptr;
    uint valuelength = _value.length;
    asm
    {
        cld;
        mov DX, _port;
        mov ESI, valueptr;
        mov ECX, valuelength;
        rep;
        insb;
    }
}

void kPortReadShort(in ushort _port, out ushort _value)
{
    asm
    {
        mov DX, _port;
        in AX, DX;
        mov _value, AX;
    }
}

void kPortWriteShort(ushort _port, ushort _value)
{
    asm
    {
        mov DX, _port;
        mov AX, _value;
        out DX, AX;
    }     
}

void kPortWriteShort(ushort _port, ushort[] _value)
{
    ushort* valueptr = _value.ptr;
    uint valuelength = _value.length;
    asm
    {
        cld;
        mov DX, _port;
        mov ESI, valueptr;
        mov ECX, valuelength;
        rep;
        insw;
    }
}

void kPortReadShort(in ushort _port, out uint _value)
{
    asm
    {
        mov DX, _port;
        in EAX, DX;
        mov _value, EAX;
    }
}

void kPortWriteShort(ushort _port, uint _value)
{
    asm
    {
        mov DX, _port;
        mov EAX, _value;
        out DX, EAX;
    }
}

void kPortWriteShorts(ushort _port, uint[] _value)
{
    uint* valueptr = _value.ptr;
    uint valuelength = _value.length;
    asm
    {
        cld;
        mov DX, _port;
        mov ESI, valueptr;
        mov ECX, valuelength;
        rep;
        insd;
    }
}*/

