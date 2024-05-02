#ifndef P_SFX
#define P_SFX

#include "sound.h"
#include <io.h>
//#include "../old_headers.h"

class PisteSFX {
private:

    int LoadWav(char *filename, int control_flags, LPDIRECTSOUND lpds, WAVEFORMATEX pcmwf, DSBUFFERDESC dsbd);

public:
    LPDIRECTSOUNDBUFFER sound_buffer[MAX_POLY_PER_FX];
    int state[MAX_POLY_PER_FX];
    int rate;
    int size;
    int id;
    int buffer_index;
    char file[_MAX_PATH];
    unsigned char channels;
    unsigned long sample_rate;
    unsigned char bits_per_sample;

    PisteSFX();

    PisteSFX(unsigned char channels, unsigned long sample_rate, unsigned char bits_per_sample);

    virtual ~PisteSFX();

    void Destroy();

    int Play();

    int Play(int volume);

    int Play(int volume, int pan, int freq);

    int Load(char *filename, int control_flags, LPDIRECTSOUND lpds, WAVEFORMATEX pcmwf, DSBUFFERDESC dsbd);
};

int PisteSFX_Set_Sfx(bool play);

bool PisteSFX_Get_Sfx(void);

int PisteSFX_LoadWav(PisteSFX *sound, char *filename, int control_flags, LPDIRECTSOUND lpds, WAVEFORMATEX pcmwf,
                     DSBUFFERDESC dsbd);

#endif