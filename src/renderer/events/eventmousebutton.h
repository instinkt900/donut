#pragma once

#include "event.h"

namespace donut {
    class EventMouseButton : public Event {
    public:
        EventMouseButton(int button, int action, int mods)
            : Event(EventType::EventMouseButton)
            , m_button(button)
            , m_action(action)
            , m_mods(mods) { }

        virtual ~EventMouseButton() = default;

        static EventType GetStaticType() { return EventType::EventMouseButton; }

        int GetButton() const { return m_button; }
        int GetAction() const { return m_action; }
        int GetMods() const { return m_mods; }

    private:
        int m_button;
        int m_action;
        int m_mods;
    };
}