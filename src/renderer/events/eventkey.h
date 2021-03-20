#pragma once

#include "event.h"

namespace donut {
    class EventKey : public Event {
    public:
        EventKey(int key, int action, int mods)
            : Event(EventType::EventKey)
            , m_key(key)
            , m_action(action)
            , m_mods(mods) { }

        virtual ~EventKey() = default;

        static EventType GetStaticType() { return EventType::EventKey; }

        int GetKey() const { return m_key; }
        int GetAction() const { return m_action; }
        int GetMods() const { return m_mods; }

    private:
        int m_key;
        int m_action;
        int m_mods;
    };
}