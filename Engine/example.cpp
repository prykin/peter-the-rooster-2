/* GAME.CPP		PisteEngine Example Program v0.1 by Janne Kivilahti 27.12.2001
                Program functionality:
                There is a background_image on the screen, scrolling from left to right.
                There is a floating ball on the screen, controllable with the mouse or gamepad.
                Pressing the A key fades the screen to black, and pressing the S key fades it back.
                The program execution ends with ESC, the right mouse button, or gamepad button 2.
				
                At the moment, PisteEngine provides:
                PisteInput = Handling of mouse, keyboard, and gamepad inputs.
                PisteDraw  = Drawing to the screen. Also includes PisteFont.
                PisteWait  = Timing (ensures the game runs at the same speed on all machines).

                Hiding the window component was not as straightforward as I imagined,
                so everything related to window initialization, etc., is "hidden" right at the bottom of the code.

                I haven't commented much on those Piste codes, but in this example,
                there are many explanations. Try to make sense of them :)

                Limitations:

            -	In free drawing, only one surface can be locked at a time.
            -	When a surface is locked in free drawing,
                you cannot use the PisteDraw_Font_Write() method.
            -	Do not flip from the same surface to the same surface.


*/


/* PRE DEFINITIONS ----------------------------------------------------------------------------*/

#define WIN32_LEAN_AND_MEAN
#define INITGUID

/* INCLUDES -----------------------------------------------------------------------------------*/

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <math.h>
#include "input.h"
#include "draw.h"
#include "sound.h"
#include "wait.h"

/* DEFINITIONS --------------------------------------------------------------------------------*/

#define WINDOW_CLASS_NAME "WINCLASS1"
#define    GAME_NAME          "ANY GAME"

const int SCREEN_WIDTH = 800;    // Do not touch! Other values do not (yet) work!
const int SCREEN_HEIGHT = 600;    // Do not touch!
const int SCREEN_BPP = 8;    // Do not touch!
const int MAX_COLORS_PALETTE = 256;    // Do not touch!

/* GLOBAL VARIABLES ---------------------------------------------------------------------------*/

HWND window_handle = NULL; // main window handle
HINSTANCE hinstance_app = NULL; // hinstance?
HDC global_dc = NULL; // global dc?

bool DirectX_error = false;// if this becomes true, the program will be terminated
char DirectX_error_message[2] = " ";  // not yet (properly) in use

bool window_closed = false;// is the window closed

int image_buffer1 = NULL;                // index to PisteDraw's image buffer array
int image_buffer2 = NULL;                // index to PisteDraw's image buffer array

int font1,                            // index to PisteDraw's font array
font2;                                // index to PisteDraw's font array

int sound1;                                // index to PisteSound's sound array
int sound2;                                // index to PisteSound's sound array

int ball_x = 320;
int ball_y = 240;
int map_x = 0;

int key_delay = 0;

/* PELI ---------------------------------------------------------------------------------------*/

