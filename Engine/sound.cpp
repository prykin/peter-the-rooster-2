/* 
PisteEngine - PisteSound 0.1	
09.12.2001	Janne Kivilahti / Piste Gamez
*/

/* INCLUDES ----------------------------------------------------------------------------------*/

#include "sound.h"
#include "sfx.h"
#include "log.h"
//#include "../old_headers.h"

/* VARIABLES ---------------------------------------------------------------------------------*/

const int MAX_SFX = 500;

unsigned char PS_channels;                // 1 = mono, 2 = stereo
unsigned long PS_samplerate;
unsigned char PS_bitrate;                // 8 tai 16

HWND PS_main_window_handle = NULL; // globally track main window
HINSTANCE PS_hinstance_app = NULL; // globally track hinstance

LPDIRECTSOUND PS_lpds;
LPDIRECTSOUNDBUFFER PS_lpdsbuffer;
DSBUFFERDESC PS_dsbd;                        // direct soundin bufferin kuvaus
WAVEFORMATEX PS_pcmwf;

PisteSFX *sounds[MAX_SFX];

char PS_error[100];

bool PS_unload = true;

/* PROTOTYPES --------------------------------------------------------------------------------*/

/* METHODS -----------------------------------------------------------------------------------*/

int
PisteSound_Init(HWND &main_window_handle, HINSTANCE &hinstance_app, unsigned char channels, unsigned long sample_rate, unsigned char bit_rate) {
    if (PS_unload) {

        PS_main_window_handle = (HWND &) main_window_handle;
        PS_hinstance_app = (HINSTANCE &) hinstance_app;

        strcpy(PS_error, "unknown error with sound system.");

        if (FAILED(DirectSoundCreate(NULL, &PS_lpds, NULL))) {
            strcpy(PS_error, "could not create direct sound surface.");
            PisteLog_Write("[Error] Piste Sound: Could not create direct sound surface!\n");
            return PS_ERROR;
        }

        if (FAILED(PS_lpds->SetCooperativeLevel(PS_main_window_handle, DSSCL_NORMAL))) {
            strcpy(PS_error, "sound system could not cooperate with windows.");
            PisteLog_Write("[Error] Piste Sound: Could not cooperate with Windows!\n");
            return PS_ERROR;
        }

        PS_channels = channels;
        PS_samplerate = sample_rate;
        PS_bitrate = bit_rate;

        for (int i = 0; i < MAX_SFX; i++)
            if (sounds[i])
                delete sounds[i];

        PS_unload = false;
    }

    return 0;
}

int PisteSound_SFX_New(char *filename) {
    int i = 0, virhe;
    bool found = false;

    PisteLog_Write("- Piste Sound: Loading sound file: ");
    PisteLog_Write(filename);
    PisteLog_Write("\n");

    /* Searching if the loaded sound was already found in the same file */

    for (i = 0; i < MAX_SFX; i++)
        if (sounds[i] != NULL)
            if (strcmp(filename, sounds[i]->file) == 0 && strcmp(sounds[i]->file, " ") != 0)
                return i;

    i = 0;

    while (i < MAX_SFX && !found) {
        if (sounds[i] == NULL) {
            sounds[i] = new PisteSFX(PS_channels, PS_samplerate, PS_bitrate);

            if ((virhe = sounds[i]->Load(filename, DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY,
                                          PS_lpds, PS_pcmwf, PS_dsbd)) > 0) {
                strcpy(PS_error, "could not load file ");
                strcat(PS_error, filename);
                if (virhe == 1)
                    strcat(PS_error, ": \n target buffer is not empty.");
                if (virhe == 2)
                    strcat(PS_error, ": \n no file / wrong type / locked.");
                if (virhe == 3)
                    strcat(PS_error, ": \n duplicate buffers failed.");

                PisteLog_Write("[Error] Piste Sound: ");
                PisteLog_Write(PS_error);
                PisteLog_Write("\n");
                return PS_ERROR;
            }

            found = true;
        }
        i++;
    }

    if (!found) {
        strcpy(PS_error, "can't load more sounds.");
        PisteLog_Write("[Error] Piste Sound: Maximum limit of sounds exceeded!\n");
        return PS_ERROR;
    }

    return i - 1;
}

int PisteSound_ToggleSoundOn(bool play) {
    PisteSFX_Set_Sfx(play);
    return 0;
}

int PisteSound_SFX_Destroy(int sfx_index) {
    int i = 0;
    bool found = false;


    if (sfx_index > -1 && sfx_index < MAX_SFX)
        if (sounds[sfx_index] != NULL) {
            sounds[sfx_index]->Destroy();
            sounds[sfx_index] = NULL;
        }

    return i;
}

int PisteSound_SFX_Play(int sfx_index) {
    if (sounds[sfx_index] != NULL)
        if (sounds[sfx_index]->Play() != 0) {
            strcpy(PS_error, "error playing movement_x sound");
            return PS_ERROR;
        }

    return 0;
}

int PisteSound_SFX_Soita(int sfx_index, int volume) {
    if (sounds[sfx_index] != NULL)
        if (sounds[sfx_index]->Play(volume) != 0) {
            strcpy(PS_error, "error playing movement_x sound");
            PisteLog_Write("[Error] Piste Sound: Error playing movement_x sound!\n");
            return PS_ERROR;
        }

    return 0;
}

int PisteSound_SFX_Play(int sfx_index, int volume, int pan, int freq) {
    if (sounds[sfx_index] != NULL)
        if (sounds[sfx_index]->Play(volume, pan, freq) != 0) {
            strcpy(PS_error, "error playing movement_x sound");
            PisteLog_Write("[Error] Piste Sound: Error playing movement_x sound!\n");
            return PS_ERROR;
        }

    return 0;
}

int PisteSound_Stop(void) {
    if (!PS_unload) {

        for (int i = 0; i < MAX_SFX; i++)
            if (sounds[i] != NULL) {
                delete sounds[i];
                sounds[i] = NULL;
            }

        if (PS_lpdsbuffer) {
            PS_lpdsbuffer->Release();
        }

        if (PS_lpds) {
            PS_lpds->Release();
        }

        PS_unload = true;
    }
    return 0;
}

LPDIRECTSOUND PisteSound_Get_DirectSound() {
    return PS_lpds;
}

char *PisteSound_GetError() {
    return PS_error;
}