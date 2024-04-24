/* 
PisteEngine - PisteWait 1.0	
09.12.2001	Janne Kivilahti / Piste Gamez
*/

/* INCLUDES ----------------------------------------------------------------------------------*/

#include "wait.h"
#include <time.h>
/* DEFINES -----------------------------------------------------------------------------------*/

/* VARIABLES ---------------------------------------------------------------------------------*/

unsigned long PW_counter = 0;

/* METHODS -----------------------------------------------------------------------------------*/

unsigned long PisteWait_Get(void) {
    return (GetTickCount());
}

unsigned long PisteWait_Start(void) {
    PW_counter = PisteWait_Get();
    return (PW_counter);
}

unsigned long PisteWait_Wait(unsigned long count) {
    while ((PisteWait_Get() - PW_counter) < count);
    return (PisteWait_Get());
}

unsigned long PisteWait_Count_Time(unsigned long count) {
    return (PisteWait_Get() - PW_counter);
}