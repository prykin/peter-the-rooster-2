/* 
PisteEngine - PisteInput 2.0	
09.12.2001	Janne Kivilahti / Piste Gamez


Versio 2.0
----------

    Controls = reading from keyboard, mouse, or game controller with a single subroutine

    Added default controls (examples: PI_KB_A, PI_KB_UP, PI_MOUSE_LEFT_BUTTON, PI_GAMEPAD1_BUTTON_1)
    Allows users to choose their own controls. Also enhances code clarity.


unsigned char PisteInput_Read_Controller();
    Indicates which control the player has selected.

bool PisteInput_Read_Controller(unsigned char control); // Returns TRUE if the user has activated the control.
    Indicates whether the player has pressed a control (mouse button, gamepad button, or key).

char *PisteInput_Read_Controller_Name(unsigned char control);
    Returns the name of the control. For example, 'arrow left'.

/*

/* INCLUDES ----------------------------------------------------------------------------------*/

#include "input.h"
#include "log.h"

/* DEFINES -----------------------------------------------------------------------------------*/

#define DIKEYDOWN(data, n) (data[n] & 0x80)
#define MOUSE_LEFT_BUTTON 0
#define MOUSE_RIGHT_BUTTON 1

struct PELIOHJAIN {
    LPDIRECTINPUTDEVICE2 lpdijoy;
    bool available;
    char nimi[80];
    DIJOYSTATE joystick_state;
    GUID joystickGUID;
};

/* VARIABLES ---------------------------------------------------------------------------------*/

LPDIRECTINPUT PI_lpdi = NULL;
LPDIRECTINPUTDEVICE PI_lpdikey = NULL;
LPDIRECTINPUTDEVICE PI_lpdimouse = NULL;

HWND PI_main_window_handle = NULL; // globally track main window
HINSTANCE PI_hinstance_app = NULL; // globally track hinstance
HDC PI_global_dc = NULL; // tracks movement_x global dc

unsigned char PI_keyboard_state[256];
DIMOUSESTATE PI_mouse_state;
bool PI_mouse_available = true;
bool PI_mouse_button;

PELIOHJAIN PI_joysticks[PI_MAX_CONTROLLERS];
int PI_joystick_index = 0;

bool PI_unload = true;

/* METHODS -----------------------------------------------------------------------------------*/

BOOL CALLBACK PisteInput_Enum_Controllers(LPCDIDEVICEINSTANCE lpddi, LPVOID guid_ptr) {
    //*(GUID*)guid_ptr = lpddi->guidInstance;
    PI_joysticks[PI_joystick_index].joystickGUID = lpddi->guidInstance;
    strcpy(PI_joysticks[PI_joystick_index].nimi, (char *)lpddi->tszProductName);
    PI_joystick_index++;
    if (PI_joystick_index < PI_MAX_CONTROLLERS)
        return(DIENUM_CONTINUE);
    return(DIENUM_STOP);
}

bool PisteInput_Initialize_Controller(int index) {
    LPDIRECTINPUTDEVICE temp;

    if (FAILED(PI_lpdi->CreateDevice(PI_joysticks[index].joystickGUID, &temp, NULL))) {
        PisteLog_Write("[Error] Piste Input: Gamepad - Create device failed! \n");
        return false;
    }

    if (FAILED(temp->QueryInterface(IID_IDirectInputDevice2, (void **) &PI_joysticks[index].lpdijoy))) {
        PisteLog_Write("[Error] Piste Input: Gamepad - Create device failed! \n");
        return false;
    }

    if (FAILED(temp->Release())) {
        PisteLog_Write("[Error] Piste Input: Gamepad - Releasing DirectInputDevice 1 failed! \n");
        return false;
    }

    if (FAILED(PI_joysticks[index].lpdijoy->SetCooperativeLevel(PI_main_window_handle,
                                                                DISCL_BACKGROUND | DISCL_NONEXCLUSIVE))) {
        PisteLog_Write("[Error] Piste Input: Gamepad - Releasing DirectInputDevice 1 failed! \n");
        return false;
    }

    if (FAILED(PI_joysticks[index].lpdijoy->SetDataFormat(&c_dfDIJoystick))) {
        PisteLog_Write("[Error] Piste Input: Gamepad - Set dataformat failed! \n");
        return false;
    }

    DIPROPRANGE joy_axis_range;

    //Defining the x-axis

    joy_axis_range.lMin = -PI_CONTROLLER_XY_MAX;
    joy_axis_range.lMax = PI_CONTROLLER_XY_MAX;
    joy_axis_range.diph.dwSize = sizeof(DIPROPRANGE);
    joy_axis_range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    joy_axis_range.diph.dwObj = DIJOFS_X;
    joy_axis_range.diph.dwHow = DIPH_BYOFFSET;
    PI_joysticks[index].lpdijoy->SetProperty(DIPROP_RANGE, &joy_axis_range.diph);

    //Defining the y-axis

    joy_axis_range.lMin = -PI_CONTROLLER_XY_MAX;
    joy_axis_range.lMax = PI_CONTROLLER_XY_MAX;
    joy_axis_range.diph.dwSize = sizeof(DIPROPRANGE);
    joy_axis_range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    joy_axis_range.diph.dwObj = DIJOFS_Y;
    joy_axis_range.diph.dwHow = DIPH_BYOFFSET;
    PI_joysticks[index].lpdijoy->SetProperty(DIPROP_RANGE, &joy_axis_range.diph);

    DIPROPDWORD dead_angle;

    dead_angle.diph.dwSize = sizeof(dead_angle);
    dead_angle.diph.dwHeaderSize = sizeof(dead_angle.diph);
    dead_angle.diph.dwObj = DIJOFS_X;
    dead_angle.diph.dwHow = DIPH_BYOFFSET;
    dead_angle.dwData = 5000; //= 10%
    PI_joysticks[index].lpdijoy->SetProperty(DIPROP_DEADZONE, &dead_angle.diph);

    dead_angle.diph.dwSize = sizeof(dead_angle);
    dead_angle.diph.dwHeaderSize = sizeof(dead_angle.diph);
    dead_angle.diph.dwObj = DIJOFS_Y;
    dead_angle.diph.dwHow = DIPH_BYOFFSET;
    dead_angle.dwData = 5000; //= 10%
    PI_joysticks[index].lpdijoy->SetProperty(DIPROP_DEADZONE, &dead_angle.diph);

    if (FAILED(PI_joysticks[index].lpdijoy->Acquire())) {
        PisteLog_Write("[Error] Piste Input: Gamepad - Acquiring device failed! \n");
        return false;
    }

    return true;
}

