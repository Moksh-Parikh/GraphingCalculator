clayTheme currentTheme = {
    .buttonTheme = {
        // ebony
        .defaultColour = (Clay_Color) {0xa2, 0x88, 0xa6, 0xff},
        // lighter ebony
        .hoveredColour = (Clay_Color) {0xd6, 0xb4, 0xdB, 0xff},
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
        .boxColour = (Clay_Color) {0xf0, 0xeb, 0xd8, 0xff},
        .hoveredColour = (Clay_Color) {0x2d, 0x33, 0x41, 0xff},
        .borderColour = (Clay_Color) {0xb6, 0x24, 0x4f, 0xff}, 
        .textColour = (Clay_Color) {0x0d, 0x13, 0x21, 0xff},
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
    int stringLength = wcslen(inString);
    
    Clay_CustomElementData *textData = (Clay_CustomElementData *)malloc(sizeof(Clay_CustomElementData));
    if (textData == NULL) {
        printf("malloc() failed in %s, exiting\n", __func__);
        exit(1);
    }
    
    *textData = (Clay_CustomElementData){
        .type = CLAY_CUSTOM_WIDE_STRING,
        .text = (wideText) {
            .string = calloc(
                        (stringLength + 1) * sizeof(wchar_t),
                        sizeof(wchar_t)
                    ),
            .stringLength = wcslen(inString),
            .fontSize = stringData.fontSize,
            .fontId = stringData.fontId,
            .textColour = stringData.textColour,
        },
    };
    if (textData->text.string == NULL) {
        printf("calloc() failed in %s, exiting\n", __func__);
        exit(1);
    }

    wcsncpy(textData->text.string, inString, stringLength + 1);

    Clay_Dimensions textDimensions = Clay_Custom_Win32_MeasureWideText(textData->text, &stringData, fonts);

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


void customGraph() {//Clay_Dimensions dimensions) {
    Clay_CustomElementData *graphInfo = (Clay_CustomElementData*)malloc(
        sizeof(Clay_CustomElementData)
    );
    
    if (graphInfo == NULL) {
        printf("malloc() failed in %s, exiting\n", __func__);
        exit(1);
    }

    *graphInfo = (Clay_CustomElementData){
        .type = CLAY_CUSTOM_GRAPH,
        .graph = (graphData){
            .backgroundColour = (Clay_Color){0xff,0xff,0xff,0xff},
            .horizontalGridLines = 5,
            .verticalGridLines = 5,
        },
    };

    CLAY({
        .id = CLAY_ID("Graph"),
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0),
            },
        },
        .custom = {
            .customData = graphInfo,
        },
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

void clayButtonRow(int buttons, int rowNumber, char** textArray, buttonStyle styleArray[], Clay_BoundingBox containerSize) {
    CLAY({
        .id = CLAY_IDI("buttonRow", rowNumber),
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0),
            },
            .padding = {
                .top = containerSize.height / 70,
            },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
            .childGap = containerSize.height / 70,
        },
    }) {
        // ensures the rows all have a unique ID
        for (int i = 0; i < buttons; i++) {
            Clay_String textAsClayString;
            stringToClayString(textArray[i], &textAsClayString);
            clayButton(
                (buttonStyle) {
                    .id = CLAY_SID(textAsClayString),
                    .defaultColour = styleArray[i].defaultColour,
                    .hoveredColour = styleArray[i].hoveredColour,
                    .textColor = styleArray[i].textColor,
                    .fontSize = 16,
                    /* .width = dimensions.width / 4, */
                    /* .height = dimensions.height / 12, */
                    .text = textAsClayString,
                }
            );
        }
    }
}

