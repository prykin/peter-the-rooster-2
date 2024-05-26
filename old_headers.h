#ifndef PEKKAKANA2_OLD_HEADERS_H
#define PEKKAKANA2_OLD_HEADERS_H

using namespace std;

char* itoa(int a, char *b, int c) {
    return _itoa(a,b,c);
}

char* ltoa(long a, char *b, int c) {
    return _ltoa(a,b,c);
}

char* strlwr(char *a) {
    return _strlwr(a);
}

#define HIunsigned
#define LOunsigned

#endif //PEKKAKANA2_OLD_HEADERS_H
