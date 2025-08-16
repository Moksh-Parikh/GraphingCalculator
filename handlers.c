void handleButton(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) { 
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        wchar_t temp[100];
        // TODO: use mbstowcs_s
        // wouldn't compile with it :(
        mbstowcs(temp, elementId.stringId.chars, strlen(elementId.stringId.chars) + 1);

        if (!wcscmp(displayBuffers.top, L"NULL")) {
            displayBuffers.top[0] = L'\0';
        }
        
        wcsncat(displayBuffers.top, temp, wcslen(temp));
        printf("Clicked: %s\n", elementId.stringId.chars);
    }
}

// void checkForWideCharacters(wchar_t* inputString,
//                             wchar_t* searchArray,
//                             occurrenceTable* outOccurrenceTable)
// {
//     uint16_t inStringLength = wcslen(inputString);
//     uint16_t searchArrayLength = wcslen(searchArray);

//     for (int i = 0; i < inStringLength; i++) {
//         for (int j = 0; j < searchArrayLength) {
//             if (inputString[i] == searchCharacter) {
//                 return i;
//             }
//         }
//     }
// }

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


double calculate(double number1, double number2, operationType operation) {
    printf("op: %d\n", operation);
    switch (operation) {
        case ADDITION:
            return number1 + number2;
        case SUBTRACTION:
            printf("result: %g\n", number1 - number2);
            return number1 - number2;
        case MULTIPLICATION:
            return number1 * number2;
        case DIVISION:
            return number1 / number2;
    }
}


uint16_t inputHandler(wchar_t* inputBuffer, wchar_t* outputBuffer) {
    wchar_t operations[5] = {
        L'+',
        L'-',
        L'*',
        L'/',
        L'^',
    };
    
    int index = 0;
    wchar_t* tempString;
    int bufferSize = wcslen(inputBuffer);
    double tempNumber, tempNumber2;
    operationType operation;

    for (int i = 0; i < 5; i++) {
        if ( ( index = findWideCharacter(inputBuffer, operations[i]) ) >= 0 ) {
            tempString = calloc(index * sizeof(wchar_t), sizeof(wchar_t));
            // don't need to allocate one more for NULL as index
            // is location of operation sign, which is excluded
            // from wcsncpy
            wcsncpy(tempString, inputBuffer, index);
            wprintf(L"index: %d\ntempString1: %ls\n", index, tempString);
            tempString[index] = L'\0';

            tempNumber = wcstold(tempString, NULL);

            free(tempString);
            tempString = calloc( (bufferSize - index + 1) * sizeof(wchar_t), sizeof(wchar_t) );

            wcsncpy(tempString, inputBuffer + index + 1, bufferSize - index);
            tempString[bufferSize - index] = L'\0';
            wprintf(L"tempString2: %ls\n", tempString);
            
            tempNumber2 = wcstold(tempString, NULL);
            operation = operationParser(inputBuffer[index]);

            if (operation == DIVISION && (tempNumber == 0 || tempNumber2 == 0) ) {
                snwprintf(outputBuffer, wcslen(L"Divide by zero") + 1, L"%ls", L"Divide by zero");
                free(tempString);
                break;
            }

            double result = calculate(tempNumber,                             
                      tempNumber2,
                      operation
                      );
            clearBuffer(outputBuffer);

            snwprintf(outputBuffer, MAX_BUFFER_SIZE, L"%lf", 
                   result
                   );
            free(tempString);
        }
    }

    return 0;
}
