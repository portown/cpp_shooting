// GameView.hpp

#pragma once

#include <windows.h>
#include <d2d1.h>

#include <memory>


namespace shooting {
    namespace ui {
        class GameView {
        public:
            static auto create(
                    HINSTANCE hInstance,
                    int nCmdShow,
                    std::shared_ptr<ID2D1Factory> direct2dFactory) -> std::shared_ptr<GameView>;

        private:
            explicit constexpr GameView() = default;
            static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
            void onPaint();

            HWND hWnd_{};
            std::shared_ptr<ID2D1Factory> direct2dFactory_;
            std::shared_ptr<ID2D1HwndRenderTarget> renderTarget_;
        };
    }
}
