module core.console;

import core.video;
import core.stdarg;

import core.consoletypes;

extern(C):

struct Console
{
    public static:

    const char[] hexmap = 
    [	
        '0', '1', '2', '3', 
        '4', '5', '6', '7', 
        '8', '9', 'A', 'B', 
        'C', 'D', 'E', 'F'
    ];

    void init()
    {
        VGAText.clearScreen();
        VGAText.setColors(ConsoleColor.Silver, ConsoleColor.Black);
    }

    void writef(string fmt, va_list ap)
    {
        uint f;
        for (int i = 0; i < fmt.length; i++)
        {
            char c = fmt[i];
            if (c == '%')
            {
                if (++i >= fmt.length)
                    break;

                switch(fmt[i])
                {
                    case 's': // zero-terminated string
                        char* t = va_arg!(char*)(ap);
                        for(char n = *t; n != 0; t++)
                        {
                            n = *t;
                            VGAText.putChar(n);
                        }
                        break;

                    case 'c': // ASCII character
                        VGAText.putChar(va_arg!(char)(ap));
                        break;

                    case 'x': // 8 digit, unsigned 32bit hex integer
                        uint u = va_arg!(uint)(ap);
                        VGAText.putString("0x");
                        char[8] digits;
                        for (int j = 7; j >= 0; j--)
                        {
                            digits[j] = hexmap[u & 0x0F];
                            u >>= 4;
                        }
                        foreach(char d; digits)
                            VGAText.putChar(d);
                        break;

                    case 'k': // 4 digit, unsigned 16bit hex integer
                        ushort u = va_arg!(ushort)(ap);
                        VGAText.putString("0x");
                        char[4] digits;
                        for (int j = 3; j >= 0; j--)
                        {
                            digits[j] = hexmap[u & 0x0F];
                            u >>= 4;
                        }
                        foreach(char d; digits)
                            VGAText.putChar(d);
                        break;

                    case 'd': // signed integer
                        int w = va_arg!(int)(ap);
                        if (w < 0)
                        {
                            f = -w;
                            VGAText.putChar('-');
                        }
                        else
                        {
                            f = w;
                        }
                        goto u2;

                    case 'u': // unsigned integer
                        f = va_arg!(uint)(ap);
                        u2:
                        {
                            char[10] d;
                            int k = 9;
                            do
                            {
                                d[k] = (f % 10) + '0';
                                f /= 10;
                                k--;
                            }
                            while(f && k >= 0);
                            while(++k < 10)
                            {
                                VGAText.putChar(d[k]);
                            }
                            break;
                        }

                    case 'X': // 2 digit, unsigned 8 bit hex integer
                        ubyte b = va_arg!(ubyte)(ap);
                        VGAText.putString("0x");
                        VGAText.putChar(hexmap[(b & 0xF0) >> 4]);
                        VGAText.putChar(hexmap[b & 0x0F]);
                        break;

                    default:
                        VGAText.putChar(fmt[i]);
                        break;
                }
            }
            else
            {
                VGAText.putChar(c);
            }
        }
    }
}

