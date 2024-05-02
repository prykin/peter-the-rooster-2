#ifndef PISTELANGUAGE
#define PISTELANGUAGE

//#include "../old_headers.h"

const int MAX_TEXTS = 200;
const int MAX_TEXT_LENGTH = 80;
const int MAX_TITLE_LENGTH = 50;//30

const int READ_SKIP = 0, READ_TITLE = 1, READ_TEXT = 2;

const char SEPARATOR_1 = '*', SEPARATOR_2 = ':';

class PisteLanguage {
public:

    PisteLanguage();

    PisteLanguage(char *filename);

    ~PisteLanguage();

    bool Read_File(char *filename);

    int Get_Index(char *title);

    char *Get_Text(int index);

    void Replace_Text(int index, char *text);

    //PisteLanguage *file;

private:
    // 200 texts
    char texts[MAX_TEXTS][MAX_TEXT_LENGTH + 1];
    // titles of the texts
    char titles[MAX_TEXTS][MAX_TITLE_LENGTH + 1];

    int read;
};

#endif