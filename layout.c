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
} theme;

typedef struct {
    void* memory;
    int offset;
} Arena;

theme currentTheme = {
    .buttonTheme = {
        // ebony
        .defaultColour = (Clay_Color) {0x60, 0x69, 0x5c, 0xff},
        // lighter ebony
        .hoveredColour = (Clay_Color) {0x80, 0x89, 0x7c, 0xff},
        .textColor = (Clay_Color) {255, 255, 255, 255},
        .fontSize = 10,
    },
    .progressBarTheme = {
        // ebony
        .backgroundColour = (Clay_Color) {0x60, 0x69, 0x5c, 0xff},
        // white
        .barColour = (Clay_Color) {0xf0, 0xeb, 0xd8, 0xff},
    },
    .headerTheme = {
        .boxColour = (Clay_Color) {0x0d, 0x13, 0x21, 0xff},
        .hoveredColour = (Clay_Color) {0x2d, 0x33, 0x41, 0xff},
        .textColour = (Clay_Color) {0xf0, 0xeb, 0xd8, 0xff},
        .fontSize = 24,
        .font = FIRACODE,
    },
    // rich black
    .mainColour = (Clay_Color) {0x0d, 0x13, 0x21, 0xff},
    // eggshell
    .accentColour1 = (Clay_Color) {0xf0, 0xeb, 0xd8, 0xff},
    // ebony
    .accentColour2 = (Clay_Color) {0x60, 0x69, 0x5c, 0xff},
    // basically white
    .hoveredAccent1 = (Clay_Color) {0xff, 0xfb, 0xf8, 0xff},

    .hoveredAccent2 = (Clay_Color) {0x80, 0x89, 0x7c, 0xff},
};


void customWideText(wchar_t* inString, Clay_Custom_Wide_String_Style stringData) {
    Arena frameArena = (Arena) {
        .memory = malloc(
                    (wcslen(inString) + 1) * sizeof(wchar_t) +
                    sizeof(uint16_t) + // stringLength
                    sizeof(uint16_t) + // fontId
                    sizeof(uint16_t) + // fontSize
                    sizeof(Clay_Color) // textColour
        )
    };

    wideText *textData = (wideText *)(frameArena.memory + frameArena.offset);
    *textData = (wideText) {
                    .string = malloc(
                                (wcslen(inString) + 1) * sizeof(wchar_t)
                            ),
                    .stringLength = wcslen(inString),
                    .fontSize = stringData.fontSize,
                    .fontId = stringData.fontId,
                    .textColour = stringData.textColour,
                };
    // printf("malloced textData\n");
    wcsncpy(textData->string, inString, wcslen(inString) + 1);
    frameArena.offset += sizeof(wideText);

    
    Clay_Dimensions textDimensions = Clay_Custom_Win32_MeasureWideText(*textData, &stringData, fonts);

    CLAY({
        .layout = {
            .sizing = {
                .width = textDimensions.width,
                .height = textDimensions.height,
            },
        },
        .custom = {
            .customData = textData,
        }
    });
}


void clayButton(buttonStyle style) {
    CLAY({
            .id = style.id,
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_GROW(0), //style.height),
                },
                .childAlignment = {
                    .x = CLAY_ALIGN_X_CENTER,
                    .y = CLAY_ALIGN_Y_CENTER,
                },
            },
            .backgroundColor = Clay_Hovered() ? style.hoveredColour : style.defaultColour,
    }) {
        Clay_OnHover(handleButton, (intptr_t)NULL);
        CLAY_TEXT(
                    style.text,
                    CLAY_TEXT_CONFIG({
                        .textColor = style.textColor,
                        .fontSize = style.fontSize,
                        .fontId = ROBOTO,
                    })
        );
    }
}


