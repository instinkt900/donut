#pragma once

#include <ostream>
#include <istream>

namespace donut {
    struct MemoryBuffer : std::streambuf
    {
        MemoryBuffer(char* base, size_t size)
        {
            setp(base, base + size);
            setg(base, base, base + size);
        }
    };

    struct OutputMemoryStream : virtual MemoryBuffer, std::ostream
    {
        OutputMemoryStream(char* base, size_t size) : MemoryBuffer(base, size), std::ostream(static_cast<std::streambuf*>(this)) {}
    };

    struct InputMemoryStream : virtual MemoryBuffer, std::istream
    {
        InputMemoryStream(char* base, size_t size) : MemoryBuffer(base, size), std::istream(static_cast<std::streambuf*>(this)) {}
    };
}
