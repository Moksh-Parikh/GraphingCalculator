// Are global variables bad?
// I dont like them so I might
// try getting rid of them

#include "headers/graphingCalculator.h"

#include "renderer/clay_renderer_gdi.c"

char** buttonText;

// defines the number of buttons on each axis 
Clay_Dimensions buttonGrid = {
    .width = 5,
    .height = 4,
};

HFONT fonts[3];

outputBuffers displayBuffers;

#include "handlers.c"
#include "layout.c"
#include "stringFunctions.c"

#define APPNAME "Calculator"
char szAppName[] = APPNAME; // The name of this application
char szTitle[] = APPNAME;   // The title bar text

long lastMsgTime = 0;
bool ui_debug_mode;
bool shiftDown = false;

#ifndef RECTWIDTH
#define RECTWIDTH(rc)   ((rc).right - (rc).left)
#endif
#ifndef RECTHEIGHT
#define RECTHEIGHT(rc)  ((rc).bottom - (rc).top)
#endif

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char *buttonTextOptions[MAXIMUM_BUTTONS] =
    {
        "C",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "*",
        "+",
        "-",
        "/",
        "=",
        "^",
        ":",
        ":-",
        ":--",
        ":---",
        ":----",
        ":-----",
        "M",
        "M+",
        "M-",
        "(",
        ")",
        "[",
        "]",
        "Differentiate",
        "Integral",
        "Approximate",
        "{",
        "}",
        ">",
        "<",
    };

    switch (message)
    {

    // ----------------------- first and last
    case WM_CREATE:
        CenterWindow(hwnd);
        // SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)"hi");//(LONG_PTR)(&john) );
        break;

    case WM_DESTROY:
        // printf("%s\n", (char *)GetWindowLongPtr(hwnd, GWLP_USERDATA) );
        PostQuitMessage(0);
        break;

    case WM_MOUSEWHEEL: // scrolling data
    {
        short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        // todo: i think GetMessageTime can roll over, so something like if(lastmsgtime > now) ... may be needed
        long now = GetMessageTime();
        float dt = (now - lastMsgTime) / 1000.00;

        lastMsgTime = now;

        // little hacky hack to make scrolling *feel* right
        if (abs(zDelta) > 100)
        {
            zDelta = zDelta * .012;
        }

        Clay_UpdateScrollContainers(true, (Clay_Vector2){.x = 0, .y = zDelta}, dt);

        InvalidateRect(hwnd, NULL, false); // force a wm_paint event
        break;
    }
    case WM_RBUTTONUP:
    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MOUSEMOVE: // mouse events
    {
        short mouseX = GET_X_LPARAM(lParam);
        short mouseY = GET_Y_LPARAM(lParam);
        short mouseButtons = LOWORD(wParam);

        Clay_SetPointerState((Clay_Vector2){mouseX, mouseY}, mouseButtons & 0b01);

        InvalidateRect(hwnd, NULL, false); // force a wm_paint event
        break;
    }

    case WM_SIZE: // resize events
    {
        RECT r = {0};
        Clay_Dimensions dim;

        if (GetClientRect(hwnd, &r)) {
            dim = (Clay_Dimensions){.height = r.bottom - r.top, .width = r.right - r.left};
            Clay_SetLayoutDimensions(dim);
        }
        
        buttonGrid.width = dim.width / 100;
        buttonGrid.height = dim.height / 100;

        if (buttonGrid.height * buttonGrid.width > MAXIMUM_BUTTONS) {
            for (int i = 0; buttonGrid.height * buttonGrid.width > MAXIMUM_BUTTONS; i++) {
                if (i % 2 == 0) {buttonGrid.height -= 1;}
                else {buttonGrid.width -= 1;}
            }
        }

        fillStringArray(buttonTextOptions, buttonText, buttonGrid.width * buttonGrid.height);

        InvalidateRect(hwnd, NULL, false); // force a wm_paint event

        break;
    }

    case WM_CHAR:
    {
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hwnd);
            break;
        }
        
        checkAndClearBuffer(&displayBuffers.top, L" ");
        checkAndClearBuffer(&displayBuffers.bottom, L" ");
        
        if (wParam == 0x08) { // backspace
            displayBuffers.top[wcslen(displayBuffers.top) - 1] = L'\0';
            wprintf(L"%ls\n", displayBuffers.top);
            InvalidateRect(hwnd, NULL, false);
            break;
        }
        
        if (wParam == '\n' || wParam == '\r') {
            inputHandler(displayBuffers.top,
                         displayBuffers.bottom
                        );
            wprintf(L"%ls\n", displayBuffers.top);
            InvalidateRect(hwnd, NULL, false);
            break;
        }

        wchar_t convertedCharacter;
        // TODO: use mbstowcs_s
        // wouldn't compile with it :(
        mbtowc(&convertedCharacter, (char *)&(wParam), 1);
        displayBuffers.top[wcslen(displayBuffers.top)] = convertedCharacter;
        displayBuffers.top[wcslen(displayBuffers.top) + 1] = L'\0';

        wprintf(L"%ls\n", displayBuffers.top);
        
        InvalidateRect(hwnd, NULL, false);
        break;
    }

    case WM_KEYDOWN:
    {
        switch (wParam) {
            case VK_SHIFT:
                shiftDown = true;
                break;
            
            case VK_F12:
                Clay_SetDebugModeEnabled(ui_debug_mode = !ui_debug_mode);
                InvalidateRect(hwnd, NULL, false); // force a wm_paint event
                break;
        }
        break;
    }

    case WM_KEYUP:
    {
        switch (wParam) {
            case VK_SHIFT:
                shiftDown = false;
                break;
        }
    }

    // ----------------------- render
    case WM_PAINT:
    {
        RECT windowRect;
        GetWindowRect(hwnd, &windowRect);

        Clay_RenderCommandArray renderCommands = createLayout(
                    (Clay_Dimensions) {
                        windowRect.right - windowRect.left,
                        windowRect.bottom - windowRect.top
                    },
                    currentTheme,
                    buttonGrid
        );
        
                Clay_Win32_Render(hwnd, renderCommands, fonts);
        break;
    }

    // ----------------------- let windows do all other stuff
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