int Game_Init(void) {

    // Initializing PisteDraw - Drawing on the Screen
    if ((PisteSound_Init((HWND &) window_handle, (HINSTANCE &) hinstance_app, 1, 22050, 8)) == PS_ERROR) {
        DirectX_error = true;
        return 1;
    }

    // Initializing PisteInput - Mouse and Keyboard Control
    if ((PisteInput_Init((HWND &) window_handle, (HINSTANCE &) hinstance_app)) == PI_ERROR) {
        DirectX_error = true;
        return 1;
    }


    // Initializing PisteDraw - Drawing on the Screen
    if ((PisteDraw_Init((HWND &) window_handle, (HINSTANCE &) hinstance_app, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                        MAX_COLORS_PALETTE)) == PD_ERROR) {
        DirectX_error = true;
        return 1;
    }


    // Creating the first image buffer:
    // SCREEN_WIDTH and SCREEN_HEIGHT: size of the screen's height and width (quite obvious?)
    // true: Buffer is located in video memory (VRAM). If in system memory, then false.
    // 255: Transparency color.
    // The method returns an integer, which is the index key to the buffer array. So, keep the key.Eli ota key talteen.
    if ((image_buffer1 = PisteDraw_Buffer_Create(SCREEN_WIDTH, SCREEN_HEIGHT, true, 255)) == PD_ERROR) {
        DirectX_error = true;
        return 1;
    }


    // Creating another one in the same way
    if ((image_buffer2 = PisteDraw_Buffer_Create(SCREEN_WIDTH, SCREEN_HEIGHT, true, 255)) == PD_ERROR) {
        DirectX_error = true;
        return 1;
    }


    // Loading an image into the first buffer. In this case, it's the background_image.
    // image_buffer1: index to PisteDraw's array containing image buffers.
    // "wormz.bmp": Image to be loaded.
    // true: Load and define palette. If not loading palette, then false.
    if (PisteDraw_Image_Load(image_buffer1, "wormz.bmp", true) == PD_ERROR) {
        DirectX_error = true;
        return 1;
    }


    // Loading another image into the second buffer. Palette not loaded. The image contains sprites and such...
    if (PisteDraw_Image_Load(image_buffer2, "wormz2.bmp", false) == PD_ERROR) {
        DirectX_error = true;
        return 1;
    }


    // Creating a new font. A bit trickier: Need to calculate pixels in the image.
    // image_buffer2: Buffer from which the font graphics are taken.
    // 1,422: X- and Y-coordinates where the font graphics start in the buffer. Top-left corner.
    // 14,14: Width and height of one character.
    // 29: How many different characters are in this font.
    if ((font1 = PisteDraw_Font_Create(image_buffer2, 1, 422, 14, 14, 29)) == PD_ERROR) {
        DirectX_error = true;
        return 1;
    }

    // And the same thing...
    if ((font2 = PisteDraw_Font_Create(image_buffer2, 1, 467, 6, 12, 46)) == PD_ERROR) {
        DirectX_error = true;
        return 1;
    }

    // Setting edges for the background buffer where drawing is not allowed. PD_BACKBUFFER refers
    // to the image buffer that is flipped to the screen once per frame. If you want to set edges
    // for some other image buffer, use its index key: for example, image_buffer1.
    // NOTE.! For each image buffer created with the PisteDraw_Buffer_Create() method,
    // the edges are automatically set to the screen width and height. But here, they can be
    // set separately.
    // PD_BACKBUFFER: image buffer where the edges are set.
    // 20,20,620,460: Left edge, top edge, right-bound edge, bottom edge.
    PisteDraw_SetClipper(PD_BACKBUFFER, 20, 20, 620, 460);


    if ((sound1 = PisteSound_SFX_New("sfx1.wav")) == PS_ERROR) {
        DirectX_error = true;
        return 1;
    }

    if ((sound2 = PisteSound_SFX_New("sfx2.wav")) == PS_ERROR) {
        DirectX_error = true;
        return 1;
    }

    // Works like TickCount (or actually is). Start timing for the first time.
    PisteWait_Start();



/*	Or the whole thing briefly without error checks and fluff...

	PisteInput_Init((HWND &)window_handle, (HINSTANCE &)hinstance_app);
	PisteDraw_Init((HWND &)window_handle, (HINSTANCE &) hinstance_app,
	 			     SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, MAX_COLORS_PALETTE);
	image_buffer1 = PisteDraw_Buffer_New(SCREEN_WIDTH,SCREEN_HEIGHT, true, 255);
	image_buffer2 = PisteDraw_Buffer_Create(SCREEN_WIDTH,SCREEN_HEIGHT, true, 255);
	PisteDraw_Load_Image(image_buffer1,"wormz.bmp", true);
	PisteDraw_Image_Load(image_buffer2,"wormz2.bmp", false);
	PisteDraw_SetClipper(PD_BACKBUFFER,20,20,620,460);
	font1 = PisteDraw_Font_Create(image_buffer2,1,422,14,14,29);
	PisteWait_Start();
*/

    return 0;
}