bool PisteInput_Initialize_Controllers() {
    //LPDIRECTINPUTDEVICE temp;

    if (FAILED(PI_lpdi->EnumDevices(DIDEVTYPE_JOYSTICK, PisteInput_Enum_Controllers, /*&PI_joystickGUID*/NULL,
                                    DIEDFL_ATTACHEDONLY))) {
        PisteLog_Write("[Error] Piste Input: Gamepads - Enumerating failed! \n");
        return false;
    }

    for (int i = 0; i < PI_MAX_CONTROLLERS; i++) {
        if (PI_joysticks[i].nimi != NULL) {
            if (PisteInput_Initialize_Controller(i)) {
                PI_joysticks[i].available = true;
            } else {
                PI_joysticks[i].available = false;
            }
        } else
            PI_joysticks[i].available = false;
    }

    /*
    if (FAILED(PI_lpdi->CreateDevice(PI_joystickGUID, &temp, NULL)))
        return false;

    if (FAILED(temp->QueryInterface(IID_IDirectInputDevice2,(void**) &PI_lpdijoy)))
        return false;

    if (FAILED(temp->Release()))
        return false;

    if (FAILED(PI_lpdijoy->SetCooperativeLevel(PI_main_window_handle, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE	)))
        return false;

    if (FAILED(PI_lpdijoy->SetDataFormat(&c_dfDIJoystick)))
        return false;

    DIPROPRANGE joy_axis_range;

    //Defining the x-axis

    joy_axis_range.lMin = -PI_CONTROLLER_XY_MAX;
    joy_axis_range.lMax =  PI_CONTROLLER_XY_MAX;
    joy_axis_range.diph.dwSize			= sizeof(DIPROPRANGE);
    joy_axis_range.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
    joy_axis_range.diph.dwObj = DIJOFS_X;
    joy_axis_range.diph.dwHow = DIPH_BYOFFSET;
    PI_lpdijoy->SetProperty(DIPROP_RANGE, &joy_axis_range.diph);

    //Defining the y-axis

    joy_axis_range.lMin = -PI_CONTROLLER_XY_MAX;
    joy_axis_range.lMax =  PI_CONTROLLER_XY_MAX;
    joy_axis_range.diph.dwSize			= sizeof(DIPROPRANGE);
    joy_axis_range.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
    joy_axis_range.diph.dwObj = DIJOFS_Y;
    joy_axis_range.diph.dwHow = DIPH_BYOFFSET;
    PI_lpdijoy->SetProperty(DIPROP_RANGE, &joy_axis_range.diph);

    DIPROPDWORD dead_angle;

    dead_angle.diph.dwSize			= sizeof(dead_angle);
    dead_angle.diph.dwHeaderSize	= sizeof(dead_angle.diph);
    dead_angle.diph.dwObj			= DIJOFS_X;
    dead_angle.diph.dwHow			= DIPH_BYOFFSET;
    dead_angle.dwData				= 5000; //= 10%
    PI_lpdijoy->SetProperty(DIPROP_DEADZONE, &dead_angle.diph);

    dead_angle.diph.dwSize			= sizeof(dead_angle);
    dead_angle.diph.dwHeaderSize	= sizeof(dead_angle.diph);
    dead_angle.diph.dwObj			= DIJOFS_Y;
    dead_angle.diph.dwHow			= DIPH_BYOFFSET;
    dead_angle.dwData				= 5000; //= 10%
    PI_lpdijoy->SetProperty(DIPROP_DEADZONE, &dead_angle.diph);

    if (FAILED(PI_lpdijoy->Acquire()))
        return false;
    */
    return true;
}

bool PisteInput_Initialize_Keyboard() {

    if (FAILED(PI_lpdi->CreateDevice(GUID_SysKeyboard, &PI_lpdikey, NULL))) {
        PisteLog_Write("[Error] Piste Input: Keyboard - Create Device failed! \n");
        return false;
    }

    if (FAILED(PI_lpdikey->SetCooperativeLevel(PI_main_window_handle,
                                               DISCL_BACKGROUND | DISCL_NONEXCLUSIVE /* | DISCL_FOREGROUND*/))) {
        PisteLog_Write("[Error] Piste Input: Keyboard - Set cooperative level failed! \n");
        return false;
    }

    if (FAILED(PI_lpdikey->SetDataFormat(&c_dfDIKeyboard))) {
        PisteLog_Write("[Error] Piste Input: Keyboard - Set data format failed! \n");
        return false;
    }

    if (FAILED(PI_lpdikey->Acquire())) {
        PisteLog_Write("[Error] Piste Input: Keyboard - Acquire failed! \n");
        return false;
    }

    return true;
}

bool PisteInput_Initialize_Mouse() {

    if (FAILED(PI_lpdi->CreateDevice(GUID_SysMouse, &PI_lpdimouse, NULL))) {
        PisteLog_Write("[Warning] Piste Input: No mouse available! \n");
        PI_mouse_available = false;
    }

    if (PI_mouse_available) {
        if (FAILED(PI_lpdimouse->SetCooperativeLevel(PI_main_window_handle, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE))) {
            PisteLog_Write("[Error] Piste Input: Mouse - Set cooperative level failed! \n");
            PI_mouse_available = false;
        }

        if (FAILED(PI_lpdimouse->SetDataFormat(&c_dfDIMouse))) {
            PisteLog_Write("[Error] Piste Input: Mouse - Set data format failed! \n");
            PI_mouse_available = false;
        }

        if (FAILED(PI_lpdimouse->Acquire())) {
            PisteLog_Write("[Error] Piste Input: Mouse - Acquire failed! \n");
            PI_mouse_available = false;
        }
    }

    return PI_mouse_available;
}

int PisteInput_Init(HWND &main_window_handle, HINSTANCE &hinstance_app) {
    if (PI_unload) {

        PI_main_window_handle = (HWND &) main_window_handle;
        PI_hinstance_app = (HINSTANCE &) hinstance_app;

        if (FAILED(DirectInputCreate(PI_hinstance_app, DIRECTINPUT_VERSION, &PI_lpdi, NULL))) {
            PisteLog_Write("[Error] Piste Input: DirectInputCreate failed! \n");
            return PI_ERROR;
        }

        /* Keyboard setup */

        if (!PisteInput_Initialize_Keyboard())
            return PI_ERROR;

        /* Mouse setup */

        if (!PisteInput_Initialize_Mouse())
            PI_mouse_available = false;
        //return PI_ERROR;

        /* Controller setup */

        PisteInput_Initialize_Controllers();

        PI_unload = false;
    }

    return 0;
}

