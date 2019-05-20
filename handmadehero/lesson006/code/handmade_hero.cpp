#include <windows.h>
#include <stdint.h>
#include <xinput.h>

#define internal static
#define global_variable static

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;


#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
    return 0;
}
global_variable x_input_get_state *XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_

#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration) 
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub)
{
    return 0;
}
global_variable x_input_set_state *XInputSetState_ = XInputSetStateStub;
#define XInputSetState XInputSetState_

internal void
Win32LoadXInput(void)
{
    HMODULE XInputLibrary = LoadLibrary("xinput1_3.dll");
    if(XInputLibrary)
    {
        XInputGetState_ = (x_input_get_state *)GetProcAddress(XInputLibrary, "XInputGetState");
        XInputSetState_ = (x_input_set_state *)GetProcAddress(XInputLibrary, "XInputSetState");
    }
    else
    {
    }
}

static bool Running = true;

struct win32_offscreen_buffer
{
    BITMAPINFO Info;
    void *Memory;

    int Height;
    int Width;
    int BytesPerPixel = 4;
};

struct win32_window_dimension
{
    int Width;
    int Height;
};

global_variable win32_offscreen_buffer BackBuffer;

internal win32_window_dimension
GetWindowDimension(HWND window)
{
    RECT ClientRect;
    win32_window_dimension Result;
    GetClientRect(window, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;
    return Result;
}

internal void
RenderWeirdGradient(win32_offscreen_buffer Buffer, int xoffset, int yoffset)
{
    int Width = Buffer.Width;
    int Height = Buffer.Height;
    uint8* Row = (uint8*)Buffer.Memory; 
    for(int Y = 0; Y < Height; ++Y)
    {
        uint32* Pixel = (uint32*)Row;
        for(int X = 0; X < Width; ++X)
        {
            uint8 Red = 0;
            uint8 Green = Y + yoffset;
            uint8 Blue = X + xoffset;

            *Pixel = Blue << 8 | Green;
            Pixel ++;
            
        }
        Row += Width * Buffer.BytesPerPixel;
    }
}


internal void
Win32ResizeDIBSection(win32_offscreen_buffer* Buffer, int width, int height)
{
    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = width;
    Buffer->Height = height;

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;
    
    int BitmapMemorySize = (Buffer->Width * Buffer->Height) * Buffer->BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    RenderWeirdGradient(*Buffer, 0, 0);

}

internal void 
Win32CopyBufferToWindow(win32_offscreen_buffer Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight, int X, int Y, int Width, int Height) 
{
    StretchDIBits(DeviceContext,
            0, 0, WindowWidth, WindowHeight,
            0, 0, Buffer.Width, Buffer.Height,
            /*
            X, Y, Width, Height,
            X, Y, Width, Height,
            */
            Buffer.Memory,
            &Buffer.Info,
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
            //win32_window_dimension Dimension = GetWindowDimension(hwnd);
            //Win32ResizeDIBSection(&BackBuffer, Dimension.Width, Dimension.Height);
        }break;
        case WM_CLOSE:
        {
            OutputDebugString("WM_Close\n");
            Running = false;
        }break;
        
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            OutputDebugString("KEY UP\n");
            uint32 VKCODE = wParam;
            bool WasDown = ((lParam & (1 << 30)) !=0);
            bool IsDown = ((lParam & (1 << 31)) !=0);

            if(WasDown != IsDown)
            {
                if(VKCODE == 'W')
                {
                }
                else if(VKCODE == 'A')
                {
                    OutputDebugString("A: ");
                    if(IsDown)
                    {
                        OutputDebugString("IsDown");
                    }
                    if(WasDown)
                    {
                        OutputDebugString("WasDown");
                    }
                }
                else if(VKCODE == 'S')
                {
                }
                else if(VKCODE == 'D')
                {
                }
                else if(VKCODE == 'Q')
                {
                }
                else if(VKCODE == 'E')
                {
                }
                else if(VKCODE == VK_UP)
                {
                }
                else if(VKCODE == VK_DOWN)
                {
                }
                else if(VKCODE == VK_LEFT)
                {
                }
                else if(VKCODE == VK_RIGHT)
                {
                }
                else if(VKCODE == VK_ESCAPE)
                {
                    OutputDebugString("ESCAPE: ");
                    if(IsDown)
                    {
                        OutputDebugString("IsDown");
                    }
                    if(WasDown)
                    {
                        OutputDebugString("WasDown");
                    }
                }
            }

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
            win32_window_dimension Dimension = GetWindowDimension(hwnd);
            Win32CopyBufferToWindow(BackBuffer, DeviceContext, Dimension.Width, Dimension.Height, X, Y, Width, Height);
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

    Win32LoadXInput(); 

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

            int xoffset = 0;
            int yoffset = 0;
            Win32ResizeDIBSection(&BackBuffer, 1200, 900);
            while(Running)
            {
                bool MessageResult = PeekMessage(&Message, WindowHandle, 0, 0, PM_REMOVE);
                while(PeekMessage(&Message, WindowHandle, 0, 0, PM_REMOVE))
                {
                    if(Message.message == WM_QUIT)
                    {
                        Running = false;
                    }

                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
                for (DWORD i=0; i< XUSER_MAX_COUNT; i++ )
                {
                    XINPUT_STATE state;
                    ZeroMemory( &state, sizeof(XINPUT_STATE) );

                    // Simply get the state of the controller from XInput.
                    DWORD dwResult = XInputGetState( i, &state );

                    if( dwResult == ERROR_SUCCESS )
                    {
                        // Controller is connected 
                        XINPUT_GAMEPAD *Pad = &state.Gamepad;

                        bool Up = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
                        bool Down = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
                        bool Left = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
                        bool Right = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
                        bool Start = (Pad->wButtons & XINPUT_GAMEPAD_START);
                        bool Back = (Pad->wButtons & XINPUT_GAMEPAD_BACK);
                        bool LeftShoulder = (Pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
                        bool RightShoulder = (Pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
                        bool AButton = (Pad->wButtons & XINPUT_GAMEPAD_A);
                        bool BButton = (Pad->wButtons & XINPUT_GAMEPAD_B);
                        bool XButton = (Pad->wButtons & XINPUT_GAMEPAD_X);
                        bool YButton = (Pad->wButtons & XINPUT_GAMEPAD_Y);

                        if(AButton)
                        {
                            yoffset += 2;
                            XINPUT_VIBRATION Vibration;
                            Vibration.wLeftMotorSpeed = 65535;
                            Vibration.wRightMotorSpeed = 65535;
                            XInputSetState(i, &Vibration);
                        }
                    }
                    else
                    {
                        // Controller is not connected 
                    }
                }

                RenderWeirdGradient(BackBuffer, xoffset, yoffset);
                HDC DeviceContext = GetDC(WindowHandle);
                win32_window_dimension Dimension = GetWindowDimension(WindowHandle);
                Win32CopyBufferToWindow(BackBuffer, DeviceContext, Dimension.Width, Dimension.Height, 0, 0, 
                        Dimension.Width, Dimension.Height);

                ++xoffset;
                //++yoffset;
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
