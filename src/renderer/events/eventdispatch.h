#pragma once

#include "ieventlistener.h"
#include "event.h"

namespace donut {
    class EventDispatch {
    public:
        EventDispatch(Event const& event)
            : m_event(event) {
        }

        bool WasDispatched() const { return m_dispatched; }

        void Dispatch(IEventListener* listener) {
            if (m_dispatched)
                return;
            m_dispatched = listener->OnEvent(m_event);
        }

        template<typename T, typename E>
        void Dispatch(T* obj, bool (T::*func)(E const&)) {
            if (m_dispatched)
                return;
            if (E const* castEvent = m_event.EventCast<E>())
                m_dispatched = (obj->*func)(*castEvent);
        }

    private:
        Event const& m_event;
        bool m_dispatched = false;
    };
}
