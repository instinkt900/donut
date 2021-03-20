#pragma once

namespace donut {
    enum class EventType {
        EventUnknown,
        EventResize,
        EventKey,
        EventMouseButton,
        EventMouseScroll,
        EventMouseMove,
    };
}
