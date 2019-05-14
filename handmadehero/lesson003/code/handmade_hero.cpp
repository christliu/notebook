#include <windows.h>

#define internal static
#define global_variable static

static bool Running = true;

global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
global_variable HBITMAP BitmapHandle;
global_variable HDC BitmapDeviceContext;


internal void
Win32ResizeDIBSection(int width, int height)
{
    if(BitmapHandle)
    {
        DeleteObject(BitmapHandle);
    }

    if(!BitmapDeviceContext)
    {
        BitmapDeviceContext = CreateCompatibleDC(0);
    }

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = width;
    BitmapInfo.bmiHeader.biHeight = height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;
    
    BitmapHandle = CreateDIBSection(
            BitmapDeviceContext, &BitmapInfo,
            DIB_RGB_COLORS,
            &BitmapMemory,
            0, 0);

}

internal void 
Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{
    StretchDIBits(DeviceContext,
            X, Y, Width, Height,
            X, Y, Width, Height,
            BitmapMemory,
            &BitmapInfo,
            DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK
WindowProc(HWND hwnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam)
{
    LRESULT Result = 0;
    switch(uMsg)
    {
        case WM_SIZE:
        {
            OutputDebugString("WM_SIZE\n");
            RECT ClientRect;
            GetClientRect(hwnd, &ClientRect);
            int Width = ClientRect.right - ClientRect.left;
            int Height = ClientRect.bottom - ClientRect.top;
            Win32ResizeDIBSection(Width, Height);
        }break;
        case WM_CLOSE:
        {
            OutputDebugString("WM_Close\n");
            Running = false;
        }break;
        case WM_DESTROY:
        {
            OutputDebugString("WM_Destroy\n");
            Running = false;
        }break;
        case WM_ACTIVATEAPP:
        {
            OutputDebugString("WM_ACTIVATEAPP\n");
        }break;
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(hwnd, &Paint);
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
            EndPaint(hwnd, &Paint);
        }break;
        default:
        {
            Result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }break;
    }
    return Result;

}

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = WindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = hInstance;
    WindowClass.lpszMenuName = 0;
    WindowClass.lpszClassName = "HandmadeHeroClass";

    if(RegisterClass(&WindowClass))
    {
        HWND WindowHandle = 
            CreateWindowEx(
                    0, 
                    WindowClass.lpszClassName,
                    "Handmade Hero",
                    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    0,
                    0,
                    hInstance,
                    0);
        if(WindowHandle)
        {
            MSG Message;
            while(Running)
            {
                bool MessageResult = GetMessage(&Message, 0, 0, 0);
                if(MessageResult > 0)
                {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            // TODO: failure
        }
    }
    else
    {
        // TODO: failure
    };
    return 0;
}
