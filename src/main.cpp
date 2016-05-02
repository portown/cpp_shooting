// main.cpp

#include <Windows.h>

#include "App.hpp"


int WINAPI WinMain(HINSTANCE const hInstance, HINSTANCE, LPSTR, int const nCmdShow) {
    auto app = shooting::ui::App::create(hInstance, nCmdShow);
    if (!app) {
        return 0;
    }

    return app->run();
}