Clay_RenderCommandArray createLayout(Clay_Dimensions dimensions, clayTheme layoutTheme, Clay_Dimensions buttonGridDimensions) {
    if (displayBuffers.top[0] == L'\0') {
        wcsncpy(displayBuffers.top, L" ", 2);
    }
    
    if (displayBuffers.bottom[0] == L'\0') {
        wcsncpy(displayBuffers.bottom, L" ", 2);
    }

    Clay_BeginLayout();

    CLAY({
        .id = CLAY_ID("OuterBody"),
        .backgroundColor = layoutTheme.mainColour,
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0),
            },
            .padding = CLAY_PADDING_ALL(dimensions.height / 30),
            .childAlignment = {
                .x = CLAY_ALIGN_Y_CENTER,
            },
            .childGap = dimensions.height / 30,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }
    }) {
        CLAY({
            .id = CLAY_ID("GraphAndEquationContainer"),
            // .backgroundColor = layoutTheme.headerTheme.boxColour,
            .layout = {
                .childAlignment = {
                    .x = CLAY_ALIGN_X_RIGHT,
                    .y = CLAY_ALIGN_Y_CENTER,
                },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = {
                    .width = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_FIXED(dimensions.height / 2),
                },
                .childGap = 4,
            },
        }) {
            // CLAY({
            //     .id = CLAY_ID("Graph"),
            //     .backgroundColor = (Clay_Color){0x0,0x0,0x0,0xff},
            //     .layout = {
            //         .sizing = {
            //             .width = CLAY_SIZING_GROW(0),
            //             .height = CLAY_SIZING_GROW(0),
            //         },
            //     },
            // }){
            customGraph();
             // (Clay_Dimensions){500, 500});
                 // }
            
            CLAY({
                .id = CLAY_ID("Equation"),
                .backgroundColor = layoutTheme.headerTheme.boxColour,
                // .border = {
                //     .width = {4,4,4,4,0},
                //     .color = layoutTheme.headerTheme.borderColour,
                // },
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_GROW(0),
                        // .height = CLAY_SIZING_GROW(0),
                    },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .childAlignment = {
                        .x = CLAY_ALIGN_X_RIGHT,
                        .y = CLAY_ALIGN_Y_CENTER,
                    },
                    .padding = CLAY_PADDING_ALL(dimensions.height / 40)
                },
            }){
                customWideText(displayBuffers.top,
                               (Clay_Custom_Wide_String_Style) {
                                    .fontId = FIRACODE,
                                    .fontSize = layoutTheme.headerTheme.fontSize,
                                    .textColour = layoutTheme.headerTheme.textColour,
                               });
                customWideText(displayBuffers.bottom,
                               (Clay_Custom_Wide_String_Style) {
                                    .fontId = FIRACODE,
                                    .fontSize = layoutTheme.headerTheme.fontSize,
                                    .textColour = layoutTheme.headerTheme.textColour,
                               });
            }
        }

        CLAY ({
                .id = CLAY_ID("buttonContainer"),
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_GROW(0),
                        .height = CLAY_SIZING_GROW(0),
                    },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .backgroundColor = currentTheme.mainColour,
        }) {
            buttonStyle styleArray[(uint16_t)buttonGridDimensions.width];
            for (int j = 0; j < (uint16_t)buttonGridDimensions.height; j++) {
                char* rowText[(uint16_t)buttonGridDimensions.width];

                for (int i = 0; i < (uint16_t)buttonGridDimensions.width; i++) {
                    styleArray[i] = (buttonStyle){
                        .defaultColour = layoutTheme.buttonTheme.defaultColour,
                        .hoveredColour = layoutTheme.buttonTheme.hoveredColour,
                        .textColor = layoutTheme.buttonTheme.textColor,
                        .fontSize = 16,
                    };

                    rowText[i] = 
                    j == 0 ? buttonText[i + j] :
                    buttonText[i + ((uint16_t)buttonGridDimensions.width * j)];
                }

                clayButtonRow(buttonGridDimensions.width,
                              j,
                              rowText,
                              styleArray,
                              Clay_GetElementData(
                                    Clay_GetElementId(CLAY_STRING("buttonContainer"))
                                    ).boundingBox
                              );
            }
        }
    }

    return Clay_EndLayout();
}
