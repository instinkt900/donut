#pragma once

#include "event.h"

namespace donut {
    class EventMouseScroll : public Event {
    public:
        EventMouseScroll(int xOffset, int yOffset)
            : Event(EventType::EventMouseScroll)
            , m_xoffset(xOffset)
            , m_yoffset(yOffset) { }

        virtual ~EventMouseScroll() = default;

        static EventType GetStaticType() { return EventType::EventMouseScroll; }

        int GetXOffset() const { return m_xoffset; }
        int GetYOffset() const { return m_yoffset; }

    private:
        int m_xoffset;
        int m_yoffset;
    };
}
