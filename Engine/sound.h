#ifndef P_SOUND
#define P_SOUND

/* INCLUDES ----------------------------------------------------------------------------------*/

#define DIRECTSOUND_VERSION 0x0700
#include "dsound.h" // TODO: Remember to fix the path
#include <windows.h>
//#include "../old_headers.h"

/* DEFINES -----------------------------------------------------------------------------------*/

#define PS_ERROR  -1

#define SOUND_NULL     0
#define SOUND_LOADED   1
#define SOUND_PLAYING  2
#define SOUND_STOPPED  3

const int MAX_SOUNDS = 100;// Maximum number of sounds in the system at once
const int MAX_POLY_PER_FX = 5;  // Maximum number of the same sound played

/* PROTOTYPES --------------------------------------------------------------------------------*/

int PisteSound_Init(HWND &main_window_handle, HINSTANCE &hinstance_app, unsigned char channels, unsigned long sample_rate, unsigned char bit_rate);

LPDIRECTSOUND PisteSound_Get_DirectSound();

int PisteSound_Stop(void);

int PisteSound_ToggleSoundOn(bool play);

int PisteSound_SFX_Play(int sfx_index);

int PisteSound_SFX_Play(int sfx_index, int volume); /* volume: 0 - 100 */

int PisteSound_SFX_Play(int sfx_index, int volume, int pan, int freq);

/* pan: -10 000 - 10 000 | freq: 100 -> 100 000 (Hz)  */

int PisteSound_SFX_Destroy(int sfx_index);

int PisteSound_SFX_New(char *filename);

char *PisteSound_GetError();

#endif