// GameView.hpp

#pragma once

#include <windows.h>
#include <d2d1.h>

#include <memory>

#include "ComUtil.hpp"


namespace shooting {
    namespace ui {
        class GameView {
        public:
            static auto create(
                    HINSTANCE hInstance,
                    int nCmdShow,
                    std::shared_ptr<ID2D1Factory> direct2dFactory) -> std::unique_ptr<GameView>;

        private:
            explicit constexpr GameView() = default;
            static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
            void onPaint();

            HWND hWnd_{};
            std::shared_ptr<ID2D1Factory> direct2dFactory_;
            com::ComUniquePtr<ID2D1HwndRenderTarget> renderTarget_;
        };
    }
}
