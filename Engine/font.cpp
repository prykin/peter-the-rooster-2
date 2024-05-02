/* 
PisteEngine - PisteFont 1.1	
09.12.2001	Janne Kivilahti / Piste Gamez

Muutokset:
11.04.2004  Fontin parametrien charge_time tiedostosta - LoadFromFile(char *file)
*/

/* INCLUDES ----------------------------------------------------------------------------------*/

#include <windows.h>
#include "font.h"
#include <windowsx.h>
#include <mmsystem.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <io.h>
#include <fstream.h>
#include <iostream.h>
#include <string.h>
/* DEFINES -----------------------------------------------------------------------------------*/

/* VARIABLES ---------------------------------------------------------------------------------*/

/* PROTOTYPES --------------------------------------------------------------------------------*/

/* METHODS -----------------------------------------------------------------------------------*/

int Log_Write(char *message) {

    return (0);

    int error = 0;

    char *filename = "fontlog.txt";

    FILE *file;

    if ((file = fopen(filename, "a")) == NULL) {
        return (1);
    }

    char sequence[255];

    //memset(sequence,' ',sizeof(sequence));
    //sequence[60] = '\n';

    strcpy(sequence, message);

    fwrite(sequence, sizeof(char), strlen(sequence), file);

    fclose(file);

    return (0);
}

PisteFont::PisteFont(int height, int width, int count) {
    file_name = NULL;

    font_height = height;
    font_width = width;
    font_count = count;
    font_bitmap = (unsigned char *) malloc(height * width * count);
    font_buffer = PisteDraw_Buffer_Create(width * count, height, true, 255);

    this->InitFonts();
}

PisteFont::PisteFont() {
    file_name = NULL;
    Log_Write("PisteFont()\n");

    font_height = 0;
    font_width = 0;
    font_count = 0;
}

int PisteFont::LoadFromFile(char *path, char *file) {
    Log_Write("LoadFromFile(char *path, char *file_name)\n");
    Log_Write(path);
    Log_Write(file);
    Log_Write("\n");

    file_name = new PisteLanguage();
    Log_Write("file_name = new PisteLanguage();");

    char xfile[_MAX_PATH];
    strcpy(xfile, path);
    strcat(xfile, file);

    Log_Write("1.\n");

    file_name = new PisteLanguage();

    Log_Write("2.\n");

    if (!file_name->Read_File(xfile))
        return -1;

    Log_Write("3.\n");

    int index = 0;

    //width
    index = file_name->Get_Index("letter width");
    if (index != -1)
        font_width = atoi(file_name->Get_Text(index));
    else
        return -1;

    Log_Write("4.\n");

    //height
    index = file_name->Get_Index("letter height");
    if (index != -1)
        font_height = atoi(file_name->Get_Text(index));
    else
        return -1;

    Log_Write("5.\n");

    //letters count
    index = file_name->Get_Index("letters");
    if (index != -1)
        font_count = strlen(file_name->Get_Text(index));
    else
        return -1;

    Log_Write("6.\n");

    int buf_x, buf_y, buf_width;

    //kuvatiedoston width
    index = file_name->Get_Index("image width");
    if (index != -1)
        buf_width = atoi(file_name->Get_Text(index));
    else
        buf_width = 640;

    //buffer x
    index = file_name->Get_Index("image x");
    if (index != -1)
        buf_x = atoi(file_name->Get_Text(index));
    else
        return -1;

    Log_Write("7.\n");

    //buffer y
    index = file_name->Get_Index("image y");
    if (index != -1)
        buf_y = atoi(file_name->Get_Text(index));
    else
        return -1;

    Log_Write("8.\n");

    if (buf_x < 0 || buf_x > 640 || buf_y < 0 || buf_y > 480)
        return -1;

    Log_Write(file_name->Get_Text(file_name->Get_Index("letter width")));
    Log_Write(" ");
    Log_Write(file_name->Get_Text(file_name->Get_Index("letter height")));
    Log_Write(" ");
    Log_Write(file_name->Get_Text(file_name->Get_Index("letters")));
    Log_Write(" ");
    Log_Write(file_name->Get_Text(file_name->Get_Index("image x")));
    Log_Write(" ");
    Log_Write(file_name->Get_Text(file_name->Get_Index("image y")));
    Log_Write(" ");
    char text[255];
    itoa(font_count, text, 10);
    Log_Write(text);
    Log_Write("\n");

    //image_file
    index = file_name->Get_Index("image");

    if (index != -1) {

        Log_Write("9.\n");
        Log_Write(file_name->Get_Text(file_name->Get_Index("image")));
        Log_Write(" ");

        int temp_image = PisteDraw_Buffer_Create(buf_width, 480, true, 255);

        Log_Write("10.\n");

        strcpy(xfile, path);
        strcat(xfile, file_name->Get_Text(index));

        Log_Write("11. ");
        Log_Write(xfile);

        if (PisteDraw_Image_Load(temp_image, xfile, true) == PD_ERROR) {
            PisteDraw_Buffer_Destroy(temp_image);
            return -1;
        }

        Log_Write("12.\n");

        font_bitmap = (unsigned char *) malloc(font_height * font_width * font_count);
        font_buffer = PisteDraw_Buffer_Create(font_width * font_count, font_height, true, 255);

        Log_Write("13.\n");

        unsigned char *buffer = NULL;
        unsigned long screen_width;

        if (PisteDraw_Draw_Begin(temp_image, *&buffer, (unsigned long &) screen_width) == PD_ERROR)
            return -1;

        Log_Write("14. screen width");
        itoa(screen_width, text, 10);
        Log_Write(text);

        Log_Write("\n15.");
        GetBitmap(buf_x, buf_y, screen_width, buffer);

        Log_Write("16.\n");

        PisteDraw_Draw_End(temp_image);

        Log_Write("17.");

        PisteDraw_Buffer_Destroy(temp_image);

        Log_Write("18.");
    } else
        return -1;

    //this->InitFontsFromFile();
    this->InitFontsFromFile();

    return 0;
}


