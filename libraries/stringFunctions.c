void removeTrailingZeroes(double value, wchar_t* outputBuffer) {
    wchar_t* buffer = malloc(MAX_BUFFER_SIZE * sizeof(wchar_t) );
    if (buffer == NULL) {
        snwprintf(outputBuffer, MAX_BUFFER_SIZE, L"%ls", buffer);
        return;
    }
    snwprintf(buffer, MAX_BUFFER_SIZE, L"%lf", value);

    int i = wcslen(buffer) - 1;

    for (i; i >= 0; i--) {
        if (buffer[i] != L'0') break;
    }
    i += 1;
    if (buffer[i - 1] == L'.') i -= 1;

    snwprintf(outputBuffer, i + 1, L"%ls", buffer);
    free(buffer);
}

void checkAndClearBuffer(wchar_t** buffer, wchar_t* comparison) {
    if (!wcscmp(*buffer, comparison)) {
        for (int i = 0; i < wcslen(comparison); i++) {
            (*buffer)[i] = L'\0';
        }
    }
}


void clearBuffer(wchar_t* buffer) {
    int bufferSize = wcslen(buffer);

    for (int i = 0; i < bufferSize; i++) {
        buffer[i] = L'\0';
    }
}


void fillStringArray(char** inputArray,
                     char** outputArray,
                     int outputArraySize
) {
    if (inputArray == NULL || outputArray == NULL) {
        return;
    }

    for (int i = 0; i < outputArraySize; i++) {
        int sourceStringLength = strlen(inputArray[i]);

        outputArray[i] = calloc(
                            (sourceStringLength + 1) * sizeof(char),
                            sizeof(char)
                         );
        if (outputArray[i] == NULL) {
            printf("calloc() failed in %s, exiting\n", __func__);
            exit(1);
        }

        strncpy(outputArray[i],
                inputArray[i],
                sourceStringLength);
        outputArray[i][sourceStringLength] = '\0';
    }
}


void stringToClayString(char* inString, Clay_String* outString) {
    (*outString).isStaticallyAllocated = false;
    
    if (inString == NULL) {
        outString->chars = calloc(strlen("inString is NULL") + 1, sizeof(char));
        if (outString->chars == NULL) {
            printf("calloc() failed in %s, exiting\n", __func__);
            exit(1);
        }
        
        strncpy((char *)outString->chars, "inString is NULL\0", strlen("inString is NULL") + 1);
        outString->length = strlen(outString->chars);
        
        return;
    }
    
    if (inString[0] == '\0') {
        outString->chars = calloc(strlen("NULL") + 1, sizeof(char));
        if (outString->chars == NULL) {
            printf("calloc() failed in %s, exiting\n", __func__);
            exit(1);
        }
        
        strncpy((char *)outString->chars, "NULL\0", strlen("NULL") + 1);
        outString->length = strlen(outString->chars);
        
        return;
    }

    outString->chars = calloc(strlen(inString) + 1, sizeof(char));
    if (outString->chars == NULL) {
        printf("calloc() failed in %s, exiting\n", __func__);
        exit(1);
    }

    strncpy((char *)outString->chars, (char*)inString, strlen(inString));

    outString->length = strlen(outString->chars);
}

