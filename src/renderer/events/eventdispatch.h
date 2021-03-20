#pragma once

#include "event.h"

namespace donut {
    class EventDispatch {
    public:
        EventDispatch(Event const& event)
            : m_event(event) {
        }

        bool WasDispatched() const { return m_dispatched; }

        template<typename T, typename E>
        void Dispatch(T* obj, bool (T::*func)(E const&)) {
            if (m_dispatched)
                return;
            if (E const* castEvent = m_event.EventCast<E>())
                (obj->*func)(*castEvent);
        }

    private:
        Event const& m_event;
        bool m_dispatched = false;
    };
}