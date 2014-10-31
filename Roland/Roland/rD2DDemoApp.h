/* rD2DDemoApp.h
 * Author: jmurphy
 * Created on Oct 30, 2014, 9:13 PM PST
 *
 */

#ifndef DIRECT2D_DEMO_APP_
#define DIRECT2D_DEMO_APP_

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

#endif // DIRECT2D_DEMO_APP_
