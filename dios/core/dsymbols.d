module core.dsymbols;

extern(C):

private alias extern(C) int function(char[][] args) MainFunc;

int _d_run_main(int argc, char **argv, MainFunc mainFunc)
{
    return 0;
}

void _d_dso_registry() {}
void _d_arraybounds(char[] file, uint line) {}

void _d_assert(char[] file, uint line) {}

void _d_unittest(char[] file, uint line) {}

__gshared void* _Dmodule_ref = null;

__gshared int _D15TypeInfo_Struct6__vtblZ = 0;

