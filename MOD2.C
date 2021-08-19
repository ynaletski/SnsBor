
#include "ALL.h"

#if defined(PMP201)
#define DGM PMP
#define s_DGM s_PMP
#endif

#include "dos_winw.c"
#include "rtu.c"
#include "arch.c"
#include "slave.c"
#include "slave2.c"
#include "prop_but.c"
#include "vol.c"

#if defined(DIGIMAG)
 #include "digimag.c"
#endif


#if defined(PMP201)
 #include "sens.c"
#endif
