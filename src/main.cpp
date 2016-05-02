// main.cpp

#include <Windows.h>


namespace {
    int mainLoop();
    LRESULT CALLBACK windowProcedure(HWND window, UINT message, WPARAM param1, LPARAM param2);

    HWND createWindow(HINSTANCE instanceHandle, int cmdShow, WNDPROC procedure);
}


int WINAPI WinMain(HINSTANCE const instanceHandle, HINSTANCE, LPSTR, int const cmdShow) {
    auto const window = createWindow(instanceHandle, cmdShow, windowProcedure);
    if (!window) {
        return 0;
    }

    return mainLoop();
}


namespace {
    int mainLoop() {
        MSG msg;

        for (;;) {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
                auto const result = GetMessage(&msg, nullptr, 0, 0);
                if (result == 0 || result == -1) {
                    break;
                }

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } else {
                Sleep(1);
            }
        }

        return static_cast<int>(msg.wParam);
    }

    LRESULT CALLBACK windowProcedure(HWND const window, UINT const message, WPARAM const param1, LPARAM const param2) {
        switch (message) {
            case WM_DESTROY:
                PostQuitMessage(0);
                break;

            default:
                return DefWindowProc(window, message, param1, param2);
        }

        return 0;
    }

    HWND createWindow(HINSTANCE const instanceHandle, int const cmdShow, WNDPROC const procedure) {
        WNDCLASSEX wc;
        wc.cbClsExtra = 0;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.cbWndExtra = 0;
        wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
        wc.hCursor = nullptr;
        wc.hIcon = reinterpret_cast<HICON>(LoadImage(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
        wc.hIconSm = reinterpret_cast<HICON>(LoadImage(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
        wc.hInstance = instanceHandle;
        wc.lpfnWndProc = procedure;
        wc.lpszClassName = "MainWindow";
        wc.lpszMenuName = nullptr;
        wc.style = 0;

        auto const atom = RegisterClassEx(&wc);
        if (!atom) {
            return nullptr;
        }

        SIZE size{320, 480};
        size.cx += GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
        size.cy += GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

        auto const window = CreateWindowEx(
                0,
                reinterpret_cast<LPCTSTR>(static_cast<INT_PTR>(LOWORD(atom))),
                "",
                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                (GetSystemMetrics(SM_CXSCREEN) - size.cx) / 2,
                (GetSystemMetrics(SM_CYSCREEN) - size.cy) / 2,
                size.cx,
                size.cy,
                nullptr,
                nullptr,
                instanceHandle,
                nullptr);

        if (!window) {
            return nullptr;
        }

        ShowWindow(window, cmdShow);
        UpdateWindow(window);

        return window;
    }
}
