#pragma once

#include "event.h"

namespace donut {
    class EventMouseScroll : public Event {
    public:
        EventMouseScroll(double xOffset, double yOffset)
            : Event(EventType::EventMouseScroll)
            , m_xoffset(xOffset)
            , m_yoffset(yOffset) { }

        virtual ~EventMouseScroll() = default;

        static EventType GetStaticType() { return EventType::EventMouseScroll; }

        double GetXOffset() const { return m_xoffset; }
        double GetYOffset() const { return m_yoffset; }

    private:
        double m_xoffset;
        double m_yoffset;
    };
}
