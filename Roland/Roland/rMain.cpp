/* rMain.cpp : Defines the entry point for the console application.
 * Author: jmurphy
 * Created: October 26, 2014, 10:58 PM PST
 *
 * Referenced from:
 * "Creating Win32 Applications (C++)" on MSDN (Oct 26, 14)
 * "Direct2D QuickStart" on MSDN (Oct 29, 14)
 *
 */

/*******************
 * Program Toggles *
 *******************/
//#define HELLO_WORLD
#define USE_DIRECT2D_DEMO_APP

/************
 * Includes *
*************/
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#include <d2d1.h>           // Direct2D for Win7 + earlier

#ifdef USE_DIRECT2D_DEMO_APP
#include "rD2DDemoApp.h"  // Used for Direct2D demo application
#endif

/********************
 * Global Variables *
 ********************/
// The main window class name.
static TCHAR szWindowClass[] = _T("Win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow);


/*********************
 * Run-time Routines *
 *********************/
int _tmain(int argc, _TCHAR* argv[])
{
    return 0;
}


/* WinMain is the main function necessary for any Win32-based application.
 *      This function is analogous to the main function in any C/C++ project.
 */
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
#ifdef USE_DIRECT2D_DEMO_APP
    /* Use HeapSetInformation to specify that the process should terminate
     * if the heap manager detects an error in any heap used by the process.
     * The return value is ignored, because we want to continue running
     * in the unlikely event that HeapSetInformation fails.
     */
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    if(SUCCEEDED(CoInitialize(NULL)))
    {
        {
            DemoApp app;

            if (SUCCEEDED(app.Initialize()))
            {
                app.RunMessageLoop();
            }
        }
        CoUninitialize();
    }

    return 0;

#else
    // This structure contains information about the window we'll be creating.
	WNDCLASSEX wcex;

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    // Now that we've created a window class, we must register it.
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 GuidedTour"),
            NULL);
    }

    hInst = hInstance; // Store instance handle in our global variable

    /* The parameters to CreateWindow Explained:
     * szWindowClass: the name of the application
     * szTitle: the text that appears in the title bar
     * WS_OVERLAPPEDWINDOW: the type of window to create
     * CW_USEDEFAULT, CW_USEDEFAULT: initial poition (x,y)
     * 500, 100: initial size (width, length)
     * NULL: the parent of this window
     * NULL: this application does not have a menu bar
     * hInstance: the first parameter from WinMain
     * NULL: not used in this application
     *
     * This function creates a handle to the window that we're creating.
     */
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    /* The parameters to ShowWindow explained:
     * hWnd: the value returned from CreateWindow
     * nCmdShow: the fourth parameter from WinMain
     *
     * Now, we display the window.
     */
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    // Add a message loop to listen for messages that the OS sends.
    MSG msg;
    BOOL bRet;
    while ( (bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            MessageBox(NULL,
            _T("An error occurred when receiving an outside message."),
            _T("Win32 Guided Tour"),
            NULL);
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
#endif
}

/*
 *  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
 *
 *  PURPOSE:  Processes messages for the main window.
 *
 *  WM_PAINT    - Paint the main window
 *  WM_DESTROY  - Post a quit message and return
 *
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef HELLO_WORLD
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, World!");
#endif

    /* Example Direct 2D application - Drawing a simple rectangle
        * Reference: "Direct2D QuickStart" on MSDN
        */
    HRESULT hr;
    ID2D1Factory * pD2DFactory = NULL;
    ID2D1HwndRenderTarget * pRT = NULL;
    ID2D1SolidColorBrush * pBlackBrush = NULL;

    switch (message)
    {
    case WM_PAINT:
        
        // Create a Direct2D Factory, which is used to create Direct2D resources
        hr = D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &pD2DFactory
            );

        // Obtain the size of the drawing area
        RECT rc;
        GetClientRect(hWnd, &rc);

        // Create a Direct2D render target (pRT - "pointer to Render Target")
        hr = pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
                hWnd,
                D2D1::SizeU( (rc.right - rc.left), (rc.bottom - rc.top) )
            ),
            &pRT
        );

        // Create a brush, using our render target
        if (SUCCEEDED(hr))
        {
            pRT->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::PaleVioletRed),
                &pBlackBrush);
        }

        // Draw the rectangle
        pRT->BeginDraw();

        pRT->DrawRectangle(
            D2D1::RectF(
                rc.left   + 100.0f,
                rc.top    + 100.0f,
                rc.right  - 100.0f,
                rc.bottom - 100.0f),
            pBlackBrush);

        hr = pRT->EndDraw();

        // Release resources
        if (pBlackBrush)
        {
            pBlackBrush->Release();
        }
        if (pD2DFactory)
        {
            pD2DFactory->Release();
        }

#ifdef HELLO_WORLD
        /* Win32 application tour */
        hdc = BeginPaint(hWnd, &ps);

        /* Here, your application is laid out.
         * For this introduction, we just print out "Hello, World!"
         * in the top left corner.
         */
        TextOut(
            hdc, 
            rc.left + ((rc.right - rc.left) / 2.0f) - 50.0f,
            rc.top  + ((rc.bottom - rc.top) / 2.0f),
            greeting, 
            _tcslen(greeting)
            );
        // End application specific layout section.

        EndPaint(hWnd, &ps);
#endif
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}