bool PisteInput_IsMouseDetected() {
    return PI_mouse_available;
}

bool PisteInput_IsControllerDetected(int controller_index) {
    return PI_joysticks[controller_index].available;
}

bool PisteInput_KeyDown(int key) {
    if (DIKEYDOWN(PI_keyboard_state, key))
        return true;

    return false;
}

bool PisteInput_Mouse_Left() {
    PI_mouse_button = false;

    if (PI_mouse_available) {
        if (PI_mouse_state.rgbButtons[MOUSE_LEFT_BUTTON] & 0x80)
            PI_mouse_button = true;
    }
    return PI_mouse_button;
}

bool PisteInput_Mouse_Right() {
    PI_mouse_button = false;

    if (PI_mouse_available) {
        if (PI_mouse_state.rgbButtons[MOUSE_RIGHT_BUTTON] & 0x80)
            PI_mouse_button = true;
    }

    return PI_mouse_button;
}

int PisteInput_Mouse_X(int x) {
    if (PI_mouse_available)
        x += PI_mouse_state.lX;

    return x;
}

int PisteInput_Mouse_Y(int y) {
    if (PI_mouse_available)
        y += PI_mouse_state.lY;

    return y;
}

int PisteInput_Controller_MoveX(int controller_index) {
    int x = 0;

    if (PI_joysticks[controller_index].available)
        x = PI_joysticks[controller_index].joystick_state.lX;

    return x;
}

int PisteInput_Controller_MoveY(int controller_index) {
    int y = 0;

    if (PI_joysticks[controller_index].available)
        y += PI_joysticks[controller_index].joystick_state.lY;

    return y;
}

bool PisteInput_Controller_Button(int controller, int index) {
    bool painettu = false;

    if (PI_joysticks[controller].available) {
        if (PI_joysticks[controller].joystick_state.rgbButtons[index] & 0x80)
            painettu = true;
    }

    return painettu;
}

char *PisteInput_GameController_Name(int controller_index) {
    return PI_joysticks[controller_index].nimi;
}

bool PisteInput_Update_Controllers() {
    bool ok = false;

    for (int ohjain = 0; ohjain < PI_MAX_CONTROLLERS; ohjain++) {
        if (PI_joysticks[ohjain].available) {
            if (FAILED(PI_joysticks[ohjain].lpdijoy->Poll())) {
                PisteLog_Write("[Warning] Piste Input: Lost control of game pad! \n");
                PI_joysticks[ohjain].available = false;
            }

            if (FAILED(PI_joysticks[ohjain].lpdijoy->GetDeviceState(sizeof(DIJOYSTATE),
                                                                    (LPVOID) & PI_joysticks[ohjain].joystick_state))) {
                PisteLog_Write("[Warning] Piste Input: Lost control of game pad! \n");
                PI_joysticks[ohjain].available = false;
            }
            if (PI_joysticks[ohjain].available)
                ok = true;
        }
    }
    return ok;
}


bool PisteInput_Update_Keyboard() {
    HRESULT result;
    bool ok = true;

    while (result = PI_lpdikey->GetDeviceState(sizeof(PI_keyboard_state), (LPVOID) PI_keyboard_state) ==
                    DIERR_INPUTLOST) {
        if (FAILED(result = PI_lpdikey->Acquire())) {
            PisteLog_Write("[Warning] Piste Input: Lost control of keyboard! \n");
            ok = false;
        }
    }

    return ok;
}

bool PisteInput_Update_Mouse() {
    if (PI_mouse_available) {
        if (FAILED(PI_lpdimouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID) & PI_mouse_state))) {
            PisteLog_Write("[Warning] Piste Input: Lost control of mouse! \n");
            PI_mouse_available = false;
        }
    }
    return PI_mouse_available;
}

char PisteInput_Read_Keyboard(void) {
    if (DIKEYDOWN(PI_keyboard_state, DIK_RSHIFT)) {
        if (DIKEYDOWN(PI_keyboard_state, DIK_A)) return ('�');
        if (DIKEYDOWN(PI_keyboard_state, DIK_1)) return ('!');
        if (DIKEYDOWN(PI_keyboard_state, DIK_ADD)) return ('?');
        if (DIKEYDOWN(PI_keyboard_state, DIK_O)) return ('�');
    }

    if (DIKEYDOWN(PI_keyboard_state, DIK_RMENU)) {
        if (DIKEYDOWN(PI_keyboard_state, DIK_O)) return ('�');
        if (DIKEYDOWN(PI_keyboard_state, DIK_A)) return ('�');
    }

    if (DIKEYDOWN(PI_keyboard_state, DIK_A)) return ('a');
    if (DIKEYDOWN(PI_keyboard_state, DIK_B)) return ('b');
    if (DIKEYDOWN(PI_keyboard_state, DIK_C)) return ('c');
    if (DIKEYDOWN(PI_keyboard_state, DIK_D)) return ('d');
    if (DIKEYDOWN(PI_keyboard_state, DIK_E)) return ('e');
    if (DIKEYDOWN(PI_keyboard_state, DIK_F)) return ('f');
    if (DIKEYDOWN(PI_keyboard_state, DIK_G)) return ('g');
    if (DIKEYDOWN(PI_keyboard_state, DIK_H)) return ('h');
    if (DIKEYDOWN(PI_keyboard_state, DIK_I)) return ('i');
    if (DIKEYDOWN(PI_keyboard_state, DIK_J)) return ('j');
    if (DIKEYDOWN(PI_keyboard_state, DIK_K)) return ('k');
    if (DIKEYDOWN(PI_keyboard_state, DIK_L)) return ('l');
    if (DIKEYDOWN(PI_keyboard_state, DIK_M)) return ('m');
    if (DIKEYDOWN(PI_keyboard_state, DIK_N)) return ('n');
    if (DIKEYDOWN(PI_keyboard_state, DIK_O)) return ('o');
    if (DIKEYDOWN(PI_keyboard_state, DIK_P)) return ('p');
    if (DIKEYDOWN(PI_keyboard_state, DIK_Q)) return ('q');
    if (DIKEYDOWN(PI_keyboard_state, DIK_R)) return ('r');
    if (DIKEYDOWN(PI_keyboard_state, DIK_S)) return ('s');
    if (DIKEYDOWN(PI_keyboard_state, DIK_T)) return ('t');
    if (DIKEYDOWN(PI_keyboard_state, DIK_U)) return ('u');
    if (DIKEYDOWN(PI_keyboard_state, DIK_V)) return ('v');
    if (DIKEYDOWN(PI_keyboard_state, DIK_W)) return ('w');
    if (DIKEYDOWN(PI_keyboard_state, DIK_X)) return ('x');
    if (DIKEYDOWN(PI_keyboard_state, DIK_Y)) return ('y');
    if (DIKEYDOWN(PI_keyboard_state, DIK_Z)) return ('z');
    if (DIKEYDOWN(PI_keyboard_state, DIK_0)) return ('0');
    if (DIKEYDOWN(PI_keyboard_state, DIK_1)) return ('1');
    if (DIKEYDOWN(PI_keyboard_state, DIK_2)) return ('2');
    if (DIKEYDOWN(PI_keyboard_state, DIK_3)) return ('3');
    if (DIKEYDOWN(PI_keyboard_state, DIK_4)) return ('4');
    if (DIKEYDOWN(PI_keyboard_state, DIK_5)) return ('5');
    if (DIKEYDOWN(PI_keyboard_state, DIK_6)) return ('6');
    if (DIKEYDOWN(PI_keyboard_state, DIK_7)) return ('7');
    if (DIKEYDOWN(PI_keyboard_state, DIK_8)) return ('8');
    if (DIKEYDOWN(PI_keyboard_state, DIK_9)) return ('9');
    if (DIKEYDOWN(PI_keyboard_state, DIK_SPACE)) return (' ');
    if (DIKEYDOWN(PI_keyboard_state, DIK_PERIOD)) return ('.');
    if (DIKEYDOWN(PI_keyboard_state, DIK_COMMA)) return (',');
    if (DIKEYDOWN(PI_keyboard_state, DIK_MINUS)) return ('-');
    if (DIKEYDOWN(PI_keyboard_state, DIK_ADD)) return ('+');
    if (DIKEYDOWN(PI_keyboard_state, DIK_SEMICOLON)) return (':');
    if (DIKEYDOWN(PI_keyboard_state, DIK_RETURN)) return ('\n');

    return ('\0');
}

