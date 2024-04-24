/* INCLUDES ----------------------------------------------------------------------------------*/

//#include <windows.h>
//#include <windowsx.h>
//#include <mmsystem.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//#include <direct.h>
//#include <io.h>
//#include <fstream.h>
//#include <iostream.h>
#include <string.h>
#include "log.h"

/* VARIABLES ---------------------------------------------------------------------------------*/

bool write_scorelog = false;

/* METHODS ----------------------------------------------------------------------------------*/

void PisteLog_EnableWriting() {
    write_scorelog = true;
}

int PisteLog_Write(char *message) {

    if (write_scorelog) {

        int error = 0;

        char *filename = "log.txt";

        FILE *file;

        if ((file = fopen(filename, "movement_x")) == NULL) {
            return (1);
        }

        char sequence[255];

        strcpy(sequence, message);

        fwrite(sequence, sizeof(char), strlen(sequence), file);

        fclose(file);
    }

    return (0);
}