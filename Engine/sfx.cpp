#include <windows.h>
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream.h> 
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include "sfx.h"

bool play_sfx = true;

PisteSFX::PisteSFX() {
    for (int i = 0; i < MAX_POLY_PER_FX; i++) {
        sound_buffer[i] = NULL;
        state[i] = SOUND_NULL;
    }
    rate = 0;
    size = 0;
    id = -1;
    buffer_index = 0;
    strcpy(this->file, " ");
}

PisteSFX::PisteSFX(unsigned char channels, unsigned long sample_rate, unsigned char bits_per_sample) {
    for (int i = 0; i < MAX_POLY_PER_FX; i++) {
        sound_buffer[i] = NULL;
        state[i] = SOUND_NULL;
    }
    rate = 0;
    size = 0;
    id = -1;
    buffer_index = 0;
    this->channels = channels;
    this->sample_rate = sample_rate;
    this->bits_per_sample = bits_per_sample;
    strcpy(this->file, " ");
}

PisteSFX::~PisteSFX() {
    for (int i = 0; i < MAX_POLY_PER_FX; i++)
        if (this->state[i] != SOUND_NULL) {
            if (this->state[i] == SOUND_PLAYING)
                this->sound_buffer[i]->Stop();
            this->sound_buffer[i]->Release();
            this->state[i] = SOUND_NULL;
        }
}

void PisteSFX::Destroy() {
    for (int i = 0; i < MAX_POLY_PER_FX; i++)
        if (this->sound_buffer[i]) {
            if (this->state[i] == SOUND_PLAYING)
                this->sound_buffer[i]->Stop();
            this->sound_buffer[i]->Release();
            this->state[i] = SOUND_NULL;
            strcpy(this->file, " ");
        }
}

int PisteSFX::Load(char *filename, int control_flags, LPDIRECTSOUND lpds, WAVEFORMATEX pcmwf, DSBUFFERDESC dsbd) {

    if (this->state[0] == SOUND_NULL) {
        if (PisteSFX_LoadWav(this, filename, control_flags, lpds, pcmwf, dsbd) == -1)
            return 2;

        strcpy(this->file, filename);

        this->buffer_index = 0;

        for (int i = 1; i < MAX_POLY_PER_FX; i++) {
            this->sound_buffer[i] = this->sound_buffer[0];
            this->state[i] = this->state[0];
            if (FAILED(lpds->DuplicateSoundBuffer(this->sound_buffer[0], &this->sound_buffer[i]))) {
                this->sound_buffer[i] = NULL;
                this->state[i] = SOUND_LOADED;
                return 3;
            }
        }
    } else
        return 1;

    return 0;
}

int PisteSFX::Play() {
    if (play_sfx) {
        if (FAILED(this->sound_buffer[buffer_index]->SetVolume(0)))
            return 1;
        if (FAILED(this->sound_buffer[buffer_index]->Play(0, 0, 0)))
            return 1;
        this->state[buffer_index] = SOUND_PLAYING;
        buffer_index++;
        if (buffer_index == MAX_POLY_PER_FX)
            buffer_index = 0;
    }
    return 0;
}

int PisteSFX::Play(int volume) {
    if (play_sfx) {
        volume = (100 - volume) * -30;//-100

        if (volume < DSBVOLUME_MIN)
            volume = DSBVOLUME_MIN;

        if (volume > DSBVOLUME_MAX)
            volume = DSBVOLUME_MAX;

        if (FAILED(this->sound_buffer[buffer_index]->SetVolume(volume)))
            return 1;

        if (FAILED(this->sound_buffer[buffer_index]->Play(0, 0, 0)))
            return 1;

        this->state[buffer_index] = SOUND_PLAYING;

        buffer_index++;

        if (buffer_index == MAX_POLY_PER_FX)
            buffer_index = 0;
    }
    return 0;
}

int PisteSFX::Play(int volume, int pan, int freq) {
    if (play_sfx) {
        volume = (100 - volume) * -30;//-100

        if (volume < DSBVOLUME_MIN)
            volume = DSBVOLUME_MIN;

        if (volume > DSBVOLUME_MAX)
            volume = DSBVOLUME_MAX;

        if (pan < -10000)
            pan = -10000;

        if (pan > 10000)
            pan = 10000;

        if (freq < 100)
            freq = 100;

        if (freq > 100000)
            freq = 100000;

        if (FAILED(this->sound_buffer[buffer_index]->SetVolume(volume)))
            return 1;

        if (FAILED(this->sound_buffer[buffer_index]->SetPan(pan)))
            return 1;

        if (FAILED(this->sound_buffer[buffer_index]->SetFrequency(freq)))
            return 1;

        if (FAILED(this->sound_buffer[buffer_index]->Play(0, 0, 0)))
            return 1;

        this->state[buffer_index] = SOUND_PLAYING;

        buffer_index++;

        if (buffer_index == MAX_POLY_PER_FX)
            buffer_index = 0;
    }
    return 0;
}

int PisteSFX_Set_Sfx(bool play) {
    play_sfx = play;
    return 0;
}

bool PisteSFX_Get_Sfx(void) {
    return play_sfx;
}