unsigned char PisteInput_Read_Controller() {
    if (DIKEYDOWN(PI_keyboard_state, DIK_A)) return PI_KB_A;
    if (DIKEYDOWN(PI_keyboard_state, DIK_B)) return PI_KB_B;
    if (DIKEYDOWN(PI_keyboard_state, DIK_C)) return PI_KB_C;
    if (DIKEYDOWN(PI_keyboard_state, DIK_D)) return PI_KB_D;
    if (DIKEYDOWN(PI_keyboard_state, DIK_E)) return PI_KB_E;
    if (DIKEYDOWN(PI_keyboard_state, DIK_F)) return PI_KB_F;
    if (DIKEYDOWN(PI_keyboard_state, DIK_G)) return PI_KB_G;
    if (DIKEYDOWN(PI_keyboard_state, DIK_H)) return PI_KB_H;
    if (DIKEYDOWN(PI_keyboard_state, DIK_I)) return PI_KB_I;
    if (DIKEYDOWN(PI_keyboard_state, DIK_J)) return PI_KB_J;
    if (DIKEYDOWN(PI_keyboard_state, DIK_K)) return PI_KB_K;
    if (DIKEYDOWN(PI_keyboard_state, DIK_L)) return PI_KB_L;
    if (DIKEYDOWN(PI_keyboard_state, DIK_M)) return PI_KB_M;
    if (DIKEYDOWN(PI_keyboard_state, DIK_N)) return PI_KB_N;
    if (DIKEYDOWN(PI_keyboard_state, DIK_O)) return PI_KB_O;
    if (DIKEYDOWN(PI_keyboard_state, DIK_P)) return PI_KB_P;
    if (DIKEYDOWN(PI_keyboard_state, DIK_Q)) return PI_KB_Q;
    if (DIKEYDOWN(PI_keyboard_state, DIK_R)) return PI_KB_R;
    if (DIKEYDOWN(PI_keyboard_state, DIK_S)) return PI_KB_S;
    if (DIKEYDOWN(PI_keyboard_state, DIK_T)) return PI_KB_T;
    if (DIKEYDOWN(PI_keyboard_state, DIK_U)) return PI_KB_U;
    if (DIKEYDOWN(PI_keyboard_state, DIK_V)) return PI_KB_V;
    if (DIKEYDOWN(PI_keyboard_state, DIK_W)) return PI_KB_W;
    if (DIKEYDOWN(PI_keyboard_state, DIK_X)) return PI_KB_X;
    if (DIKEYDOWN(PI_keyboard_state, DIK_Y)) return PI_KB_Y;
    if (DIKEYDOWN(PI_keyboard_state, DIK_Z)) return PI_KB_Z;
    if (DIKEYDOWN(PI_keyboard_state, DIK_0)) return PI_KB_0;
    if (DIKEYDOWN(PI_keyboard_state, DIK_1)) return PI_KB_1;
    if (DIKEYDOWN(PI_keyboard_state, DIK_2)) return PI_KB_2;
    if (DIKEYDOWN(PI_keyboard_state, DIK_3)) return PI_KB_3;
    if (DIKEYDOWN(PI_keyboard_state, DIK_4)) return PI_KB_4;
    if (DIKEYDOWN(PI_keyboard_state, DIK_5)) return PI_KB_5;
    if (DIKEYDOWN(PI_keyboard_state, DIK_6)) return PI_KB_6;
    if (DIKEYDOWN(PI_keyboard_state, DIK_7)) return PI_KB_7;
    if (DIKEYDOWN(PI_keyboard_state, DIK_8)) return PI_KB_8;
    if (DIKEYDOWN(PI_keyboard_state, DIK_9)) return PI_KB_9;

    if (DIKEYDOWN(PI_keyboard_state, DIK_RETURN)) return PI_KB_ENTER;
    if (DIKEYDOWN(PI_keyboard_state, DIK_SPACE)) return PI_KB_SPACE;
    if (DIKEYDOWN(PI_keyboard_state, DIK_RSHIFT)) return PI_KB_RSHIFT;
    if (DIKEYDOWN(PI_keyboard_state, DIK_LSHIFT)) return PI_KB_LSHIFT;
    if (DIKEYDOWN(PI_keyboard_state, DIK_RALT)) return PI_KB_RALT;
    if (DIKEYDOWN(PI_keyboard_state, DIK_LALT)) return PI_KB_LALT;
    if (DIKEYDOWN(PI_keyboard_state, DIK_RCONTROL)) return PI_KB_RCONTROL;
    if (DIKEYDOWN(PI_keyboard_state, DIK_LCONTROL)) return PI_KB_LCONTROL;
    if (DIKEYDOWN(PI_keyboard_state, DIK_PRIOR)) return PI_KB_PAGEUP;
    if (DIKEYDOWN(PI_keyboard_state, DIK_NEXT)) return PI_KB_PAGEDOWN;
    if (DIKEYDOWN(PI_keyboard_state, DIK_BACKSPACE)) return PI_KB_BACKSPACE;
    if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPADENTER)) return PI_KB_NUMPADENTER;

    if (DIKEYDOWN(PI_keyboard_state, DIK_UP)) return PI_KB_UP;
    if (DIKEYDOWN(PI_keyboard_state, DIK_DOWN)) return PI_KB_DOWN;
    if (DIKEYDOWN(PI_keyboard_state, DIK_LEFT)) return PI_KB_LEFT;
    if (DIKEYDOWN(PI_keyboard_state, DIK_RIGHT)) return PI_KB_RIGHT;

    if (DIKEYDOWN(PI_keyboard_state, DIK_F1)) return PI_KB_F1;
    if (DIKEYDOWN(PI_keyboard_state, DIK_F2)) return PI_KB_F2;
    if (DIKEYDOWN(PI_keyboard_state, DIK_F3)) return PI_KB_F3;
    if (DIKEYDOWN(PI_keyboard_state, DIK_F4)) return PI_KB_F4;
    if (DIKEYDOWN(PI_keyboard_state, DIK_F5)) return PI_KB_F5;
    if (DIKEYDOWN(PI_keyboard_state, DIK_F6)) return PI_KB_F6;
    if (DIKEYDOWN(PI_keyboard_state, DIK_F7)) return PI_KB_F7;
    if (DIKEYDOWN(PI_keyboard_state, DIK_F8)) return PI_KB_F8;
    if (DIKEYDOWN(PI_keyboard_state, DIK_F9)) return PI_KB_F9;
    if (DIKEYDOWN(PI_keyboard_state, DIK_F10)) return PI_KB_F10;

    if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD0)) return PI_KB_NUMPAD_0;
    if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD1)) return PI_KB_NUMPAD_1;
    if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD2)) return PI_KB_NUMPAD_2;
    if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD3)) return PI_KB_NUMPAD_3;
    if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD4)) return PI_KB_NUMPAD_4;
    if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD5)) return PI_KB_NUMPAD_5;
    if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD6)) return PI_KB_NUMPAD_6;
    if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD7)) return PI_KB_NUMPAD_7;
    if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD8)) return PI_KB_NUMPAD_8;
    if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD9)) return PI_KB_NUMPAD_9;

    if (PisteInput_Mouse_Left()) return PI_MOUSE_LEFT_BUTTON;
    if (PisteInput_Mouse_Right()) return PI_MOUSE_RIGHT_BUTTON;

    if (PisteInput_Controller_MoveX(PI_CONTROLLER_1) < 0) return PI_CONTROLLER_1_MOVE_LEFT;
    if (PisteInput_Controller_MoveX(PI_CONTROLLER_1) > 0) return PI_CONTROLLER_1_MOVE_RIGHT;
    if (PisteInput_Controller_MoveY(PI_CONTROLLER_1) < 0) return PI_CONTROLLER_1_MOVE_UP;
    if (PisteInput_Controller_MoveY(PI_CONTROLLER_1) > 0) return PI_CONTROLLER_1_MOVE_DOWN;
    if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_1)) return PI_CONTROLLER_1_BUTTON_1;
    if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_2)) return PI_CONTROLLER_1_BUTTON_2;
    if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_3)) return PI_CONTROLLER_1_BUTTON_3;
    if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_4)) return PI_CONTROLLER_1_BUTTON_4;
    if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_5)) return PI_CONTROLLER_1_BUTTON_5;
    if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_6)) return PI_CONTROLLER_1_BUTTON_6;

    if (PisteInput_Controller_MoveX(PI_CONTROLLER_2) < 0) return PI_CONTROLLER_2_MOVE_LEFT;
    if (PisteInput_Controller_MoveX(PI_CONTROLLER_2) > 0) return PI_CONTROLLER_2_MOVE_RIGHT;
    if (PisteInput_Controller_MoveY(PI_CONTROLLER_2) < 0) return PI_CONTROLLER_2_MOVE_UP;
    if (PisteInput_Controller_MoveY(PI_CONTROLLER_2) > 0) return PI_CONTROLLER_2_MOVE_DOWN;
    if (PisteInput_Controller_Button(PI_CONTROLLER_2, PI_CONTROLLER_BUTTON_1)) return PI_CONTROLLER_2_BUTTON_1;
    if (PisteInput_Controller_Button(PI_CONTROLLER_2, PI_CONTROLLER_BUTTON_2)) return PI_CONTROLLER_2_BUTTON_2;
    if (PisteInput_Controller_Button(PI_CONTROLLER_2, PI_CONTROLLER_BUTTON_3)) return PI_CONTROLLER_2_BUTTON_3;
    if (PisteInput_Controller_Button(PI_CONTROLLER_2, PI_CONTROLLER_BUTTON_4)) return PI_CONTROLLER_2_BUTTON_4;
    if (PisteInput_Controller_Button(PI_CONTROLLER_2, PI_CONTROLLER_BUTTON_5)) return PI_CONTROLLER_2_BUTTON_5;
    if (PisteInput_Controller_Button(PI_CONTROLLER_2, PI_CONTROLLER_BUTTON_6)) return PI_CONTROLLER_2_BUTTON_6;

    return 0;
}

