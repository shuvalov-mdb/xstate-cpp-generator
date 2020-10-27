// This is an automatically generated header, do not edit.

#pragma once

#include <iostream>
#include <deque>
#include <functional>
#include <mutex>
#include <sstream>
#include <tuple>
#include <vector>

namespace {{it.properties.namespace }} {

// All states declared in the SM {{it.generator.class()}}.
enum class {{it.generator.class()}}State {
    UNDEFINED_OR_ERROR_STATE = 0,
{{@foreach(it.machine.states) => key, val}}
    {{key}},
{{/foreach}}
};

std::string {{it.generator.class()}}StateToString({{it.generator.class()}}State state);

std::ostream& operator << (std::ostream& os, const {{it.generator.class()}}State& state);

// @returns true if 'state' is a valid State.
bool isValid{{it.generator.class()}}State({{it.generator.class()}}State state);

// All events declared in the SM {{it.generator.class()}}.
enum class {{it.generator.class()}}Event {
    UNDEFINED_OR_ERROR_EVENT = 0,
{{@each(it.generator.events()) => val, index}}
    {{val}},
{{/each}}
};

std::string {{it.generator.class()}}EventToString({{it.generator.class()}}Event event);

std::ostream& operator << (std::ostream& os, const {{it.generator.class()}}Event& event);

// @returns true if 'event' is a valid Event.
bool isValid{{it.generator.class()}}Event({{it.generator.class()}}Event event);

// As a transition could be conditional (https://xstate.js.org/docs/guides/guards.html#guards-condition-functions)
// one event is mapped to a vector of possible transitions.
using {{it.generator.class()}}TransitionToStatesPair = std::pair<{{it.generator.class()}}Event,
        std::vector<{{it.generator.class()}}State>>;

/**
 * All valid transitions from the specified state. The transition to state graph
 * is code genrated from the model and cannot change.
 */
{{@foreach(it.machine.states) => key, val}}
const std::vector<{{it.generator.class()}}TransitionToStatesPair>& {{it.generator.class()}}ValidTransitionsFrom{{it.generator.capitalize(key)}}State();
{{/foreach}}


/**
 * Convenient default SM spec structure to parameterize the State Machine.
 * It can be replaced with a custom one if the SM events do not need any payload to be attached, and if there
 * is no guards, and no other advanced features.
 */
template <typename SMContext = std::nullptr_t>
struct Default{{it.generator.class()}}Spec {
    /** 
     * Generic data structure stored in the State Machine to keep some user-defined state that can be modified
     * when transitions happen.
     */
    using StateMachineContext = SMContext;

    /**
     * Each Event has a payload attached, which is passed in to the related callbacks.
     * The type should be movable for efficiency.
     */
{{@each(it.generator.events()) => val, index}}
    using Event{{it.generator.capitalize(val)}}Payload = std::nullptr_t;
{{/each}}
};

/**
 *  State machine as declared in Xstate library for {{it.generator.class()}}.
 *  SMSpec is a convenient template struct, which allows to specify various definitions used by generated code. In a simple
 *  case it's not needed and a convenient default is provided.
 */
template <typename SMSpec = Default{{it.generator.class()}}Spec<std::nullptr_t>>
class {{it.generator.class()}} {
  public:
    using TransitionToStatesPair = {{it.generator.class()}}TransitionToStatesPair;
    using State = {{it.generator.class()}}State;
    using Event = {{it.generator.class()}}Event;
{{@each(it.generator.events()) => val, index}}
    using {{it.generator.capitalize(val)}}Payload = typename SMSpec::Event{{it.generator.capitalize(val)}}Payload;
{{/each}}

    /**
     * Structure represents the current in-memory state of the State Machine.
     */
    struct CurrentState {
        State currentState = {{it.generator.class()}}State::{{it.generator.initialState()}};
        /** previousState could be undefined if SM is at initial state */
        State previousState;
        /** The event that transitioned the SM from previousState to currentState */
        Event lastEvent;
        /** 
         * The SM can process events only in a serialized way. If this Event Queue is empty, the posted event will be
         * processed immediately, otherwise it will be posted to the queue and processed by the same thread that is
         * currently processing the previous Event. Thus each Event is removed from the queue only when it's processed.
         * This SM is strictly single-threaded it terms of processing all necessary callbacks, it is using the same 
         * user thread that invoked a 'send Event' method to drain the whole queue.
         */
        std::deque<std::function<void()>> eventQueue;
        /** Timestamp of the last transition, or the class instantiation if at initial state */
        std::chrono::system_clock::time_point lastTransitionTime = std::chrono::system_clock::now();
        /** Count of the transitions made so far */
        int totalTransitions = 0;
    };

    {{it.generator.class()}}() {}

    virtual ~{{it.generator.class()}}() {}

    CurrentState currentState() const {
        std::lock_guard<std::mutex> lck(_lock);
        return _currentState;
    }

    /**
     * The only way to change the SM state is to post an event.
     * If the event queue is empty the transition will be processed in the current thread.
     * If the event queue is not empty, this adds the event into the queue and returns immediately. The events
     * in the queue will be processed sequentially by the same thread that is currently processing the front of the queue.
     */
{{@each(it.generator.events()) => val, index}}
    void postEvent{{it.generator.capitalize(val)}} ({{it.generator.capitalize(val)}}Payload&& payload);
{{/each}}

    /**
     * All valid transitions from the current state of the State Machine.
     */
    const std::vector<{{it.generator.class()}}TransitionToStatesPair>& validTransitionsFromCurrentState() const {
        std::lock_guard<std::mutex> lck(_lock);
        return validTransitionsFrom(_currentState.currentState);
    }

    /**
     * All valid transitions from the specified state.
     */
    static inline const std::vector<TransitionToStatesPair>& validTransitionsFrom({{it.generator.class()}}State state) {
        switch (state) {
{{@foreach(it.machine.states) => key, val}}
          case {{it.generator.class()}}State::{{key}}:
            return {{it.generator.class()}}ValidTransitionsFrom{{it.generator.capitalize(key)}}State();
{{/foreach}}
          default: {
            std::stringstream ss;
            ss << "invalid SM state " << state;
            throw std::runtime_error(ss.str());
          } break;
        }
    }

  private:
    template<typename Payload>
    void _postEventHelper (Event event, Payload&& payload);

    mutable std::mutex _lock;

    CurrentState _currentState;
    // Arbitrary user-defined data structure, see above.
    typename SMSpec::StateMachineContext _context;
};

{{@each(it.generator.events()) => val, index}}
template <typename SMSpec>
inline void {{it.generator.class()}}<SMSpec>::postEvent{{it.generator.capitalize(val)}} ({{it.generator.class()}}::{{it.generator.capitalize(val)}}Payload&& payload) {
    _postEventHelper({{it.generator.class()}}::Event::{{val}}, std::move(payload));
}

{{/each}}

template<typename SMSpec>
template<typename Payload>
void {{it.generator.class()}}<SMSpec>::_postEventHelper ({{it.generator.class()}}::Event event, Payload&& payload) {
    std::lock_guard<std::mutex> lck(_lock);
    const bool processNow = _currentState.eventQueue.empty();
}


}  // namespace

