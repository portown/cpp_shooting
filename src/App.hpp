// App.hpp

#pragma once

#include <windows.h>
#include <d2d1.h>

#include <experimental/optional>

#include "GameView.hpp"


namespace std {
    using experimental::optional;
}


namespace shooting {
    namespace ui {
        class App {
        public:
            static auto create(HINSTANCE hInstance, int nCmdShow) -> std::optional<App>;
            ~App();

            auto run() -> int;

        private:
            explicit App(HINSTANCE hInstance, int nCmdShow);

            HINSTANCE const hInstance_;
            int const nCmdShow_;
            std::shared_ptr<ID2D1Factory> direct2dFactory_;

            std::shared_ptr<GameView> view_;
        };
    }
}
