#pragma once

namespace donut {
    class NonCopyable {
    public:
        NonCopyable() = default;
        ~NonCopyable() = default;

        NonCopyable(NonCopyable const&) = delete;
        NonCopyable& operator=(NonCopyable const&) = delete;
    };
}
