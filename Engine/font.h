#ifndef P_FONT
#define P_FONT

#include "draw.h"
#include "../Language/language.h"

class PisteFont {
private:
    int font_table[256];
    int font_height, font_width, font_count;
    PisteLanguage *file_name;

public:
    unsigned char *font_bitmap;
    int font_buffer;

    int InitFonts(void);

    int InitFontsFromFile(void);

    int GetBitmap(int buffer_x, int buffer_y, int screen_width, unsigned char *buffer);

    int DrawString(char *text, int font_x, int font_y, int target_buffer);

    int DrawString(int font_x, int font_y, int buffer_width, char *text, unsigned char *back_buffer, bool loop);

    int DrawStringLed(int font_x, int font_y, int buffer_width, char *text, unsigned char *back_buffer);

    int DrawStringTransparent(int font_x, int font_y, int buffer_width, RECT clipper, char *text,
                              unsigned char *back_buffer, bool loop, int transparency);

    int DrawStringColored(int font_x, int font_y, int buffer_width, char *text, unsigned char *back_buffer, bool loop,
                          unsigned char color2);

    int GetHeight(void) { return font_height; };

    int LoadFromFile(char *path, char *file_name);

    PisteFont(int height, int width, int count);

    PisteFont();

    ~PisteFont();
};

#endif