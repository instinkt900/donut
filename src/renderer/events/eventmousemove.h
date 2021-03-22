#pragma once

#include "event.h"

namespace donut {
    class EventMouseMove : public Event {
    public:
        EventMouseMove(double x, double y)
            : Event(EventType::EventMouseMove)
            , m_x(x)
            , m_y(y) { }

        virtual ~EventMouseMove() = default;

        static EventType GetStaticType() { return EventType::EventMouseMove; }

        double GetX() const { return m_x; }
        double GetY() const { return m_y; }

    private:
        double m_x;
        double m_y;
    };
}