PisteFont::~PisteFont() {
    free(font_bitmap);
    PisteDraw_Buffer_Destroy(font_buffer);

    if (file_name != NULL)
        delete file_name;
}

int PisteFont::InitFontsFromFile(void) {
    Log_Write("\nInit_fonts_tiedosto(void)\n");

    int font_index[256];
    char kirjaimet[MAX_TEXT_LENGTH];
    int i;

    for (i = 0; i < 256; i++)
        font_table[i] = -1;

    Log_Write("\n1\n");

    strcpy(kirjaimet, file_name->Get_Text(file_name->Get_Index("letters")));

    Log_Write("\n2\n");

    for (i = 0; i < font_count; i++) {
        font_index[i] = i * font_width;
    }

    Log_Write("\n3\n");

    for (i = 0; i < font_count; i++) {
        font_table[(unsigned char) toupper(kirjaimet[i])] = font_index[i];
    }

    Log_Write("\n4\n");

    return (0);
}

int PisteFont::InitFonts(void) {
    Log_Write("\nInit_fonts(void)\n");

    int font_index[256];
    int i;

    for (i = 0; i < 256; i++)
        font_table[i] = -1;

    for (i = 0; i < font_count; i++)
        font_index[i] = i * font_width;

    font_table[(unsigned char) 'A'] = font_index[0];
    font_table[(unsigned char) 'B'] = font_index[1];
    font_table[(unsigned char) 'C'] = font_index[2];
    font_table[(unsigned char) 'D'] = font_index[3];
    font_table[(unsigned char) 'E'] = font_index[4];
    font_table[(unsigned char) 'F'] = font_index[5];
    font_table[(unsigned char) 'G'] = font_index[6];
    font_table[(unsigned char) 'H'] = font_index[7];
    font_table[(unsigned char) 'I'] = font_index[8];
    font_table[(unsigned char) 'J'] = font_index[9];
    font_table[(unsigned char) 'K'] = font_index[10];
    font_table[(unsigned char) 'L'] = font_index[11];
    font_table[(unsigned char) 'M'] = font_index[12];
    font_table[(unsigned char) 'N'] = font_index[13];
    font_table[(unsigned char) 'O'] = font_index[14];
    font_table[(unsigned char) 'P'] = font_index[15];
    font_table[(unsigned char) 'Q'] = font_index[16];
    font_table[(unsigned char) 'R'] = font_index[17];
    font_table[(unsigned char) 'S'] = font_index[18];
    font_table[(unsigned char) 'T'] = font_index[19];
    font_table[(unsigned char) 'U'] = font_index[20];
    font_table[(unsigned char) 'V'] = font_index[21];
    font_table[(unsigned char) 'W'] = font_index[22];
    font_table[(unsigned char) 'X'] = font_index[23];
    font_table[(unsigned char) 'Y'] = font_index[24];
    font_table[(unsigned char) 'Z'] = font_index[25];
    font_table[(unsigned char) toupper('�')] = font_index[26];
    font_table[(unsigned char) toupper('�')] = font_index[27];
    font_table[(unsigned char) toupper('�')] = font_index[28];
    font_table[(unsigned char) '0'] = font_index[29];
    font_table[(unsigned char) '1'] = font_index[30];
    font_table[(unsigned char) '2'] = font_index[31];
    font_table[(unsigned char) '3'] = font_index[32];
    font_table[(unsigned char) '4'] = font_index[33];
    font_table[(unsigned char) '5'] = font_index[34];
    font_table[(unsigned char) '6'] = font_index[35];
    font_table[(unsigned char) '7'] = font_index[36];
    font_table[(unsigned char) '8'] = font_index[37];
    font_table[(unsigned char) '9'] = font_index[38];
    font_table[(unsigned char) '.'] = font_index[39];
    font_table[(unsigned char) '!'] = font_index[40];
    font_table[(unsigned char) '?'] = font_index[41];
    font_table[(unsigned char) ':'] = font_index[42];
    font_table[(unsigned char) '-'] = font_index[43];
    font_table[(unsigned char) ','] = font_index[44];
    font_table[(unsigned char) '+'] = font_index[45];
    font_table[(unsigned char) '='] = font_index[46];
    font_table[(unsigned char) '('] = font_index[47];
    font_table[(unsigned char) ')'] = font_index[48];
    font_table[(unsigned char) '/'] = font_index[49];
    font_table[(unsigned char) '#'] = font_index[50];
    font_table[(unsigned char) '\\'] = font_index[51];
    font_table[(unsigned char) '_'] = font_index[52];
    font_table[(unsigned char) '%'] = font_index[53];
    return (0);
}

