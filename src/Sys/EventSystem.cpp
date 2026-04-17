#include "EventSystem.h"

void EventSystem::subscribe(EventType eventType, EventCallback callback) {
    listeners[eventType].push_back(callback);
}

void EventSystem::triggerEvent(EventType eventType, Entity& entity) {
    auto it = listeners.find(eventType);
    if (it != listeners.end()) {
        for (auto& callback : it->second) {
            callback(entity);
        }
    }
}

void EventSystem::clear() {
    listeners.clear();
}

