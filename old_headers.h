#ifndef PEKKAKANA2_OLD_HEADERS_H
#define PEKKAKANA2_OLD_HEADERS_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <wchar.h>
#include <stdlib.h>

using namespace std;

const int _MAX_PATH = 32768;
const int _MAX_DIR = 256;

#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HINSTANCE);
DECLARE_HANDLE(HDC);
//DECLARE_HANDLE(RECT);

#define FAILED(hr) (((HRESULT)(hr)) < 0)

unsigned long GetTickCount();

typedef struct IDirectSoundBuffer   *LPDIRECTSOUNDBUFFER;
typedef struct IDirectSound         *LPDIRECTSOUND;

typedef struct tWAVEFORMATEX {
    unsigned int  wFormatTag;
    unsigned int  nChannels;
    unsigned long nSamplesPerSec;
    unsigned long nAvgBytesPerSec;
    unsigned int  nBlockAlign;
    unsigned int  wBitsPerSample;
    unsigned int  cbSize;
} WAVEFORMATEX, *PWAVEFORMATEX, *NPWAVEFORMATEX, *LPWAVEFORMATEX;

typedef unsigned long FOURCC;
typedef HWND HMMIO;

typedef struct _MMCKINFO {
    FOURCC ckid;
    unsigned long  cksize;
    FOURCC fccType;
    unsigned long  dwDataOffset;
    unsigned long  dwFlags;
} MMCKINFO, *PMMCKINFO, *NPMMCKINFO, *LPMMCKINFO;

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;

typedef struct DSBUFFERDESC {
    unsigned long dwSize;
    unsigned long dwFlags;
    unsigned long dwBufferBytes;
    unsigned long dwReserved;
    LPWAVEFORMATEX lpwfxFormat;
    GUID guid3DAlgorithm;
} DSBUFFERDESC;

typedef struct tagPALETTEENTRY {
    unsigned char peRed;
    unsigned char peGreen;
    unsigned char peBlue;
    unsigned char peFlags;
} PALETTEENTRY, *PPALETTEENTRY, *LPPALETTEENTRY;

int _findclose(intptr_t handle);
char strlwr(string);
char *_getcwd(char *buffer,int maxlen);
char *itoa(int num, char *str, int radix);
char *ltoa(int num, char *str, int radix);
char *_gcvt(double value, int digits, char *buffer);

typedef struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;

intptr_t _findfirst(
   const char *filespec,
   struct _finddata_t *fileinfo
);

int _chdir(
   const char *dirname
);

int _findnext(
   intptr_t handle,
   struct _finddata_t *fileinfo
);

typedef	unsigned long	_fsize_t;

struct _finddata_t
{
    unsigned	attrib;		/* Attributes, see constants above. */
    time_t		time_create;
    time_t		time_access;	/* always midnight local time */
    time_t		time_write;
    _fsize_t	size;
    char		name[FILENAME_MAX];	/* may include spaces. */
};

typedef enum Keyboard_Device
{
    DIK_0,
    DIK_1,
    DIK_2,
    DIK_3,
    DIK_4,
    DIK_5,
    DIK_6,
    DIK_7,
    DIK_8,
    DIK_9,
    DIK_A,
    DIK_ABNT_C1,
    DIK_ABNT_C2,
    DIK_ADD,
    DIK_APOSTROPHE,
    DIK_APPS,
    DIK_AT,
    DIK_AX,
    DIK_B,
    DIK_BACK,
    DIK_BACKSLASH,
    DIK_BACKSPACE,
    DIK_C,
    DIK_CALCULATOR,
    DIK_CAPITAL,
    DIK_COLON,
    DIK_COMMA,
    DIK_CONVERT,
    DIK_D,
    DIK_DECIMAL,
    DIK_DELETE,
    DIK_DIVIDE,
    DIK_DOWN,
    DIK_E,
    DIK_END,
    DIK_EQUALS,
    DIK_ESCAPE,
    DIK_F,
    DIK_F1,
    DIK_F2,
    DIK_F3,
    DIK_F4,
    DIK_F5,
    DIK_F6,
    DIK_F7,
    DIK_F8,
    DIK_F9,
    DIK_F10,
    DIK_F11,
    DIK_F12,
    DIK_F13,
    DIK_F14,
    DIK_F15,
    DIK_G,
    DIK_GRAVE,
    DIK_H,
    DIK_HOME,
    DIK_I,
    DIK_INSERT,
    DIK_J,
    DIK_K,
    DIK_KANA,
    DIK_KANJI,
    DIK_L,
    DIK_LALT,
    DIK_LBRACKET,
    DIK_LCONTROL,
    DIK_LEFT,
    DIK_LMENU,
    DIK_LSHIFT,
    DIK_LWIN,
    DIK_M,
    DIK_MAIL,
    DIK_MEDIASELECT,
    DIK_MEDIASTOP,
    DIK_MINUS,
    DIK_MULTIPLY,
    DIK_MUTE,
    DIK_MYCOMPUTER,
    DIK_N,
    DIK_NEXT,
    DIK_NEXTTRACK,
    DIK_NOCONVERT,
    DIK_NUMLOCK,
    DIK_NUMPAD0,
    DIK_NUMPAD1,
    DIK_NUMPAD2,
    DIK_NUMPAD3,
    DIK_NUMPAD4,
    DIK_NUMPAD5,
    DIK_NUMPAD6,
    DIK_NUMPAD7,
    DIK_NUMPAD8,
    DIK_NUMPAD9,
    DIK_NUMPADCOMMA,
    DIK_NUMPADENTER,
    DIK_NUMPADEQUALS,
    DIK_O,
    DIK_OEM_102,
    DIK_P,
    DIK_PAUSE,
    DIK_PERIOD,
    DIK_PLAYPAUSE,
    DIK_POWER,
    DIK_PREVTRACK,
    DIK_PRIOR,
    DIK_Q,
    DIK_R,
    DIK_RALT,
    DIK_RBRACKET,
    DIK_RCONTROL,
    DIK_RETURN,
    DIK_RIGHT,
    DIK_RMENU,
    DIK_RSHIFT,
    DIK_RWIN,
    DIK_S,
    DIK_SCROLL,
    DIK_SEMICOLON,
    DIK_SLASH,
    DIK_SLEEP,
    DIK_SPACE,
    DIK_STOP,
    DIK_SUBTRACT,
    DIK_SYSRQ,
    DIK_T,
    DIK_TAB,
    DIK_U,
    DIK_UNDERLINE,
    DIK_UNLABELED,
    DIK_UP,
    DIK_V,
    DIK_VOLUMEDOWN,
    DIK_VOLUMEUP,
    DIK_W,
    DIK_WAKE,
    DIK_WEBBACK,
    DIK_WEBFAVORITES,
    DIK_WEBFORWARD,
    DIK_WEBHOME,
    DIK_WEBREFRESH,
    DIK_WEBSEARCH,
    DIK_WEBSTOP,
    DIK_X,
    DIK_Y,
    DIK_YEN,
    DIK_Z
};// Keyboard Device, *LPKeyboard Device;

/*
Unknown type name 'HWND'
 Unknown type name 'HINSTANCE'
 Unknown type name 'HDC'
 Unknown type name 'RECT'
 Unknown type name 'MIDASmodule'
 Unknown type name 'MIDASmodulePlayHandle'
 Unknown type name 'ifstream'
 Use of undeclared identifier 'strcpy'
 Use of undeclared identifier 'strcmp'
 Unknown type name 'ofstream'
 */
#endif //PEKKAKANA2_OLD_HEADERS_H
