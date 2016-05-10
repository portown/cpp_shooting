// WinUtil.hpp

#pragma once

#include <windows.h>


namespace win {
    auto getClientRect(HWND const hWnd) {
        RECT rc;
        GetClientRect(hWnd, &rc);
        return rc;
    }

    constexpr auto width(RECT const& rc) { return rc.right - rc.left; }
    constexpr auto height(RECT const& rc) { return rc.bottom - rc.top; }

    namespace detail {
        template <int code, class ResultType>
        struct GDIStockObjectType {};
    }

    namespace {
        constexpr auto WhiteBrush = detail::GDIStockObjectType<WHITE_BRUSH, HBRUSH>{};
    }

    template <int code, class ResultType>
    inline ResultType getStockObject(detail::GDIStockObjectType<code, ResultType>) {
        return reinterpret_cast<ResultType>(GetStockObject(code));
    }
}
