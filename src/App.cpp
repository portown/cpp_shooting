// App.cpp

#include "App.hpp"

#include <functional>
#include <utility>


namespace ns = shooting::ui;


namespace std {
    using experimental::nullopt;
}


auto ns::App::create(
        HINSTANCE const hInstance,
        int const nCmdShow) -> std::optional<App> {
    if (FAILED(CoInitialize(nullptr))) {
        return std::nullopt;
    }

    App app{hInstance, nCmdShow};

    ID2D1Factory* factoryPtr;
    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factoryPtr))) {
        return std::nullopt;
    }
    app.direct2dFactory_.reset(factoryPtr, [](ID2D1Factory* p) { p->Release(); });

    app.view_ = GameView::create(hInstance, nCmdShow, app.direct2dFactory_);

    return std::move(app);
}

ns::App::App(HINSTANCE const hInstance, int const nCmdShow)
        : hInstance_{hInstance}, nCmdShow_{nCmdShow} {
}

ns::App::~App() {
    CoUninitialize();
}

auto ns::App::run() -> int {
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
