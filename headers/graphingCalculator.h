#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <math.h>

#define CLAY_IMPLEMENTATION
#include "clay.h"

#define MAX_BUFFER_SIZE 1024

#define FIRACODE    0
#define ROBOTO      1
#define WINGDINGS   2

typedef enum {
    CLAY_CUSTOM_WIDE_STRING,
} customElementType;

typedef struct {
    wchar_t* top;
    wchar_t* bottom;
} outputBuffers;

typedef enum {
    ADDITION = 0,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    EXPONENT
} operationType;

typedef struct {
    wchar_t character;
    uint16_t occurrence;
} occurrenceTable;

typedef struct {
    wchar_t* string;
    uint16_t stringLength;
    uint16_t fontId;
    uint16_t fontSize;
    Clay_Color textColour;
} wideText;

typedef struct {
    uint16_t fontId;
    uint16_t fontSize;
    Clay_Color textColour;
} Clay_Custom_Wide_String_Style;

typedef struct {
    uint64_t size;
    uint64_t arrayPointer;
    char **arrayBottom;
} variableStringArray;

void clearBuffer(wchar_t* buffer);