int PisteSFX_LoadWav(PisteSFX *sound, char *filename, int control_flags, LPDIRECTSOUND lpds, WAVEFORMATEX pcmwf,
                     DSBUFFERDESC dsbd) {
    HMMIO hwav;    // handle to wave file
    MMCKINFO parent,  // parent chunk
    child;   // child chunk
    WAVEFORMATEX wfmtx;   // wave format structure

    int sound_id = -1;       // id of sound to be loaded
    // looping variable
    unsigned char *snd_buffer,       // temporary sound buffer to hold voc data
    *audio_ptr_1 = NULL, // data ptr to first write buffer
    *audio_ptr_2 = NULL; // data ptr to second write buffer

    unsigned long audio_length_1 = 0,  // length of first write buffer
    audio_length_2 = 0;  // length of second write buffer

    if (sound->state[0] != SOUND_NULL)
        return (-1);

// set up chunk info structure
    parent.ckid = (FOURCC) 0;
    parent.cksize = 0;
    parent.fccType = (FOURCC) 0;
    parent.dwDataOffset = 0;
    parent.dwFlags = 0;

// copy data
    child = parent;

// open the WAV file
    if ((hwav = mmioOpen(filename, NULL, MMIO_READ | MMIO_ALLOCBUF)) == NULL)
        return (-1);

// descend into the RIFF 
    parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

    if (mmioDescend(hwav, &parent, NULL, MMIO_FINDRIFF)) {
        // close the file
        mmioClose(hwav, 0);

        // return error, no wave section
        return (-1);
    } // end if

// descend to the WAVEfmt 
    child.ckid = mmioFOURCC('f', 'm', 't', ' ');

    if (mmioDescend(hwav, &child, &parent, 0)) {
        // close the file
        mmioClose(hwav, 0);

        // return error, no format section
        return (-1);
    } // end if

// now read the wave format information from file
    if (mmioRead(hwav, (char *) &wfmtx, sizeof(wfmtx)) != sizeof(wfmtx)) {
        // close file
        mmioClose(hwav, 0);

        // return error, no wave format data
        return (-1);
    } // end if

// make sure that the data format is PCM
    if (wfmtx.wFormatTag != WAVE_FORMAT_PCM) {
        // close the file
        mmioClose(hwav, 0);

        // return error, not the right data format
        return (-1);
    } // end if

// now ascend up one level, so we can access data chunk
    if (mmioAscend(hwav, &child, 0)) {
        // close file
        mmioClose(hwav, 0);

        // return error, couldn't ascend
        return (-1);
    } // end if

// descend to the data chunk 
    child.ckid = mmioFOURCC('d', 'a', 't', 'a');

    if (mmioDescend(hwav, &child, &parent, MMIO_FINDCHUNK)) {
        // close file
        mmioClose(hwav, 0);

        // return error, no data
        return (-1);
    } // end if

// finally!!!! now all we have to do is read the data in and
// set up the directsound buffer

// allocate the memory to load sound data
    snd_buffer = (unsigned char *) malloc(child.cksize);

// read the wave data 
    mmioRead(hwav, (char *) snd_buffer, child.cksize);

// close the file
    mmioClose(hwav, 0);

// set rate and size in data structure
    sound->rate = wfmtx.nSamplesPerSec;
    sound->size = child.cksize;
    sound->state[0] = SOUND_LOADED;

// set up the format data structure
    memset(&pcmwf, 0, sizeof(WAVEFORMATEX));

    pcmwf.wFormatTag = WAVE_FORMAT_PCM;  // pulse code modulation
    pcmwf.nChannels = sound->channels;
    pcmwf.nSamplesPerSec = sound->sample_rate;            // always this rate
    pcmwf.nBlockAlign = 1;
    pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
    pcmwf.wBitsPerSample = sound->bits_per_sample;
    pcmwf.cbSize = 0;

// prepare to create sounds buffer
    dsbd.dwSize = sizeof(DSBUFFERDESC);
    dsbd.dwFlags = control_flags | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
    dsbd.dwBufferBytes = child.cksize;
    dsbd.lpwfxFormat = &pcmwf;

// create the sound buffer
    if (FAILED(lpds->CreateSoundBuffer(&dsbd, &sound->sound_buffer[0], NULL))) {
        // release memory
        free(snd_buffer);

        // return error
        return (-1);
    } // end if

// copy data into sound buffer
    if (FAILED(
            sound->sound_buffer[0]->Lock(0, child.cksize, (void **) &audio_ptr_1, &audio_length_1, (void **) &audio_ptr_2,
                                         &audio_length_2, DSBLOCK_FROMWRITECURSOR)))
        return (0);

// copy first section of circular buffer
    memcpy(audio_ptr_1, snd_buffer, audio_length_1);

// copy last section of circular buffer
    memcpy(audio_ptr_2, (snd_buffer + audio_length_1), audio_length_2);

// unlock the buffer
    if (FAILED(sound->sound_buffer[0]->Unlock(audio_ptr_1, audio_length_1, audio_ptr_2, audio_length_2)))
        return (0);

// release the temp buffer
    free(snd_buffer);

// return id
    return (0);

} // end DSound_Load_WAV