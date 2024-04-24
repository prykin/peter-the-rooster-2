#include <stdlib.h>
#include <stdio.h>
//#include <fstream.h>
//#include <iostream.h>
//#include <io.h>
#include <fcntl.h>
//#include <windows.h>

#include "language.h"

PisteLanguage::PisteLanguage() {
    read = READ_SKIP;

    for (int i = 0; i < MAX_TEXTS; i++) {
        strcpy(texts[i], "");
        strcpy(titles[i], "");
    }
}

PisteLanguage::PisteLanguage(char *file) {
    read = READ_SKIP;

    for (int i = 0; i < MAX_TEXTS; i++) {
        strcpy(texts[i], "");
        strcpy(titles[i], "");
    }

    Read_File(file);
}

PisteLanguage::~PisteLanguage() {}

bool PisteLanguage::Read_File(char *filename) {

    ifstream *file = new ifstream(filename, ios::in);

    if (file->fail()) {
        delete (file);
        return false;
    }

    for (int i = 0; i < MAX_TEXTS; i++) {
        strcpy(texts[i], "");
        strcpy(titles[i], "");
    }

    char character;
    int array_index = 0;
    int string_index = 0;
    read = READ_SKIP;

    bool continue_flag = true;

    while (continue_flag && file->peek() != EOF) {
        //file->read(character, sizeof(character));

        character = file->get();

        switch (character) {
            case SEPARATOR_1    :
                if (read == READ_SKIP) {
                    read = READ_TITLE;
                    string_index = 0;
                } else {
                    read = READ_SKIP;
                    array_index++;
                }
                break;

            case SEPARATOR_2    :
                if (read == READ_TITLE) {
                    read = READ_TEXT;
                    string_index = 0;
                    break;
                }

                if (read == READ_TEXT) {
                    if (string_index < MAX_TEXT_LENGTH) {
                        texts[array_index][string_index] = character;
                        texts[array_index][string_index + 1] = '\0';
                        string_index++;
                    }
                }
                break;

            case '\n'        :
                if (read != READ_SKIP) {
                    read = READ_SKIP;
                    array_index++;
                }
                break;

            case '\t'        :
            case '\v'        :
                break;

            default            :
                if (read != READ_SKIP && !(string_index == 0 && character == ' ')) {
                    if (read == READ_TITLE) {
                        if (string_index < MAX_TITLE_LENGTH) {
                            //strcat(titles[array_index],(char *)character);
                            titles[array_index][string_index] = character;
                            titles[array_index][string_index + 1] = '\0';
                            string_index++;
                        }
                    }
                    if (read == READ_TEXT) {
                        if (string_index < MAX_TEXT_LENGTH) {
                            //strcat(texts[array_index],(char *)character);
                            texts[array_index][string_index] = character;
                            texts[array_index][string_index + 1] = '\0';
                            string_index++;
                        }
                    }
                }
                break;

        }

        if (array_index >= MAX_TEXTS)
            continue_flag = false;
    }

    delete file;

    return true;
}

int PisteLanguage::Get_Index(char *title) {
    int i = 0;

    while (i < MAX_TEXTS && strcmp(titles[i], title) != 0)
        i++;

    if (i == MAX_TEXTS)
        return -1;

    return i;

}

char *PisteLanguage::Get_Text(int index) {
    if (index >= 0 && index < MAX_TEXTS)
        return texts[index];
    else
        return ".....";
}

void PisteLanguage::Replace_Text(int index, char *text) {
    if (index >= 0 && index < MAX_TEXTS)
        strcpy(texts[index], text);
}