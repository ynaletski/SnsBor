
#define Test 0  // 1 - тестовый режим без MVD

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include <time.h>
#include <sys\types.h>
#include <sys\timeb.h>

#include "device.h"

#include "PROC.h"

#if defined(MMI_ICP)
#include "MMI.h"
#endif

#if defined(MMI_NEW)
#include "MMI_NEW.h"
#endif

#include "queue.h"
#include "icp.h"
//#include "mmi.h"
#include "host.h"
#include "i7060.h"
#include "eee.h"
#include "mvd.h"
#include "rtu.h"
#include "arch.h"
#include "slave.h"
#include "slave2.h"
//#include "ref_t.h"
#include "digimag.h"
#include "sens.h"
#include "vol.h"
#include "avt_ctrl.h"

#if defined(PMP201)
#define s_DGM s_PMP
#endif

