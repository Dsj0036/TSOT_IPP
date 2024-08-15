

#include <cellstatus.h>
#include <sys/prx.h>
#include "System.h"
#include "src\debug.h"


#define BUILD_DATE  __DATE__ " " __TIME__

SYS_MODULE_INFO( TSoT, 0, 1, 1);
SYS_MODULE_START( _TSoT_prx_entry );


#define _INGAME_DEBUG_PRINTING_ARGS int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11

ONCE(debug_printing)

char buffer[128];


CREATE_DUMMY_STUB(int, _ingame_debug_printing, _INGAME_DEBUG_PRINTING_ARGS);
CREATE_DUMMY_STUB(int, _ingame_debug_printing2, _INGAME_DEBUG_PRINTING_ARGS);


int patched_debug_printing(_INGAME_DEBUG_PRINTING_ARGS) {
    _sys_memset(buffer, 0, 128);
    const char* format = (char*)a1;
    snprintf(buffer, 128, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
    return _ingame_debug_printing(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
}



extern "C" int _TSoT_prx_entry(void)
{
    Debug::Initialize();

    sys_sleep(15000);

    Debug::coutf("The Stick Of The Truth | Reversing Initialization | Build %s\n", BUILD_DATE);

    // This both functions are used in game for output printing.

    hookfunction(0x00156570, take(patched_debug_printing), take(_ingame_debug_printing));
    hookfunction(0x0051282C, take(patched_debug_printing), take(_ingame_debug_printing2));



    return SYS_PRX_RESIDENT;
}