int Game_Main_Draw(void) {
    int x, y;
    map_x = 1 + map_x % 640;

    /* FILLING THE SCREEN WITH GRAY COLOR */

    // Filling the screen with color:
    // PD_BACKBUFFER: buffer to be filled
    // 10: color from the palette to fill with.
    PisteDraw_Buffer_Clear(PD_BACKBUFFER, 10);
    // Or:
    // PisteDraw_Buffer_Clear(PD_BACKBUFFER, 0, 0, 640, 480, 10);

    /* MAKING BACKGROUND SCROLL */

    // Copying whole buffers to other buffers:
    // image_buffer1: source buffer to copy from
    // PD_BACKBUFFER: target buffer to copy to
    // map_x,0: X and Y where to copy to in the target buffer
    PisteDraw_Buffer_Flip_Fast(image_buffer1, PD_BACKBUFFER, map_x, 0);
    // Same thing as before
    PisteDraw_Buffer_Flip_Fast(image_buffer1, PD_BACKBUFFER, map_x - 640, 0);

    /* DRAW A BALL TO THE MOUSE CURSOR POSITIONE */

    // Copying a part of the buffer to another buffer at a certain position:
    // image_buffer2: Source buffer to copy from
    // PD_BACKBUFFER: Target buffer to copy to (in this case, the surface that is flipped to the screen at the end)
    // ball_x, ball_y: X and Y coordinates where to copy to in the target buffer (left_bound top_left)
    // 200,141,219,160: Area from the source buffer to copy from.
    PisteDraw_Buffer_Flip_Fast(image_buffer2, PD_BACKBUFFER, ball_x, ball_y, 200, 141, 219, 160);
    //PisteDraw_Buffer_Flip(image_buffer2,PD_BACKBUFFER,ball_x,ball_y,true,true);


    /* DRAWING SNOWFALL TO THE IMAGE BUFFER AND BACKGROUND BUFFER */

    // Prepare Free drawing buffer
    unsigned char *buffer = NULL;
    unsigned long tod_leveys;
    // Before anything can be messed with in the image buffer, it must be locked first.
    // image_buffer1: Buffer to be locked for drawing
    // buffer: Pointer to the content of the image buffer
    // real_width: Actual width of the image buffer
    // Even if the screen width is set to 640x480, it can actually be wider
    PisteDraw_Draw_Begin(image_buffer1, *&buffer, (unsigned long &) tod_leveys);

    // Drawing snowfall to the image buffer

    for (x = 100; x < 200; x++)
        for (y = 100; y < 200; y++)
            buffer[x + y * tod_leveys] = rand() % 30;

    PisteDraw_Draw_End(image_buffer1); //Possible, but not mandatory in this interval

    // Prepare another buffer for drawing. Before that, check if some other buffer is locked. If so, release it first.
    PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer, (unsigned long &) tod_leveys);

    for (x = 300; x < 350; x++)
        for (y = 300; y < 350; y++)
            buffer[x + y * tod_leveys] = rand() % 30;

    // Stop drawing by releasing the locked buffer.
    PisteDraw_Draw_End(PD_BACKBUFFER);

    /* DRAWING "TEST" TEXT TO THE SCREEN */

    // Write text to the screen using the previously allocated font //
    // font1: index key to the created font used for writing
    // "test": probably the text written to the screen :)
    // 10,10: where to write.
    // CAUTION! The font drawing doesn't care about the edges and will write recklessly
    // even outside the screen if it can be accessed.
    // The return value is the width of the written text in pixels
    PisteDraw_Font_Write(font1, "piste engine test", PD_BACKBUFFER, 10, 10);

    /* DRAWING INSTRUCTION TEXTS TO THE SCREEN */

    int tekstin_leveys = 0;
    tekstin_leveys += PisteDraw_Font_Write(font2, "end: esc, right-bound mouse button or gamepad button 2.",
                                           PD_BACKBUFFER, 10, 40);
    PisteDraw_Font_Write(font2, "movement_x and s: fading.", PD_BACKBUFFER, 10, 60);

    /* DRAWING THE AMOUNT OF FREE VIDEO MEMORY TO THE SCREEN */

    char vram[30];
    _ltoa(PisteDraw_GetMemoryUsed(), vram, 10);
    PisteDraw_Font_Write(font2, vram, PD_BACKBUFFER, 300, 10);


    /* DRAWING TEXT TO THE SCREEN IF GAMEPAD BUTTON 2 IS PRESSED */

    if (PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_1))
        PisteDraw_Font_Write(font2, "gamepad button 1 pressed", PD_BACKBUFFER, 10, 75);

    /* WAITING IF THE COMPUTER IS TOO FAST */

    // If we were too quick, wait a moment. Same as e.g., TickWait.
    // 10 hundredths of a second (if I remember correctly)
    PisteWait_Wait(10);

    /* FLIPPING THE BACKGROUND BUFFER (PD_BACKBUFFER) TO THE SCREEN */

    // Dumping the contents of the double buffer PD_BACKBUFFER to the display memory
    PisteDraw_UpdateScreen();

    // Starting the timer again, which is then checked again with the PisteWait() method.
    PisteWait_Start();



    /* All of the above without unnecessary nonsense...

    int x, y;
    map_x = 1 + map_x%640;
    PisteDraw_Buffer_Clear(PD_BACKBUFFER,10);
    PisteDraw_Buffer_Flip_Nopea(kuvabufferi1,PD_BACKBUFFER,map_x,0);
    PisteDraw_Buffer_Flip_Fast(kuvabufferi1,PD_BACKBUFFER,map_x-640,0);
    PisteDraw_Buffer_Flip_Fast(kuvabufferi2,PD_BACKBUFFER,pallo_x,pallo_y,200,141,219,160);
    unsigned char *buffer = NULL;
    unsigned long tod_leveys;
    PisteDraw_Piirto_Aloita(kuvabufferi1, *&buffer, (unsigned long &)tod_leveys);

    for (x=100;x<200;x++)
        for (y=100;y<200;y++)
            buffer[x+y*tod_leveys] = rand()%30;

    PisteDraw_Draw_Begin(PD_BACKBUFFER, *&buffer, (unsigned long &)tod_leveys);

    for (x=300;x<350;x++)
        for (y=300;y<350;y++)
            buffer[x+y*tod_leveys] = rand()%30;

    PisteDraw_Draw_End();
    PisteDraw_Font_Kirjoita(font1,"testi",PD_BACKBUFFER,10,10);
    int tekstin_leveys = 0;
    tekstin_leveys += PisteDraw_Font_Kirjoita(font2,"lopetus: esc tai right_bound hiiren nappi.",PD_BACKBUFFER,10,40);
    PisteDraw_Font_Kirjoita(font2,"movement_x ja s: feidaus.",PD_BACKBUFFER,10,60);
    char  vram[30];
    ltoa(PisteDraw_GetMemoryUsed(), vram, 10);
    PisteDraw_Font_Write(font2,vram,PD_BACKBUFFER,300,10);
    PisteWait_Wait(10);
    PisteDraw_UpdateScreen();
    PisteWait_Start();
    */

    return 0;
}

