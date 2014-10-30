/* rD2DDemoApp.cpp
 * Author: jmurphy
 * Created on Oct 29, 2014, 7:28 PM PST
 *
 * Referenced from:
 * "Creating a Simple Direct2D Application" on MSDN (Oct 29, 14)
 *
 */

// Windows Header Files:
#include <Windows.h>

// C Runtime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

// Direct 2D Header Files:
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

/*********************
 * Utility Functions *
 *********************/
template <class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease)->NULL;
    }
}

#ifndef Assert
#if defined(DEBUG) || defined(_DEBUG)
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

/**********
* Classes *
***********/
class DemoApp
{
public:
    DemoApp();  // Constructor
    ~DemoApp(); // Destructor

    // Register the window class and call methods for instantiating drawing resources
    HRESULT Initialize();

    // Process and dispatch messages
    void RunMessageLoop();

private:
    // Initialize device-independent resources.
    HRESULT CreateDeviceIndependentResources();

    // Initialize device-dependent resources.
    HRESULT CreateDeviceDependentResources();

    // Release device-dependent resources.
    void DiscardDeviceResources();

    // Draw content
    HRESULT OnRender();

    // Resize the render target
    void OnResize(UINT width, UINT height);

    // The windows procedure
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;
    ID2D1Factory* m_pDirect2dFactory;
    ID2D1HwndRenderTarget * m_pRenderTarget;
    ID2D1SolidColorBrush * m_pLightSlateGrayBrush;
    ID2D1SolidColorBrush * m_pCornflowerBlueBrush;
};

/* Class Constructor */
DemoApp::DemoApp() :
m_hwnd(NULL),
m_pDirect2dFactory(NULL),
m_pRenderTarget(NULL),
m_pLightSlateGrayBrush(NULL),
m_pCornflowerBlueBrush(NULL)
{
}

/* Class Destructor */
DemoApp::~DemoApp()
{
    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pLightSlateGrayBrush);
    SafeRelease(&m_pCornflowerBlueBrush);
}

/********************
* Run-Time Routines *
*********************/
void DemoApp::RunMessageLoop()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HRESULT DemoApp::Initialize()
{
    HRESULT hr;

    // Initialize device-independent resouces, such as the Direct2D factory.
    hr = CreateDeviceIndependentResources();

    if (SUCCEEDED(hr))
    {
        // Register the window class.
        WNDCLASSEX wcex     = { sizeof(WNDCLASSEX) };
        wcex.style          = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc    = DemoApp::WndProc;
        wcex.cbClsExtra     = 0;
        wcex.cbWndExtra     = sizeof(LONG_PTR);
        wcex.hInstance      = HINST_THISCOMPONENT;
        wcex.hbrBackground  = NULL;
        wcex.lpszMenuName   = NULL;
        wcex.hCursor        = LoadCursor(NULL, IDI_APPLICATION);
        wcex.lpszClassName  = L"D2DDemoApp";

        RegisterClass(&wcex);

        // Because the CreateWindow function takes its size in pixels,
        // obtain the system DPI and use it to scale the window size.
        FLOAT dpiX, dpiY;

        // The factory returns the current system DPI. This is also the value
        // it will use to create its own windows.
        m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

        // Create the window.
        m_hwnd = CreateWindow(
            L"D2DDemoApp",
            L"Direct2D Demo App",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
            static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
            NULL,
            NULL,
            HINST_THISCOMPONENT,
            this
            );
        hr = m_hwnd ? S_OK : E_FAIL;
        if (SUCCEEDED(hr))
        {
            ShowWindow(m_hwnd, SW_SHOWNORMAL);
            UpdateWindow(m_hwnd);
        }
    }

    return hr;
}