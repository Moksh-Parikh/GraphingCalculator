void handleButton(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) { 
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        wchar_t temp[100];
        // TODO: use mbstowcs_s
        // wouldn't compile with it :(
        mbstowcs(temp, elementId.stringId.chars, strlen(elementId.stringId.chars) + 1);

        if (!wcscmp(displayBuffer, L"NULL")) {
            displayBuffer[0] = L'\0';
        }
        
        wcsncat(displayBuffer, temp, wcslen(temp));
        printf("Clicked: %s\n", elementId.stringId.chars);
    }
}
