module core.stdio;

private
{
    import core.stdarg;
    import core.console;
}

public:

extern(C):

void kprintf(string fmt, ...)
{
    va_list ap;
    va_start!(string)(ap, fmt);
    Console.writef(fmt, ap);
    va_end(ap);
}

/+
Receiving Input

The keyboard communicates with your computer through a chip called 8042 
(on modern system, the functionality of that chip is emulated by the chipset). 
Any key press or key release leads to the transmission of a scancode to the 8042 
which then raises IRQ1 and makes the scancode available in its data port (port 0x60).
From your point of view, things are as easy as
 void KeyboardIsr()
 {
    byte new_scan_code = inportb(0x60);
 
    /* Do something with the scancode.
     * Remember you only get '''one''' byte of the scancode each time the ISR is invoked.
     * (Though most of the times the scancode is only one byte.) 
     */
 
    /* Acknowledge the IRQ, pretty much tells the PIC that we can accept >=priority IRQs now. */
    outportb(0x20,0x20);
 }
+/