bool didAllocConsole = false;


void HandleClayErrors(Clay_ErrorData errorData)
{
    if (!didAllocConsole)
    {
        didAllocConsole = AllocConsole();
    }

    printf("Handle Clay Errors: %s\r\n", errorData.errorText.chars);
}


int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    MSG msg;
    WNDCLASS wc;
    HWND hwnd;

    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(clayRequiredMemory, malloc(clayRequiredMemory) );
    Clay_Initialize(clayMemory,
                    (Clay_Dimensions)
                        {.width = 800,
                        .height = 600},
                    (Clay_ErrorHandler){
                    HandleClayErrors}
    );


    buttonText = (char **)calloc(
                (uint16_t)buttonGrid.width *
                (uint16_t)buttonGrid.height *
                sizeof(char *), sizeof(char *)
    );
    if (buttonText == NULL) {
        printf("calloc() failed in %s, exiting\n", __func__);
        exit(1);
    }

    displayBuffers.top = calloc(
                    MAX_BUFFER_SIZE *
                    sizeof(wchar_t),
                    sizeof(wchar_t)
    );
    if (displayBuffers.top == NULL) {
        printf("calloc() failed in %s, exiting\n", __func__);
        exit(1);
    }
    
    displayBuffers.bottom = calloc(
                    MAX_BUFFER_SIZE *
                    sizeof(wchar_t),
                    sizeof(wchar_t)
    );
    if (displayBuffers.bottom == NULL) {
        printf("calloc() failed in %s, exiting\n", __func__);
        exit(1);
    }

    Clay_Win32_SetRendererFlags(CLAYGDI_RF_ALPHABLEND | CLAYGDI_RF_SMOOTHCORNERS);

    fonts[0] = Clay_Win32_SimpleCreateFont("resources/FiraCode-Regular.ttf", "FiraCode", 20, FW_NORMAL);
    fonts[1] = Clay_Win32_SimpleCreateFont("resources/Roboto-Regular.ttf", "Roboto", 20, FW_NORMAL);
    fonts[2] = Clay_Win32_SimpleCreateFont("resources/wingdings.ttf", "wingdings", 20, FW_NORMAL);

    Clay_SetMeasureTextFunction(Clay_Win32_MeasureText, fonts);

    // Clay_Win32_WndProc_Data john = {
    //     .buttonTextArray = buttonText,
    //     .buttonGridDimensions = buttonGrid,
    //     .fontArray = fonts,
    //     .fontArraySize = 3,
    //     .displayBuffers = displayBuffers,
    // };

    ZeroMemory(&wc, sizeof wc);
    wc.hInstance = hInstance;
    wc.lpszClassName = szAppName;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (FALSE == RegisterClass(&wc))
        return 0;

    // Calculate window rectangle by given client size
    // TODO: AdjustWindowRectExForDpi for DPI support
    RECT rcWindow = { .right = 450, .bottom = 600 };
    AdjustWindowRectExForDpi(&rcWindow, WS_OVERLAPPEDWINDOW, FALSE, 0, 157);//GetDpiForWindow(hwnd) );

    hwnd = CreateWindow(
        szAppName,
        szTitle,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        RECTWIDTH(rcWindow), // CW_USEDEFAULT,
        RECTHEIGHT(rcWindow), // CW_USEDEFAULT,
        0,
        0,
        hInstance,
        0);

    if (hwnd == NULL)
        return 0;

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    free(displayBuffers.top);
    free(displayBuffers.bottom);

    for (int i = 0; i < buttonGrid.width * buttonGrid.height; i++) {
        free(buttonText[i]);
    }

    free(buttonText);

    return msg.wParam;
}

void CenterWindow(HWND hwnd_self)
{
    HWND hwnd_parent;
    RECT rw_self, rc_parent, rw_parent;
    int xpos, ypos;

    hwnd_parent = GetParent(hwnd_self);
    if (NULL == hwnd_parent)
        hwnd_parent = GetDesktopWindow();

    GetWindowRect(hwnd_parent, &rw_parent);
    GetClientRect(hwnd_parent, &rc_parent);
    GetWindowRect(hwnd_self, &rw_self);

    xpos = rw_parent.left + (rc_parent.right + rw_self.left - rw_self.right) / 2;
    ypos = rw_parent.top + (rc_parent.bottom + rw_self.top - rw_self.bottom) / 2;

    SetWindowPos(
        hwnd_self, NULL,
        xpos, ypos, 0, 0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