int Game_Main(void) {
    if (window_closed)
        return (0);

    /* GET THE CURRENT STATES OF THE KEYBOARD, MOUSE, AND GAMEPADS */

    // Keyboard
    if (!PisteInput_Update_Keyboard())        //Get the states of the keys
        DirectX_error = true;

    // Mouse
    if (!PisteInput_Update_Mouse())            //Get the state of the mouse
        DirectX_error = true;

    // All gamepads
    if (!PisteInput_Update_Controllers()) {}
    //DirectX_error = true;

    // Add the transformation of the mouse cursor position to the ball's x and y variables.

    ball_x = PisteInput_Mouse_X(ball_x);    //Or: ball_x += PisteInput_Mouse_X(0);
    ball_y = PisteInput_Mouse_Y(ball_y);

    // Add the movements of gamepad 1 to the ball's x and y variables.

    ball_x += PisteInput_Controller_MoveX(PI_CONTROLLER_1) / 50;
    ball_y += PisteInput_Controller_MoveY(PI_CONTROLLER_1) / 50;

    if (ball_x > SCREEN_WIDTH)
        ball_x = SCREEN_WIDTH;

    if (ball_y > SCREEN_HEIGHT)
        ball_y = SCREEN_HEIGHT;

    if (ball_x < 0)
        ball_x = 0;

    if (ball_y < 0)
        ball_y = 0;

    Game_Main_Draw();


    /* FADE THE PALETTE IF THE USER PRESSES THE A AND S BUTTONS */

    // Check if the user has pressed the A button.
    if (PisteInput_KeyDown(DIK_A))
        PisteDraw_Fade_Palette_Out(PD_FADE_SLOW);
    // If so, start fading the palette to black.
    // Available speeds: PD_FADE_SLOW (=1), PD_FADE_NORMAL (=2), PD_FADE_FAST (=5)
    // You can actually give any positive value as a parameter.
    // Fading occurs from 100 to 0.
    if (PisteInput_KeyDown(DIK_S))
        PisteDraw_Fade_Palette_In(PD_FADE_NORMAL);

    if (key_delay == 0) {

        if (PisteInput_KeyDown(DIK_1)) {
            if (PisteSound_SFX_Play(sound1) == PS_ERROR)
                DirectX_error = true;
            key_delay = 30;
        }

        if (PisteInput_KeyDown(DIK_2)) {
            if (PisteSound_SFX_Play(sound2) == PS_ERROR)
                DirectX_error = true;
            key_delay = 30;
        }
    } else
        key_delay--;

    /* EXIT THE PROGRAM IF THE USER PRESSES ESC OR THE RIGHT MOUSE BUTTON */

    // If the user presses ESC or the right mouse button, or gamepad button 2,
    // exit the program.
    // PisteInput_MouseLeft() = mouse left button"

    if (PisteInput_KeyDown(DIK_ESCAPE) || PisteInput_Mouse_Right() ||
            PisteInput_Controller_Button(PI_CONTROLLER_1, PI_CONTROLLER_BUTTON_2)) {
        SendMessage(window_handle, WM_CLOSE, 0, 0);
        window_closed = true;
    }

    return 0;
}

