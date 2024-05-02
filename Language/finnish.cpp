#define WIN32_LEAN_AND_MEAN
#define INITGUID

#include <string.h>
#include <istream.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "language.h"
#include "../old_headers.h"

int main() {
    PisteLanguage *language = new PisteLanguage();

    if (!language->Read_File("suomi.lng"))
        cout << "Failed to read the file!\n";
    else
        cout << "File read successfully!\n";

    int text_index = 0;

    text_index = language->Get_Index("testi2");
    cout << "Index: " << text_index << " text: |" << language->Get_Text(text_index) << "|\n";

    for (int i = 0; i < 10; i++) {
        cout << "Index: " << i << " text: |" << language->Get_Text(i) << "|\n";
        text_index++;
    }
    std::cin;

    delete language;

    return 0;
}
