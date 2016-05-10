// ComUtil.hpp

#pragma once

#include <memory>

#include <windows.h>


namespace com {
    struct ComDeleter {
        auto operator()(IUnknown* const p) const {
            p->Release();
        }
    };

    template <class T>
    using ComUniquePtr = std::unique_ptr<T, ComDeleter>;
}