int Game_Quit(void) {
    PisteSound_Stop();
    PisteDraw_Quit();
    PisteInput_Quit();

    // If an error has occurred, display a text box with the message 'Error!'.
    // Oh, the user will surely be surprised by this informative message :)

    if (DirectX_error) {
        MessageBox(window_handle, PisteDraw_GetError(), "Error!", MB_OK | MB_ICONEXCLAMATION);

        MessageBox(window_handle, PisteSound_GetError(), "Error!", MB_OK | MB_ICONEXCLAMATION);
    }
    return 0;
}


/* WINDOW -------------------------------------------------------------------------------------*/

// Follow the window code. The following is the window's 'event listener'.


LRESULT CALLBACK WindowProc(HWND hwnd,UINT msg, WPARAM wparam,LPARAM lparam) {
    PAINTSTRUCT ps;
    HDC hdc;

    switch(msg)
    {
        case WM_CREATE:
        {
            return(0);
        } break;

        case WM_PAINT:
        {
            hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return(0);
        } break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return(0);
        } break;

        default:
            break;

    }

    return (DefWindowProc(hwnd, msg, wparam, lparam));
}

// Everything starts and ends here: WinMain. From here it all begins and within this, the game runs.

int IDI_ICON1;
int IDC_CURSOR1;

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
    WNDCLASSEX winclass;
    HWND hwnd;
    MSG msg;

    winclass.cbSize = sizeof(WNDCLASSEX);
    winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    winclass.lpfnWndProc = WindowProc;
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hInstance = hinstance;
    winclass.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
    winclass.hCursor = LoadCursor(hinstance, MAKEINTRESOURCE(IDC_CURSOR1));
    winclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    winclass.lpszMenuName = NULL;
    winclass.lpszClassName = WINDOW_CLASS_NAME;
    winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    hinstance_app = hinstance;

    if (!RegisterClassEx(&winclass))
        return(0);

    if (!(hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME, GAME_NAME, WS_POPUP | WS_VISIBLE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL, NULL, hinstance, NULL)))
        return(0);

    window_handle = hwnd;
    Game_Init();
    ShowCursor(FALSE);

    while(!DirectX_error)
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Game_Main();
    }

    Game_Quit();
    ShowCursor(TRUE);
    return(msg.wParam);
}