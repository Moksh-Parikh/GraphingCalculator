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

#define CENTRE(min, max) (min) + ( ( (max) - (min) ) / 2)

#define INIT_INTEGER_STACK { .type = INTEGER, .top = -1, .integerContents = NULL, }
#define INIT_DOUBLE_STACK { .type = DOUBLE, .top = -1, .floatContents = NULL, }

#define STACK_IS_EMPTY(x) ((x).top < 0) + ((x).top >= 0)
#define STACK_ISNT_EMPTY(x) !STACK_IS_EMPTY(x)

#define BACKSPACE 0x08

#define COMPAREPRECEDENCE(x, y) getOperatorPrecedence((x)) - getOperatorPrecedence((y))

#define LEFT 0
#define RIGHT 1

#define MAX_BUFFER_SIZE 1024
#define MAXIMUM_BUTTONS 55

#define FIRACODE    0
#define ROBOTO      1
#define WINGDINGS   2

typedef enum {
    CLAY_CUSTOM_WIDE_STRING,
    CLAY_CUSTOM_GRAPH
} customElementType;

typedef struct {
    wchar_t* top;
    wchar_t* bottom;
} outputBuffers;

typedef struct {
    wchar_t* string;
    uint16_t stringLength;
    uint16_t fontId;
    uint16_t fontSize;
    Clay_Color textColour;
} wideText;

typedef struct {
    int32_t leftTopX;
    int32_t leftTopY;
    uint32_t width;
    uint32_t height;
    Clay_Color backgroundColour;
    Clay_Color graphColour;
    uint16_t horizontalGridLines;
    uint16_t verticalGridLines;
} graphData;

typedef struct {
    customElementType type;
    union {
        wideText text;
        graphData graph;
    };
} Clay_CustomElementData;

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

typedef struct {
    Clay_ElementId id;
    int width;
    int height;
    Clay_Color defaultColour;
    Clay_Color hoveredColour;
    Clay_Color textColor;
    Clay_String text;
    int fontSize;
    int action;
} buttonStyle;

typedef struct {
    int maxWidth;
    int height;
    Clay_Color backgroundColour;
    Clay_Color barColour;
    float progress;
} progressBarStyle;

typedef struct {
    Clay_ElementId id;
    Clay_SizingAxis width;
    int height;
    Clay_Color boxColour;
    Clay_Color hoveredColour;
    Clay_Color textColour;
    Clay_Color borderColour;
    int font;
    int fontSize;
    int index;
} headerStyle;

typedef struct {
    buttonStyle buttonTheme;
    progressBarStyle progressBarTheme;
    headerStyle headerTheme;
    Clay_Color mainColour;
    Clay_Color accentColour1;
    Clay_Color accentColour2;
    Clay_Color hoveredAccent1;
    Clay_Color hoveredAccent2;
} clayTheme;

typedef struct {
    void* memory;
    int offset;
} Arena;

typedef struct {
    char** buttonTextArray;
    HFONT* fontArray;
    int fontArraySize;
    Clay_Dimensions buttonGridDimensions;
    outputBuffers displayBuffers;
} Clay_Win32_WndProc_Data;

typedef struct {
    HDC hdcMem;
    HBITMAP hbmMem;
    HBITMAP hbmMemPrev;
    void* pBits;
    SIZE size;
} HDCSubstitute;

typedef enum {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    EXPONENT,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    DIGIT,
    DECIMAL_POINT,
    LETTER,
    NONE,
} operationType;

typedef enum {
    INTEGER,
    DOUBLE
} stackType;

typedef struct {
    stackType type;
    int32_t top;
    union {
        int32_t* integerContents;
        double* floatContents;
    };
} stack;

void CenterWindow(HWND hWnd);

// stack.c
int pushToStack(void* value, stack* inputStack);
int popStack(void* output, stack* inputStack);
void emptyStack(stack *inputStack);

// stringFunctions.c
void checkAndClearBuffer(wchar_t** buffer, wchar_t* comparison);
void clearBuffer(wchar_t* buffer);
void fillStringArray(char** inputArray, char** outputArray, int outputArraySize);
void stringToClayString(char* inString, Clay_String* outString);

// layout.c
void customWideText(wchar_t* inString, Clay_Custom_Wide_String_Style stringData);
void clayButton(buttonStyle style);
void clayButtonRow(int buttons, int rowNumber, char** textArray, buttonStyle styleArray[], Clay_BoundingBox containerSize);
Clay_RenderCommandArray createLayout(Clay_Dimensions dimensions, clayTheme layoutTheme, Clay_Dimensions buttonGridDimensions);

// handlers.c
void handleButton(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData); 
int16_t findWideCharacter(wchar_t* inputString, wchar_t searchCharacter);
operationType operationParser(wchar_t operationCharacter);
double calculate(double number1, double number2, operationType operation);
uint16_t inputHandler(wchar_t* inputBuffer, wchar_t* outputBuffer);

// renderer
DWORD Clay_Win32_GetRendererFlags();
void Clay_Win32_SetRendererFlags(DWORD dwFlags);
static inline Clay_Color ColorBlend(Clay_Color base, Clay_Color overlay, float factor);
static float RoundedRectPixelCoverage(int x, int y, const Clay_CornerRadius radius, int width, int height);
static void CreateHDCSubstitute(HDCSubstitute* phdcs, HDC hdcSrc, PRECT prc);
static void __Clay_Win32_FillRoundRect(HDC hdc, PRECT prc, Clay_Color color, Clay_CornerRadius radius);
void Clay_Win32_Render(HWND hwnd, Clay_RenderCommandArray renderCommands, HFONT* fonts);
static Clay_Dimensions Clay_Win32_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);
static Clay_Dimensions Clay_Custom_Win32_MeasureWideText(wideText text, Clay_Custom_Wide_String_Style *config, void *userData);
HFONT Clay_Win32_SimpleCreateFont(const char* filePath, const char* family, int height, int weight);

// equationEvaluator.c
operationType operatorType(char character);
double calculate(double value1, double value2, operationType operation);
double parseEquation(char* inputEquation);
int getOperatorPrecedence(char operator);
int getOperatorAssociativeness(char operator);
int makePolishNotation(char* equation, char** outputString);