void stringToClayString(char* inString, Clay_String* outString) {
    /* outString = malloc(sizeof(Clay_String)); */
    (*outString).isStaticallyAllocated = false;
    
    if (inString[0] == '\0') {
        outString->chars = calloc(strlen("NULL") + 1, sizeof(char));
        if (outString->chars == NULL) printf("heheh dickhead1\n");
        
        strncpy((char *)outString->chars, "NULL\0", strlen("NULL") + 1);
        outString->length = strlen(outString->chars);
        
        return;
    }

    outString->chars = calloc(strlen(inString) + 1, sizeof(char));
    if (outString->chars == NULL) printf("heheh dickhead1\n");

    strncpy((char *)outString->chars, (char*)inString, strlen(inString));

    outString->length = strlen(outString->chars);
}


void clayButtonRow(int buttons, int rowNumber, char** textArray, Clay_BoundingBox containerSize) {
    CLAY({
        .id = CLAY_IDI("buttonRow", rowNumber),
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0),
            },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
            .childGap = containerSize.height / 70,
        },
    }) {
        // ensures the rows all have a unique ID
        int iteratorOffset = rowNumber * 10;
        for (int i = 0; i < buttons; i++) {
            Clay_String textAsClayString;
            stringToClayString(textArray[i], &textAsClayString);
            clayButton(
                (buttonStyle) {
                    .id = CLAY_SID(textAsClayString),
                    .defaultColour = currentTheme.buttonTheme.defaultColour,
                    .hoveredColour = currentTheme.buttonTheme.hoveredColour,
                    .textColor = currentTheme.buttonTheme.textColor,
                    .fontSize = 16,
                    /* .width = dimensions.width / 4, */
                    /* .height = dimensions.height / 12, */
                    .text = textAsClayString,
                }
            );
        }
    }
}

Clay_RenderCommandArray createLayout(Clay_Dimensions dimensions) {
    if (displayBuffer[0] == L'\0') {
        wcsncpy(displayBuffer, L"NULL", 4);
    }

    Clay_BeginLayout();

    CLAY({
        .id = CLAY_ID("OuterBody"),
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0),
            },
            .padding = CLAY_PADDING_ALL(dimensions.height / 30),
            .childAlignment = {
                .x = CLAY_ALIGN_Y_CENTER,
            },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }
    }) {
        CLAY({
            .id = CLAY_ID("Header"),
            .backgroundColor = (Clay_Color){30, 100, 140, 255},
            .layout = {
                .childAlignment = {
                    .x = CLAY_ALIGN_X_RIGHT,
                    .y = CLAY_ALIGN_Y_CENTER,
                },
                .sizing = {
                    .width = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_FIXED(dimensions.height / 10),
                },
                .padding = CLAY_PADDING_ALL(dimensions.height / 40)
            },
        }) {
            customWideText(displayBuffer,
                           (Clay_Custom_Wide_String_Style) {
                                .fontId = FIRACODE,
                                .fontSize = 10,//dimensions.height / 10,
                                .textColour = 
                                (Clay_Color) {
                                    255,
                                    255,
                                    255,
                                    255
                                }
                           });
        }

        CLAY ({
                .id = CLAY_ID("buttonContainer"),
                .layout = {
                    .padding = CLAY_PADDING_ALL(dimensions.height / 40),
                    .sizing = {
                        .width = CLAY_SIZING_GROW(0),
                        .height = CLAY_SIZING_GROW(0),
                    },
                    .childGap = dimensions.height / 70,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .backgroundColor = currentTheme.mainColour,
        }) {
            for (int j = 0; j < (uint16_t)buttonGrid.height; j++) {
                char* rowText[(uint16_t)buttonGrid.width];

                for (int i = 0; i < (uint16_t)buttonGrid.width; i++) {
                    rowText[i] = 
                    j == 0 ? buttonText.arrayBottom[i + j] :
                    buttonText.arrayBottom[i + ((uint16_t)buttonGrid.width * j)];
                }

                clayButtonRow(buttonGrid.width, j, rowText, Clay_GetElementData(
                                    Clay_GetElementId(CLAY_STRING("buttonContainer"))
                                    ).boundingBox
                              );
            }
        }
    }

    return Clay_EndLayout();
}