int PisteFont::GetBitmap(int buffer_x, int buffer_y, int screen_width, unsigned char *buffer) {
    int font_bitmap_leveys = font_count * font_width;

    Log_Write("\nGet_bitmap\n");

    unsigned char *fbuffer = NULL;
    unsigned long f_width;
    unsigned char color;

    Log_Write("\n1\n");

    if (PisteDraw_Draw_Begin(font_buffer, *&fbuffer, (unsigned long &) f_width) == PD_ERROR)
        return PD_ERROR;

    Log_Write("\n2\n");

    for (int y = 0; y < this->font_height; y++)
        for (int x = 0; x < font_bitmap_leveys - 1; x++) {
            color = buffer[buffer_x + x + (buffer_y + y) * screen_width];
            fbuffer[x + y * f_width] = color;
            if (color != 255)
                color %= 32;
            font_bitmap[x + y * font_bitmap_leveys] = color;

        }

    Log_Write("\n3\n");

    if (PisteDraw_Draw_End(font_buffer) == PD_ERROR)
        return PD_ERROR;

    Log_Write("\n4\n");

    return (0);
}

int PisteFont::DrawString(int font_x, int font_y, int buffer_width, char *text, unsigned char *back_buffer, bool loop) {
    int i = 0, i2;
    int x, y, ix, fx, fy, x2;
    unsigned char color;
    char character = '0';
    int font_bitmap_width = font_count * font_width;
    unsigned char *buffer;

    while (character != '\0') {
        character = text[i];
        ix = font_table[(unsigned char) toupper(character)];
        i2 = i * font_width;

        if (ix > -1) {
            for (x = 0; x < font_width; x++) {
                fx = font_x + x + i2;
                x2 = ix + x;
                buffer = back_buffer + fx;

                for (y = 0; y < font_height; y++) {
                    if ((color = font_bitmap[x2 + y * font_bitmap_width]) != 255) {
                        fy = font_y + y;
                        buffer[fy * buffer_width] = color;
                    }
                }
            }
        }
        i++;
    }
    return ((i - 1) * font_width);
}