bool PisteInput_Read_Controller(unsigned char controller) {
    switch (controller) {
        case PI_KB_A:
            if (DIKEYDOWN(PI_keyboard_state, DIK_A)) return true;
            break;
        case PI_KB_B:
            if (DIKEYDOWN(PI_keyboard_state, DIK_B)) return true;
            break;
        case PI_KB_C:
            if (DIKEYDOWN(PI_keyboard_state, DIK_C)) return true;
            break;
        case PI_KB_D:
            if (DIKEYDOWN(PI_keyboard_state, DIK_D)) return true;
            break;
        case PI_KB_E:
            if (DIKEYDOWN(PI_keyboard_state, DIK_E)) return true;
            break;
        case PI_KB_F:
            if (DIKEYDOWN(PI_keyboard_state, DIK_F)) return true;
            break;
        case PI_KB_G:
            if (DIKEYDOWN(PI_keyboard_state, DIK_G)) return true;
            break;
        case PI_KB_H:
            if (DIKEYDOWN(PI_keyboard_state, DIK_H)) return true;
            break;
        case PI_KB_I:
            if (DIKEYDOWN(PI_keyboard_state, DIK_I)) return true;
            break;
        case PI_KB_J:
            if (DIKEYDOWN(PI_keyboard_state, DIK_J)) return true;
            break;
        case PI_KB_K:
            if (DIKEYDOWN(PI_keyboard_state, DIK_K)) return true;
            break;
        case PI_KB_L:
            if (DIKEYDOWN(PI_keyboard_state, DIK_L)) return true;
            break;
        case PI_KB_M:
            if (DIKEYDOWN(PI_keyboard_state, DIK_M)) return true;
            break;
        case PI_KB_N:
            if (DIKEYDOWN(PI_keyboard_state, DIK_N)) return true;
            break;
        case PI_KB_O:
            if (DIKEYDOWN(PI_keyboard_state, DIK_O)) return true;
            break;
        case PI_KB_P:
            if (DIKEYDOWN(PI_keyboard_state, DIK_P)) return true;
            break;
        case PI_KB_Q:
            if (DIKEYDOWN(PI_keyboard_state, DIK_Q)) return true;
            break;
        case PI_KB_R:
            if (DIKEYDOWN(PI_keyboard_state, DIK_R)) return true;
            break;
        case PI_KB_S:
            if (DIKEYDOWN(PI_keyboard_state, DIK_S)) return true;
            break;
        case PI_KB_T:
            if (DIKEYDOWN(PI_keyboard_state, DIK_T)) return true;
            break;
        case PI_KB_U:
            if (DIKEYDOWN(PI_keyboard_state, DIK_U)) return true;
            break;
        case PI_KB_V:
            if (DIKEYDOWN(PI_keyboard_state, DIK_V)) return true;
            break;
        case PI_KB_W:
            if (DIKEYDOWN(PI_keyboard_state, DIK_W)) return true;
            break;
        case PI_KB_X:
            if (DIKEYDOWN(PI_keyboard_state, DIK_X)) return true;
            break;
        case PI_KB_Y:
            if (DIKEYDOWN(PI_keyboard_state, DIK_Y)) return true;
            break;
        case PI_KB_Z:
            if (DIKEYDOWN(PI_keyboard_state, DIK_Z)) return true;
            break;
        case PI_KB_0:
            if (DIKEYDOWN(PI_keyboard_state, DIK_0)) return true;
            break;
        case PI_KB_1:
            if (DIKEYDOWN(PI_keyboard_state, DIK_1)) return true;
            break;
        case PI_KB_2:
            if (DIKEYDOWN(PI_keyboard_state, DIK_2)) return true;
            break;
        case PI_KB_3:
            if (DIKEYDOWN(PI_keyboard_state, DIK_3)) return true;
            break;
        case PI_KB_4:
            if (DIKEYDOWN(PI_keyboard_state, DIK_4)) return true;
            break;
        case PI_KB_5:
            if (DIKEYDOWN(PI_keyboard_state, DIK_5)) return true;
            break;
        case PI_KB_6:
            if (DIKEYDOWN(PI_keyboard_state, DIK_6)) return true;
            break;
        case PI_KB_7:
            if (DIKEYDOWN(PI_keyboard_state, DIK_7)) return true;
            break;
        case PI_KB_8:
            if (DIKEYDOWN(PI_keyboard_state, DIK_8)) return true;
            break;
        case PI_KB_9:
            if (DIKEYDOWN(PI_keyboard_state, DIK_9)) return true;
            break;
        case PI_KB_ENTER:
            if (DIKEYDOWN(PI_keyboard_state, DIK_RETURN)) return true;
            break;
        case PI_KB_SPACE:
            if (DIKEYDOWN(PI_keyboard_state, DIK_SPACE)) return true;
            break;
        case PI_KB_RSHIFT:
            if (DIKEYDOWN(PI_keyboard_state, DIK_RSHIFT)) return true;
            break;
        case PI_KB_LSHIFT:
            if (DIKEYDOWN(PI_keyboard_state, DIK_LSHIFT)) return true;
            break;
        case PI_KB_RALT:
            if (DIKEYDOWN(PI_keyboard_state, DIK_RALT)) return true;
            break;
        case PI_KB_LALT:
            if (DIKEYDOWN(PI_keyboard_state, DIK_LALT)) return true;
            break;
        case PI_KB_RCONTROL:
            if (DIKEYDOWN(PI_keyboard_state, DIK_RCONTROL)) return true;
            break;
        case PI_KB_LCONTROL:
            if (DIKEYDOWN(PI_keyboard_state, DIK_LCONTROL)) return true;
            break;
        case PI_KB_PAGEUP:
            if (DIKEYDOWN(PI_keyboard_state, DIK_PRIOR)) return true;
            break;
        case PI_KB_PAGEDOWN:
            if (DIKEYDOWN(PI_keyboard_state, DIK_NEXT)) return true;
            break;
        case PI_KB_BACKSPACE:
            if (DIKEYDOWN(PI_keyboard_state, DIK_BACKSPACE)) return true;
            break;
        case PI_KB_NUMPADENTER:
            if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPADENTER)) return true;
            break;

        case PI_KB_UP:
            if (DIKEYDOWN(PI_keyboard_state, DIK_UP)) return true;
            break;
        case PI_KB_DOWN:
            if (DIKEYDOWN(PI_keyboard_state, DIK_DOWN)) return true;
            break;
        case PI_KB_LEFT:
            if (DIKEYDOWN(PI_keyboard_state, DIK_LEFT)) return true;
            break;
        case PI_KB_RIGHT:
            if (DIKEYDOWN(PI_keyboard_state, DIK_RIGHT)) return true;
            break;

        case PI_KB_F1:
            if (DIKEYDOWN(PI_keyboard_state, DIK_F1)) return true;
            break;
        case PI_KB_F2:
            if (DIKEYDOWN(PI_keyboard_state, DIK_F2)) return true;
            break;
        case PI_KB_F3:
            if (DIKEYDOWN(PI_keyboard_state, DIK_F3)) return true;
            break;
        case PI_KB_F4:
            if (DIKEYDOWN(PI_keyboard_state, DIK_F4)) return true;
            break;
        case PI_KB_F5:
            if (DIKEYDOWN(PI_keyboard_state, DIK_F5)) return true;
            break;
        case PI_KB_F6:
            if (DIKEYDOWN(PI_keyboard_state, DIK_F6)) return true;
            break;
        case PI_KB_F7:
            if (DIKEYDOWN(PI_keyboard_state, DIK_F7)) return true;
            break;
        case PI_KB_F8:
            if (DIKEYDOWN(PI_keyboard_state, DIK_F8)) return true;
            break;
        case PI_KB_F9:
            if (DIKEYDOWN(PI_keyboard_state, DIK_F9)) return true;
            break;
        case PI_KB_F10:
            if (DIKEYDOWN(PI_keyboard_state, DIK_F10)) return true;
            break;

        case PI_KB_NUMPAD_0:
            if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD0)) return true;
            break;
        case PI_KB_NUMPAD_1:
            if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD1)) return true;
            break;
        case PI_KB_NUMPAD_2:
            if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD2)) return true;
            break;
        case PI_KB_NUMPAD_3:
            if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD3)) return true;
            break;
        case PI_KB_NUMPAD_4:
            if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD4)) return true;
            break;
        case PI_KB_NUMPAD_5:
            if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD5)) return true;
            break;
        case PI_KB_NUMPAD_6:
            if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD6)) return true;
            break;
        case PI_KB_NUMPAD_7:
            if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD7)) return true;
            break;
        case PI_KB_NUMPAD_8:
            if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD8)) return true;
            break;
        case PI_KB_NUMPAD_9:
            if (DIKEYDOWN(PI_keyboard_state, DIK_NUMPAD9)) return true;
            break;

        case PI_MOUSE_LEFT_BUTTON:
            if (PisteInput_Mouse_Left()) return true;
            break;
        case PI_MOUSE_RIGHT_BUTTON:
            if (PisteInput_Mouse_Right()) return true;
            break;

        case PI_CONTROLLER_1_MOVE_LEFT:
            if (PisteInput_Controller_MoveX(PI_CONTROLLER_1) < 0) return true;
            break;
        case PI_CONTROLLER_1_MOVE_RIGHT:
            if (PisteInput_Controller_MoveX(PI_CONTROLLER_1) > 0) return true;
            break;
        case PI_CONTROLLER_1_MOVE_UP:
            if (PisteInput_Controller_MoveY(PI_CONTROLLER_1) < 0) return true;
            break;
        case PI_CONTROLLER_1_MOVE_DOWN:
            if (PisteInput_Controller_MoveY(PI_CONTROLLER_1) > 0) return true;
            break;
        case PI_CONTROLLER_1_BUTTON_1:
            if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_1)) return true;
            break;
        case PI_CONTROLLER_1_BUTTON_2:
            if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_2)) return true;
            break;
        case PI_CONTROLLER_1_BUTTON_3:
            if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_3)) return true;
            break;
        case PI_CONTROLLER_1_BUTTON_4:
            if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_4)) return true;
            break;
        case PI_CONTROLLER_1_BUTTON_5:
            if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_5)) return true;
            break;
        case PI_CONTROLLER_1_BUTTON_6:
            if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_6)) return true;
            break;

        case PI_CONTROLLER_2_MOVE_LEFT:
            if (PisteInput_Controller_MoveX(PI_CONTROLLER_2) < 0) return true;
            break;
        case PI_CONTROLLER_2_MOVE_RIGHT:
            if (PisteInput_Controller_MoveX(PI_CONTROLLER_2) > 0) return true;
            break;
        case PI_CONTROLLER_2_MOVE_UP:
            if (PisteInput_Controller_MoveY(PI_CONTROLLER_2) < 0) return true;
            break;
        case PI_CONTROLLER_2_MOVE_DOWN:
            if (PisteInput_Controller_MoveY(PI_CONTROLLER_2) > 0) return true;
            break;
        case PI_CONTROLLER_2_BUTTON_1:
            if (PisteInput_Controller_Button(PI_CONTROLLER_2, PI_CONTROLLER_BUTTON_1)) return true;
            break;
        case PI_CONTROLLER_2_BUTTON_2:
            if (PisteInput_Controller_Button(PI_CONTROLLER_2, PI_CONTROLLER_BUTTON_2)) return true;
            break;
        case PI_CONTROLLER_2_BUTTON_3:
            if (PisteInput_Controller_Button(PI_CONTROLLER_2, PI_CONTROLLER_BUTTON_3)) return true;
            break;
        case PI_CONTROLLER_2_BUTTON_4:
            if (PisteInput_Controller_Button(PI_CONTROLLER_2, PI_CONTROLLER_BUTTON_4)) return true;
            break;
        case PI_CONTROLLER_2_BUTTON_5:
            if (PisteInput_Controller_Button(PI_CONTROLLER_2, PI_CONTROLLER_BUTTON_5)) return true;
            break;
        case PI_CONTROLLER_2_BUTTON_6:
            if (PisteInput_Controller_Button(PI_CONTROLLER_2, PI_CONTROLLER_BUTTON_6)) return true;
            break;

        default:
            break;
    }

    return false;
}

