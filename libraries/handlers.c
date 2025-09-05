void handleButton(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) { 
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        wchar_t temp[100];
        // TODO: use mbstowcs_s
        // wouldn't compile with it :(
        mbstowcs(temp, elementId.stringId.chars, strlen(elementId.stringId.chars) + 1);

        if (!wcscmp(displayBuffers.top, L"NULL")) {
            displayBuffers.top[0] = L'\0';
        }

        if ( !strncmp(elementId.stringId.chars, CLEAR_CHARACTER, strlen(CLEAR_CHARACTER) ) ) {
            displayBuffers.top[0] = L'\0';
            return;
        }

        wcsncat(displayBuffers.top, temp, wcslen(temp));
        printf("Clicked: %s\n", elementId.stringId.chars);
    }
}

int16_t findWideCharacter(wchar_t* inputString,
                      wchar_t searchCharacter) {
    uint16_t inStringLength = wcslen(inputString);

    for (int i = 0; i < inStringLength; i++) {
        if (inputString[i] == searchCharacter) {
            return i;
        }
    }

    return -1;
}

operationType operationParser(wchar_t operationCharacter) {
    wprintf(L"char: %lc\n", operationCharacter);
    switch (operationCharacter) {
        case L'+':
            return ADDITION;
        
        case L'-':
            return SUBTRACTION;
        
        case L'*':
            return MULTIPLICATION;
        
        case L'/':
            return DIVISION;
        
        case L'^':
            return EXPONENT;
    }
}

char* wideStringToAnsi(wchar_t* wideStr) {
    size_t wideLen = wcslen(wideStr);
    
    char* ansiStr = (char*)malloc(wideLen + 1); // +1 for the null terminator
    if (ansiStr == NULL) {
        return NULL;
    }

    // Convert each wide character to a char
    for (size_t i = 0; i < wideLen; i++) {
        if (wideStr[i] > 255) {
            free(ansiStr);
            return NULL;
        }
        ansiStr[i] = (char)wideStr[i];
    }
    
    ansiStr[wideLen] = '\0';
    return ansiStr;
}

uint16_t inputHandler(wchar_t* inputBuffer, wchar_t* outputBuffer) {
    char *equationString = wideStringToAnsi(inputBuffer);

    double result = parseEquation(equationString);

    clearBuffer(outputBuffer);
    
    removeTrailingZeroes(result, outputBuffer);
    free(equationString);
    
    return 0;
}
