#ifndef P_WAIT
#define P_WAIT

/* INCLUDES ----------------------------------------------------------------------------------*/

//#include <windows.h>
#include "../old_headers.h"

/* DEFINES -----------------------------------------------------------------------------------*/

/* PROTOTYPES --------------------------------------------------------------------------------*/

unsigned long PisteWait_Get(void);

unsigned long PisteWait_Start(void);

unsigned long PisteWait_Wait(unsigned long count);

unsigned long PisteWait_Count_Time(unsigned long count);

#endif