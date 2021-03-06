// App.hpp

#pragma once

#include <windows.h>
#include <d2d1.h>

#include "optional.hpp"

#include "GameView.hpp"


namespace shooting {
    namespace ui {
        class App {
        public:
            static auto create(HINSTANCE hInstance, int nCmdShow) -> std::optional<App>;
            ~App();

            App(App&&) = default;
            App& operator=(App&&) = default;

            auto run() -> int;

        private:
            explicit App(HINSTANCE hInstance, int nCmdShow);

            HINSTANCE const hInstance_;
            int const nCmdShow_;
            std::shared_ptr<ID2D1Factory> direct2dFactory_;

            std::unique_ptr<GameView> view_;
        };
    }
}
