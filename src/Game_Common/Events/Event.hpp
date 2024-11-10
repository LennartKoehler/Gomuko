#pragma once

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
								virtual EventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }
#define HZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

enum class EventType
{
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};



class Event{
public:
    bool handled = false;
    virtual EventType getEventType() const = 0;
    virtual const char* getName() const = 0;
    virtual std::string toString() const { return getName(); }
};

class EventDispatcher{
public:
    EventDispatcher (Event& event) : m_Event(event){}
    
    template<typename T, typename F>
    bool dispatch(const F& function){
        if (m_Event.getEventType() == T::getStaticType()){
            m_Event.handled |= function(static_cast<T&>(m_Event));
				return true;
			}
			return false;
        }

private:
    Event& m_Event;
};

