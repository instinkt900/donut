#pragma once

#include "event.h"

namespace donut {
    class EventMouseMove : public Event {
    public:
        EventMouseMove(int x, int y)
            : Event(EventType::EventMouseMove)
            , m_x(x)
            , m_y(y) { }

        virtual ~EventMouseMove() = default;

        static EventType GetStaticType() { return EventType::EventMouseMove; }

        int GetX() const { return m_x; }
        int GetY() const { return m_y; }

    private:
        int m_x;
        int m_y;
    };
}
