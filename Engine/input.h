/* PisteEngine PisteInput v2.0 */

#ifndef P_INPUT
#define P_INPUT

/* INCLUDES ----------------------------------------------------------------------------------*/

#define DIRECTINPUT_VERSION 0x0700
//#include "dinput.h" // TODO: Remember to fix the path //MUISTA KORJATA POLKU OIKEIN!!!!
//#include <windows.h>
#include "../old_headers.h"

/* DEFINES -----------------------------------------------------------------------------------*/

#define        PI_ERROR -1000

#define        PI_MAX_CONTROLLERS 2

#define        PI_CONTROLLER_1      0
#define        PI_CONTROLLER_2      1

#define        PI_CONTROLLER_XY_MAX      100        //Maximum (+/-) value returned by the functions Ohjain_X() and Ohjain_Y()
#define        PI_CONTROLLER_BUTTON_1 0
#define        PI_CONTROLLER_BUTTON_2 1
#define        PI_CONTROLLER_BUTTON_3 2
#define        PI_CONTROLLER_BUTTON_4 3
#define        PI_CONTROLLER_BUTTON_5 4
#define        PI_CONTROLLER_BUTTON_6 5

// Control codes
#define        PI_KB_A  1
#define        PI_KB_B  2
#define        PI_KB_C  3
#define        PI_KB_D  4
#define        PI_KB_E  5
#define        PI_KB_F  6
#define        PI_KB_G  7
#define        PI_KB_H  8
#define        PI_KB_I  9
#define        PI_KB_J  10
#define        PI_KB_K  12
#define        PI_KB_L  13
#define        PI_KB_M  14
#define        PI_KB_N  15
#define        PI_KB_O  16
#define        PI_KB_P  17
#define        PI_KB_Q  18
#define        PI_KB_R  19
#define        PI_KB_S  20
#define        PI_KB_T  21
#define        PI_KB_U  22
#define        PI_KB_V  23
#define        PI_KB_W  24
#define        PI_KB_X  25
#define        PI_KB_Y  26
#define        PI_KB_Z  27

#define        PI_KB_0  30
#define        PI_KB_1  31
#define        PI_KB_2  32
#define        PI_KB_3  33
#define        PI_KB_4  34
#define        PI_KB_5  35
#define        PI_KB_6  36
#define        PI_KB_7  37
#define        PI_KB_8  38
#define        PI_KB_9  39

#define        PI_KB_SPACE        40
#define        PI_KB_RSHIFT    41
#define        PI_KB_LSHIFT    42
#define        PI_KB_RALT        43
#define        PI_KB_LALT        44
#define        PI_KB_RCONTROL    45
#define        PI_KB_LCONTROL    46
#define        PI_KB_ENTER        47
#define        PI_KB_PAGEUP    48
#define        PI_KB_PAGEDOWN    49
#define        PI_KB_BACKSPACE    50
#define        PI_KB_NUMPADENTER 51

#define        PI_KB_UP        52
#define        PI_KB_DOWN        53
#define        PI_KB_LEFT        54
#define        PI_KB_RIGHT        55

#define        PI_KB_F1        60
#define        PI_KB_F2        61
#define        PI_KB_F3        62
#define        PI_KB_F4        63
#define        PI_KB_F5        64
#define        PI_KB_F6        65
#define        PI_KB_F7        66
#define        PI_KB_F8        67
#define        PI_KB_F9        68
#define        PI_KB_F10        69
#define        PI_KB_F11        70
#define        PI_KB_F12        71

#define        PI_KB_NUMPAD_0  80
#define        PI_KB_NUMPAD_1  81
#define        PI_KB_NUMPAD_2  82
#define        PI_KB_NUMPAD_3  83
#define        PI_KB_NUMPAD_4  84
#define        PI_KB_NUMPAD_5  85
#define        PI_KB_NUMPAD_6  86
#define        PI_KB_NUMPAD_7  87
#define        PI_KB_NUMPAD_8  88
#define        PI_KB_NUMPAD_9  89

#define        PI_MOUSE_LEFT_BUTTON    100
#define        PI_MOUSE_RIGHT_BUTTON    101
#define        PI_MOUSE_MOVE_RIGHT        102
#define        PI_MOUSE_MOVE_LEFT        103
#define        PI_MOUSE_MOVE_UP            104
#define        PI_MOUSE_MOVE_DOWN            105

#define        PI_CONTROLLER_1_MOVE_LEFT    110
#define        PI_CONTROLLER_1_MOVE_RIGHT        111
#define        PI_CONTROLLER_1_MOVE_UP            112
#define        PI_CONTROLLER_1_MOVE_DOWN            113
#define        PI_CONTROLLER_1_BUTTON_1        114
#define        PI_CONTROLLER_1_BUTTON_2        115
#define        PI_CONTROLLER_1_BUTTON_3        116
#define        PI_CONTROLLER_1_BUTTON_4        117
#define        PI_CONTROLLER_1_BUTTON_5        118
#define        PI_CONTROLLER_1_BUTTON_6        119

#define        PI_CONTROLLER_2_MOVE_LEFT    130
#define        PI_CONTROLLER_2_MOVE_RIGHT        131
#define        PI_CONTROLLER_2_MOVE_UP            132
#define        PI_CONTROLLER_2_MOVE_DOWN            133
#define        PI_CONTROLLER_2_BUTTON_1        134
#define        PI_CONTROLLER_2_BUTTON_2        135
#define        PI_CONTROLLER_2_BUTTON_3        136
#define        PI_CONTROLLER_2_BUTTON_4        137
#define        PI_CONTROLLER_2_BUTTON_5        138
#define        PI_CONTROLLER_2_BUTTON_6        139

