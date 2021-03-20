#pragma once

#include "event.h"

namespace donut {
	class EventResize : public Event {
    public:
        EventResize(int width, int height)
        : Event(EventType::EventResize)
        , m_width(width)
        , m_height(height)
        { }

        virtual ~EventResize() = default;

        static EventType GetStaticType() { return EventType::EventResize; }

        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }

    private:
        int m_width;
        int m_height;
	};
}
