// GameView.cpp

#include "GameView.hpp"


namespace ns = shooting::ui;


auto ns::GameView::create(
        HINSTANCE const hInstance,
        int const nCmdShow,
        std::shared_ptr<ID2D1Factory> const direct2dFactory) -> std::shared_ptr<GameView> {
    WNDCLASSEX wc;
    wc.cbClsExtra = 0;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = sizeof(LONG_PTR);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wc.hCursor = nullptr;
    wc.hIcon = reinterpret_cast<HICON>(LoadImage(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
    wc.hIconSm = reinterpret_cast<HICON>(LoadImage(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
    wc.hInstance = hInstance;
    wc.lpfnWndProc = windowProcedure;
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

    struct GameViewImpl : public GameView {};   // for make_shared
    auto const view = std::make_shared<GameViewImpl>();
    view->direct2dFactory_ = direct2dFactory;

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
            hInstance,
            view.get());

    if (!window) {
        return nullptr;
    }

    ShowWindow(window, nCmdShow);
    UpdateWindow(window);

    return view;
}

LRESULT CALLBACK ns::GameView::windowProcedure(
        HWND const hWnd,
        UINT const message,
        WPARAM const wParam,
        LPARAM const lParam) {
    if (message == WM_CREATE) {
        auto const view = reinterpret_cast<GameView*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(view));
        view->hWnd_ = hWnd;
        ShowCursor(FALSE);
        return 0;
    }

    auto const view = reinterpret_cast<GameView*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (!view) {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    switch (message) {
        case WM_DISPLAYCHANGE:
            InvalidateRect(hWnd, nullptr, FALSE);
            break;

        case WM_PAINT:
            view->onPaint();
            ValidateRect(hWnd, nullptr);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void ns::GameView::onPaint() {
    if (!renderTarget_) {
        RECT rect;
        GetClientRect(hWnd_, &rect);

        auto const size = D2D1::SizeU(
                rect.right - rect.left,
                rect.bottom - rect.top);

        ID2D1HwndRenderTarget* renderTarget;
        auto const result = direct2dFactory_->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(hWnd_, size),
                &renderTarget);
        if (FAILED(result)) {
            return;
        }
        renderTarget_.reset(renderTarget, [](ID2D1HwndRenderTarget* p) { p->Release(); });
    }

    renderTarget_->BeginDraw();

    renderTarget_->SetTransform(D2D1::Matrix3x2F::Identity());
    renderTarget_->Clear(D2D1::ColorF(D2D1::ColorF::White));

    auto const result = renderTarget_->EndDraw();
    if (FAILED(result)) {
        renderTarget_.reset();
    }
}
