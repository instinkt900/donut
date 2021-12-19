#pragma once

namespace donut::vulkan {
    struct Viewport {
        float x, y, width, height;
    };

    bool operator==(Viewport const& a, Viewport const& b) {
        return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
    }

    bool operator!=(Viewport const& a, Viewport const& b) {
        return !(a == b);
    }
}
