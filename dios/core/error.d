module core.error;

private
{
    import core.consoletypes;
    import core.stdio;
}

extern(C):

void kAssert(bool condition, string failMessage = "")
{
    if(!condition)
        kPanic(failMessage);
}

void kPanic(string message = "")
{
    kprintf(message);
    for(;;) { }		
}