char *PisteInput_Read_Controller_Name(unsigned char controller) {
    switch (controller) {
        case PI_KB_A:
            return "movement_x";
            break;
        case PI_KB_B:
            return "movement_y";
            break;
        case PI_KB_C:
            return "c";
            break;
        case PI_KB_D:
            return "d";
            break;
        case PI_KB_E:
            return "e";
            break;
        case PI_KB_F:
            return "f";
            break;
        case PI_KB_G:
            return "g";
            break;
        case PI_KB_H:
            return "h";
            break;
        case PI_KB_I:
            return "i";
            break;
        case PI_KB_J:
            return "j";
            break;
        case PI_KB_K:
            return "k";
            break;
        case PI_KB_L:
            return "l";
            break;
        case PI_KB_M:
            return "m";
            break;
        case PI_KB_N:
            return "n";
            break;
        case PI_KB_O:
            return "o";
            break;
        case PI_KB_P:
            return "p";
            break;
        case PI_KB_Q:
            return "q";
            break;
        case PI_KB_R:
            return "r";
            break;
        case PI_KB_S:
            return "s";
            break;
        case PI_KB_T:
            return "t";
            break;
        case PI_KB_U:
            return "u";
            break;
        case PI_KB_V:
            return "v";
            break;
        case PI_KB_W:
            return "w";
            break;
        case PI_KB_X:
            return "x";
            break;
        case PI_KB_Y:
            return "y";
            break;
        case PI_KB_Z:
            return "z";
            break;
        case PI_KB_0:
            return "0";
            break;
        case PI_KB_1:
            return "1";
            break;
        case PI_KB_2:
            return "2";
            break;
        case PI_KB_3:
            return "3";
            break;
        case PI_KB_4:
            return "4";
            break;
        case PI_KB_5:
            return "5";
            break;
        case PI_KB_6:
            return "6";
            break;
        case PI_KB_7:
            return "7";
            break;
        case PI_KB_8:
            return "8";
            break;
        case PI_KB_9:
            return "9";
            break;
        case PI_KB_ENTER:
            return "return";
            break;
        case PI_KB_SPACE:
            return "space";
            break;
        case PI_KB_RSHIFT:
            return "right shift";
            break;
        case PI_KB_LSHIFT:
            return "left shift";
            break;
        case PI_KB_RALT:
            return "right alt";
            break;
        case PI_KB_LALT:
            return "left alt";
            break;
        case PI_KB_RCONTROL:
            return "right control";
            break;
        case PI_KB_LCONTROL:
            return "left control";
            break;
        case PI_KB_PAGEUP:
            return "page up";
            break;
        case PI_KB_PAGEDOWN:
            return "page down";
            break;
        case PI_KB_BACKSPACE:
            return "backspace";
            break;
        case PI_KB_NUMPADENTER:
            return "numpad enter";
            break;

        case PI_KB_UP:
            return "arrow up";
            break;
        case PI_KB_DOWN:
            return "arrow down";
            break;
        case PI_KB_LEFT:
            return "arrow left";
            break;
        case PI_KB_RIGHT:
            return "arrow right";
            break;

        case PI_KB_F1:
            return "f1";
            break;
        case PI_KB_F2:
            return "f2";
            break;
        case PI_KB_F3:
            return "f3";
            break;
        case PI_KB_F4:
            return "f4";
            break;
        case PI_KB_F5:
            return "f5";
            break;
        case PI_KB_F6:
            return "f6";
            break;
        case PI_KB_F7:
            return "f7";
            break;
        case PI_KB_F8:
            return "f8";
            break;
        case PI_KB_F9:
            return "f9";
            break;
        case PI_KB_F10:
            return "f10";
            break;

        case PI_KB_NUMPAD_0:
            return "numpad 0";
            break;
        case PI_KB_NUMPAD_1:
            return "numpad 1";
            break;
        case PI_KB_NUMPAD_2:
            return "numpad 2";
            break;
        case PI_KB_NUMPAD_3:
            return "numpad 3";
            break;
        case PI_KB_NUMPAD_4:
            return "numpad 4";
            break;
        case PI_KB_NUMPAD_5:
            return "numpad 5";
            break;
        case PI_KB_NUMPAD_6:
            return "numpad 6";
            break;
        case PI_KB_NUMPAD_7:
            return "numpad 7";
            break;
        case PI_KB_NUMPAD_8:
            return "numpad 8";
            break;
        case PI_KB_NUMPAD_9:
            return "numpad 9";
            break;

        case PI_MOUSE_LEFT_BUTTON:
            return "left mouse button";
            break;
        case PI_MOUSE_RIGHT_BUTTON:
            return "right mouse button";
            break;

        case PI_CONTROLLER_1_MOVE_LEFT:
            return "joystic 1 left";
            break;
        case PI_CONTROLLER_1_MOVE_RIGHT:
            return "joystic 1 right";
            break;
        case PI_CONTROLLER_1_MOVE_UP:
            return "joystic 1 up";
            break;
        case PI_CONTROLLER_1_MOVE_DOWN:
            return "joystic 1 down";
            break;
        case PI_CONTROLLER_1_BUTTON_1:
            return "joystic 1 button 1";
            break;
        case PI_CONTROLLER_1_BUTTON_2:
            return "joystic 1 button 2";
            break;
        case PI_CONTROLLER_1_BUTTON_3:
            return "joystic 1 button 3";
            break;
        case PI_CONTROLLER_1_BUTTON_4:
            return "joystic 1 button 4";
            break;
        case PI_CONTROLLER_1_BUTTON_5:
            return "joystic 1 button 5";
            break;
        case PI_CONTROLLER_1_BUTTON_6:
            return "joystic 1 button 6";
            break;

        case PI_CONTROLLER_2_MOVE_LEFT:
            return "joystic 2 left";
            break;
        case PI_CONTROLLER_2_MOVE_RIGHT:
            return "joystic 2 right";
            break;
        case PI_CONTROLLER_2_MOVE_UP:
            return "joystic 2 up";
            break;
        case PI_CONTROLLER_2_MOVE_DOWN:
            return "joystic 2 down";
            break;
        case PI_CONTROLLER_2_BUTTON_1:
            return "joystic 2 button 1";
            break;
        case PI_CONTROLLER_2_BUTTON_2:
            return "joystic 2 button 2";
            break;
        case PI_CONTROLLER_2_BUTTON_3:
            return "joystic 2 button 3";
            break;
        case PI_CONTROLLER_2_BUTTON_4:
            return "joystic 2 button 4";
            break;
        case PI_CONTROLLER_2_BUTTON_5:
            return "joystic 2 button 5";
            break;
        case PI_CONTROLLER_2_BUTTON_6:
            return "joystic 2 button 6";
            break;

        default:
            break;
    }

    return " ";
}

int PisteInput_Quit() {
    if (!PI_unload) {
        for (int i = 0; i < PI_MAX_CONTROLLERS; i++)
            if (PI_joysticks[i].lpdijoy) {
                PI_joysticks[i].lpdijoy->Unacquire();
                PI_joysticks[i].lpdijoy->Release();
                PI_joysticks[i].lpdijoy = NULL;
            }
        if (PI_lpdimouse) {
            PI_lpdimouse->Unacquire();
            PI_lpdimouse->Release();
            PI_lpdimouse = NULL;
        }
        if (PI_lpdikey) {
            PI_lpdikey->Unacquire();
            PI_lpdikey->Release();
            PI_lpdikey = NULL;
        }
        if (PI_lpdi) {
            PI_lpdi->Release();
            PI_lpdi = NULL;
        }
        PI_unload = true;
    }

    return 0;
}