int PisteFont::DrawString(char *text, int font_x, int font_y, int target_buffer) {
    int i = 0, i2;
    int ix;
    char merkki = '0';

    while (merkki != '\0') {
        merkki = text[i];
        ix = font_table[(unsigned char) toupper(merkki)];
        i2 = i * font_width;

        if (ix > -1)
            PisteDraw_Buffer_Flip_Fast(font_buffer, target_buffer, font_x + i2, font_y, ix, 0, ix + font_width,
                                       font_height);

        i++;
    }

    return ((i - 1) * font_width);
}

int PisteFont::DrawStringLed(int font_x, int font_y, int buffer_width, char *text,//[],
                                     unsigned char *back_buffer) {
    int i = 0, i2;
    int x, y, ix, fx, fy, x2;
    unsigned char color;
    char character = '0';
    int font_bitmap_width = font_count * font_width;

    while (character != '\0') {
        character = text[i];
        ix = font_table[(unsigned char) toupper(character)];
        i2 = i * font_width;
        if (ix > -1) {
            for (x = 0; x < font_width; x++) {
                fx = font_x + x;
                x2 = ix + x;
                for (y = 0; y < font_height; y++) {
                    color = font_bitmap[x2 + y * font_bitmap_width];
                    if (color == 11 || color == 31) {
                        fy = font_y + y;
                        fy %= 470;
                        color++;
                        back_buffer[fx + i2 + fy * buffer_width] = color;
                    }
                }
            }
        }
        i++;
    }
    return ((i - 1) * font_width);
}

int PisteFont::DrawStringTransparent(int font_x, int font_y, int buffer_width, RECT clipper, char *text,
                                     unsigned char *back_buffer, bool loop, int transparency) {
    int i = 0;
    int x, y, ix, fx, fy, p1, p2;
    unsigned char color, color2, color3;
    char character = '0';

    int font_bitmap_width = font_count * font_width;

    if (transparency > 100)
        transparency = 100;

    p1 = transparency;
    p2 = 100 - transparency;

    while (character != '\0') {
        character = text[i];
        ix = font_table[(unsigned char) toupper(character)];
        if (ix > -1) {
            for (x = 0; x < font_width; x++) {
                fx = font_x + x + i * font_width;
                //if (loop) fx %= 480;
                //x2 = ix + x;

                if (fx >= clipper.left && fx <= clipper.right)
                    for (y = 0; y < font_height; y++) {
                        color = font_bitmap[ix + x + y * font_bitmap_width];

                        if (color != 255) {
                            fy = font_y + y;

                            if (fy >= clipper.top && fy < clipper.bottom) {
                                //fy %= 470;
                                fy *= buffer_width;
                                fy += fx;
                                color2 = back_buffer[fy];
                                /*
                                color3  = color2;
                                color2 %= 32;
                                color3 -= color2;
        */
                                color3 = (color2 >> 5) << 5;
                                color2 -= color3;

                                color = (color * p1 + color2 * p2) / 100;
                                back_buffer[fy] = color + color3;
                            }
                        }
                    }
            }
        }
        i++;
    }
    return ((i - 1) * font_width);
}

int PisteFont::DrawStringColored(int font_x, int font_y, int buffer_width, char *text, unsigned char *back_buffer,
                                 bool loop, unsigned char color2) {
    int i = 0, i2;
    int x, y, ix, fx, fy, x2;
    unsigned char color;
    char character = '0';
    int font_bitmap_width = font_count * font_width;

    while (character != '\0') {
        character = text[i];
        ix = font_table[(unsigned char) toupper(character)];
        i2 = i * font_width;
        if (ix > -1) {
            for (x = 0; x < font_width; x++) {
                fx = font_x + x + i2;
                if (loop) fx %= 480;
                x2 = ix + x;
                for (y = 0; y < font_height; y++) {
                    color = font_bitmap[x2 + y * font_bitmap_width];

                    if (color != 255) {
                        fy = font_y + y;
                        fy %= 470;
                        color /= 2;
                        back_buffer[fx + fy * buffer_width] = color + color2;
                    }
                }
            }
        }
        i++;
    }
    return ((i - 1) * font_width);
}