/* PROTOTYPES --------------------------------------------------------------------------------*/

int PisteInput_Init(HWND &main_window_handle, HINSTANCE &hinstance_app);

//-----------------------------------------------------------------------------------------------------------
bool PisteInput_Update_Mouse();        // Updates mouse information (has it been moved, what buttons have been pressed...)
//-----------------------------------------------------------------------------------------------------------
bool PisteInput_Update_Keyboard();    // Updates keyboard information
//-----------------------------------------------------------------------------------------------------------
bool PisteInput_Update_Controllers();    // Updates controller information.
//-----------------------------------------------------------------------------------------------------------
bool PisteInput_Mouse_Right();    // Returns true if the right button of the mouse is pressed
//-----------------------------------------------------------------------------------------------------------
bool PisteInput_Mouse_Left();    // Returns true if the left button of the mouse is pressed
//-----------------------------------------------------------------------------------------------------------
int PisteInput_Mouse_X(int x);    // Kertoo paljonko hiiren kursorin on liikkunut vaakatasossa.
// If you want to get the current x-coordinate of the mouse cursor as movement_x result,
// set x to the previous x-coordinate of the mouse. If you want only the
// change, set x to 0.
//-----------------------------------------------------------------------------------------------------------
int PisteInput_Mouse_Y(int y);    // Returns how much the mouse cursor has moved vertically.
// See the description for PisteInput_Mouse_X(int x). Works in the same way.
//-----------------------------------------------------------------------------------------------------------
bool PisteInput_KeyDown(int key);// Returns true if the queried key is pressed on the keyboard.
// key = DirectInput constant. For example, the A key is DIK_A. DirectInput
// constants are listed at the end of this header.
//-----------------------------------------------------------------------------------------------------------
int PisteInput_Quit();        // Closes PisteInput
//-----------------------------------------------------------------------------------------------------------
bool PisteInput_Read_Controller(unsigned char controller); // Returns true if the user has activated the specified controller.
//-----------------------------------------------------------------------------------------------------------
unsigned char PisteInput_Read_Controller(); // Returns the controller that was last input.
//-----------------------------------------------------------------------------------------------------------
char *PisteInput_Read_Controller_Name(unsigned char controller); // Returns the name of the controller. For example, 'arrow left'.
//-----------------------------------------------------------------------------------------------------------
char PisteInput_Read_Keyboard(void); // Returns the character that was last input on the keyboard.
//-----------------------------------------------------------------------------------------------------------
bool PisteInput_Controller_Button(int controller, int index);

// Returns true if the specified button of the specified controller is pressed.
// For example, controller 1 button 1:
// PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_1);
//-----------------------------------------------------------------------------------------------------------
char *PisteInput_GameController_Name(int controller_index);    // Returns the name of the specified controller.
//-----------------------------------------------------------------------------------------------------------
int PisteInput_Controller_MoveX(int controller_index);    // Kertoo onko ohjainta painettu left tai right.
// Returns movement_x value less than zero if moved left, greater than zero if moved right.
// controller_index = index of game controller 1 or 2.
//-----------------------------------------------------------------------------------------------------------
int PisteInput_Controller_MoveY(int controller_index);    // Returns whether the controller has been moved up or down.
// Returns movement_x value less than zero if moved up, greater than zero if moved down.
// controller_index = index of game controller 1 or 2.
//-----------------------------------------------------------------------------------------------------------
bool PisteInput_IsMouseDetected();            // Returns true if movement_x mouse is detected in the system.
//-----------------------------------------------------------------------------------------------------------
bool PisteInput_IsControllerDetected(int controller_index); // Returns true if movement_x game controller is detected in the system.
// controller_index = index of game controller 1 or 2.
//-----------------------------------------------------------------------------------------------------------

#endif

/* List of DirectInput button codes:
	
	Button					Code
	-------					----------

	ESC						DIK_ESCAPE
	ENTER					DIK_RETURN
	Left CTRL				DIK_LCONTROL
	Right CTRL				DIK_RCONTROL
	Left SHIFT				DIK_LSHIFT
	Right SHIFT				DIK_RSHIFT
	Space   				DIK_SPACE
	Left ALT				DIK_LALT
	Right ALT				DIK_RALT

  
	Right					DIK_RIGHT
	Left					DIK_LEFT
	Up					    DIK_UP
	Down					DIK_DOWN


	F1						DIK_F1 
	F2						DIK_F2
	F3						DIK_F3
	...						...
	F10						DIK_F10


	0						DIK_0
	1						DIK_1
	2						DIK_2
	3						DIK_3
	...						...
	9						DIK_9
  

	A						DIK_A
	B						DIK_B
	C						DIK_C
	...						...
	Y						DIK_Y
	X						DIK_X
	

	1 (numpad)				DIK_NUMPAD1 
	2 (numpad)				DIK_NUMPAD2
	3 (numpad)				DIK_NUMPAD3
	...						...
	9 (numpad)				DIK_NUMPAD9


	+ (numpad)				DIK_ADD					
	- (numpad)				DIK_SUBTRACT 
	, (numpad)				DIK_DECIMAL 

	ENTER (numpad)			DIK_NUMPADENTER 

	*						DIK_MULTIPLY 
	-						DIK_MINUS
	=						DIK_EQUALS
	BACKSPACE				DIK_BACK
	TAB						DIK_TAB
	DELETE					DIK_DELETE 
	INSERT					DIK_INSERT
	PAGE DOWN				DIK_NEXT 
	PAGE UP					DIK_PRIOR
	[						DIK_LBRACKET 
	]						DIK_RBRACKET 
	:						DIK_SEMICOLON 
	'						DIK_APOSTROPHE 
	.						DIK_PERIOD 
	,						DIK_COMMA 
	
*/