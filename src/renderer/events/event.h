#pragma once

#include "event_types.h"

namespace donut {
    class Event {
    public:
        virtual ~Event() = default;

        static EventType GetStaticType() { return EventType::EventUnknown; }

        template<typename T>
        T const* EventCast() const {
            if (m_type == T::GetStaticType())
                return static_cast<T const*>(this);
            return nullptr;
        }

    protected:
        Event(EventType type)
            : m_type(type) { }


    private:
        EventType m_type;
    };
}
