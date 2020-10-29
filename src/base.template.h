/** 
 * This header is automatically generated using the Xstate to C++ code generator:
 *    https://github.com/shuvalov-mdb/xstate-cpp-generator , @author Andrew Shuvalov
 *
 * Please do not edit. If changes are needed, regenerate using the TypeScript template '{{it.properties.tsScriptName}}'.
 * Generated at {{it.generator.annotation()}} from Xstate definition '{{it.properties.tsScriptName}}'.
 * The simplest command line to run the generation:
 *     ts-node '{{it.properties.tsScriptName}}'
 */

#pragma once

#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <memory>
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
 * Enum to indicate the current state transition phase in callbacks. This enum is used only for logging
 * and is not part of any State Machine logic.
 */
enum class {{it.generator.class()}}TransitionPhase { 
    UNDEFINED = 0,
    LEAVING_STATE,
    ENTERING_STATE,
    ENTERED_STATE,
    TRANSITION_NOT_FOUND
};

std::ostream& operator << (std::ostream& os, const {{it.generator.class()}}TransitionPhase& phase);

template <typename SMSpec> class {{it.generator.class()}};  // Forward declaration to use in Spec.

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
    using Event{{it.generator.capitalize(val)}}Payload = std::unique_ptr<std::nullptr_t>;
{{/each}}

    /**
     * Actions are modeled in the Xstate definition, see https://xstate.js.org/docs/guides/actions.html.
     * This block is for transition actions.
     */
{{@foreach(it.machine.states) => state, val}}
{{@each(it.generator.stateEventActions(state)) => pair, index}}
    void {{pair[1]}} ({{it.generator.class()}}<Default{{it.generator.class()}}Spec>* sm, Event{{it.generator.capitalize(pair[0])}}Payload*) {}
{{/each}}
{{/foreach}}
};

/**
 *  State machine as declared in Xstate library for {{it.generator.class()}}.
 *  SMSpec is a convenient template struct, which allows to specify various definitions used by generated code. In a simple
 *  case it's not needed and a convenient default is provided.
 * 
 *  State Machine is not an abstract class and can be used without subclassing at all,
 *  though its functionality will be limited in terms of callbacks.
 *  Even though it's a templated class, a default SMSpec is provided to make a simple
 *  State Machine without any customization. In the most simple form, a working 
 *  {{it.generator.class()}} SM instance can be instantiated and used as in this example:
 * 
 *    {{it.generator.class()}}<> machine;
 *    auto currentState = machine.currentState();
{{@each(it.generator.events()) => val, index}}
 *    {{it.generator.class()}}<>::{{it.generator.capitalize(val)}}Payload payload{{val}};      // ..and init payload with data
 *    machine.postEvent{{it.generator.capitalize(val)}} (std::move(payload{{val}}));
{{/each}}
 * 
 *  Also see the generated unit tests in the example-* folders for more example code.
 */
template <typename SMSpec = Default{{it.generator.class()}}Spec<std::nullptr_t>>
class {{it.generator.class()}} {
  public:
    using TransitionToStatesPair = {{it.generator.class()}}TransitionToStatesPair;
    using State = {{it.generator.class()}}State;
    using Event = {{it.generator.class()}}Event;
    using TransitionPhase = {{it.generator.class()}}TransitionPhase;
    using StateMachineContext = typename SMSpec::StateMachineContext;
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
         * The SM can process events only in a serialized way. If this 'blockedForProcessingAnEvent' is false, the posted 
         * event will be processed immediately, otherwise it will be posted to the queue and processed by the same 
         * thread that is currently processing the previous Event. 
         * This SM is strictly single-threaded it terms of processing all necessary callbacks, it is using the same 
         * user thread that invoked a 'send Event' method to drain the whole queue.
         */
        bool blockedForProcessingAnEvent = false;
        /** 
         * The SM can process events only in a serialized way. This queue stores the events to be processed.
         */
        std::deque<std::function<void()>> eventQueue;
        /** Timestamp of the last transition, or the class instantiation if at initial state */
        std::chrono::system_clock::time_point lastTransitionTime = std::chrono::system_clock::now();
        /** Count of the transitions made so far */
        int totalTransitions = 0;
    };

    {{it.generator.class()}}() {}

    virtual ~{{it.generator.class()}}() {}

    /**
     * Returns a copy of the current state, skipping some fields.
     */
    CurrentState currentState() const {
        std::lock_guard<std::mutex> lck(_lock);
        CurrentState aCopy;  // We will not copy the event queue.
        aCopy.currentState = _currentState.currentState;
        aCopy.previousState = _currentState.previousState;
        aCopy.lastEvent = _currentState.lastEvent;
        aCopy.totalTransitions = _currentState.totalTransitions;
        aCopy.lastTransitionTime = _currentState.lastTransitionTime;
        return aCopy;
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
     * Provides a mechanism to access the internal user-defined Context (see SMSpec::StateMachineContext).
     * @param callback is executed safely under lock for full R/W access to the Context. Thus, this method
     *   can be invoked concurrently from any thread and any of the callbacks declared below.
     */
    void accessContextLocked(std::function<void(StateMachineContext& userContext)> callback);

    /**
     * The block of virtual callback methods the derived class can override to extend the SM functionality.
     * All callbacks are invoked without holding the internal lock, thus it is allowed to call SM methods from inside.
     */

    /**
     * Overload this method to log or mute the case when the default generated method for entering, entered
     * or leaving the state is not overloaded. By default it just prints to stdout. The default action is very
     * useful for the initial development. In production. it's better to replace it with an appropriate
     * logging or empty method to mute.
     */
    virtual void logTransition(TransitionPhase phase, State currentState, State nextState) const;

    /**
     * 'onLeavingState' callbacks are invoked right before entering a new state. The internal 
     * '_currentState' data still points to the current state.
     */
{{@foreach(it.machine.states) => key, val}}
    virtual void onLeaving{{it.generator.capitalize(key)}}State(State nextState) {
        logTransition({{it.generator.class()}}TransitionPhase::LEAVING_STATE, State::{{key}}, nextState);
    }
{{/foreach}}

    /**
     * 'onEnteringState' callbacks are invoked right before entering a new state. The internal 
     * '_currentState' data still points to the existing state.
     * @param payload mutable payload, ownership remains with the caller. To take ownership of the payload 
     *   override another calback from the 'onEntered*State' below.
     */
{{@each(it.generator.allEventToStatePairs()) => pair, index}}
    virtual void onEnteringState{{it.generator.capitalize(pair[1])}}On{{pair[0]}}(State nextState, {{it.generator.capitalize(pair[0])}}Payload* payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition({{it.generator.class()}}TransitionPhase::ENTERING_STATE, _currentState.currentState, State::{{pair[1]}});
    }
{{/each}}

    /**
     * 'onEnteredState' callbacks are invoked after SM moved to new state. The internal 
     * '_currentState' data already points to the existing state.
     * It is guaranteed that the next transition will not start until this callback returns.
     * It is safe to call postEvent*() to trigger the next transition from this method.
     * @param payload ownership is transferred to the user.
     */
{{@each(it.generator.allEventToStatePairs()) => pair, index}}
    virtual void onEnteredState{{it.generator.capitalize(pair[1])}}On{{pair[0]}}({{it.generator.capitalize(pair[0])}}Payload&& payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition({{it.generator.class()}}TransitionPhase::ENTERED_STATE, _currentState.currentState, State::{{pair[1]}});
    }
{{/each}}


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
    void _postEventHelper(State state, Event event, Payload&& payload);

    void _leavingStateHelper(State fromState, State newState);

    // The implementation will cast the void* of 'payload' back to full type to invoke the callback.
    void _enteringStateHelper(Event event, State newState, void* payload);

    void _transitionActionsHelper(State fromState, Event event, void* payload);

    // The implementation will cast the void* of 'payload' back to full type to invoke the callback.
    void _enteredStateHelper(Event event, State newState, void* payload);

    mutable std::mutex _lock;

    CurrentState _currentState;
    // Arbitrary user-defined data structure, see above.
    typename SMSpec::StateMachineContext _context;
};

/******   Internal implementation  ******/

{{@each(it.generator.events()) => val, index}}
template <typename SMSpec>
inline void {{it.generator.class()}}<SMSpec>::postEvent{{it.generator.capitalize(val)}} ({{it.generator.class()}}::{{it.generator.capitalize(val)}}Payload&& payload) {
    State currentState;
    {
        std::lock_guard<std::mutex> lck(_lock);
        // If the SM is currently processing another event, adds this one to the queue. The thread processing
        // that event is responsible to drain the queue, this is why we also check for the queue size.
        if (_currentState.blockedForProcessingAnEvent || !_currentState.eventQueue.empty()) {
            std::function<void()> eventCb{[ this, p{std::make_shared<{{it.generator.capitalize(val)}}Payload>(std::move(payload))} ] () mutable {
                postEvent{{it.generator.capitalize(val)}} (std::move(*p));
            }};
            _currentState.eventQueue.emplace_back(eventCb);
            return;  // Returns immediately, the event will be posted asynchronously.
        }

        currentState = _currentState.currentState;
        _currentState.blockedForProcessingAnEvent = true;
    }
    // Event processing is done outside of the '_lock' as the 'blockedForProcessingAnEvent' flag is guarding us.
    _postEventHelper(currentState, {{it.generator.class()}}::Event::{{val}}, std::move(payload));
}

{{/each}}

template<typename SMSpec>
template<typename Payload>
void {{it.generator.class()}}<SMSpec>::_postEventHelper ({{it.generator.class()}}::State state, {{it.generator.class()}}::Event event, Payload&& payload) {

    // Step 1: Invoke the guard callback. TODO: implement.

    // Step 2: check if the transition is valid.
    const std::vector<{{it.generator.class()}}State>* targetStates = nullptr;
    const std::vector<{{it.generator.class()}}TransitionToStatesPair>& validTransitions = validTransitionsFrom(state);
    for (const auto& transitionEvent : validTransitions) {
        if (transitionEvent.first == event) {
            targetStates = &transitionEvent.second;
        }
    }
    if (targetStates == nullptr || targetStates->empty()) {
        logTransition(TransitionPhase::TRANSITION_NOT_FOUND, state, state);
        std::lock_guard<std::mutex> lck(_lock);
        _currentState.blockedForProcessingAnEvent = false;  // We are done.
        return;
    }
    State newState = (*targetStates)[0];

    // Step 3: Invoke the 'leaving the state' callback.
    _leavingStateHelper(state, newState);

    // Step 4: Invoke the 'entering the state' callback.
    _enteringStateHelper(event, newState, &payload);

    // ... and the transiton actions.
    _transitionActionsHelper(state, event, &payload);

    {
        // Step 5: do the transition.
        std::lock_guard<std::mutex> lck(_lock);
        _currentState.previousState = _currentState.currentState;
        _currentState.currentState = newState;
        _currentState.lastTransitionTime = std::chrono::system_clock::now();
        _currentState.lastEvent = event;
        ++_currentState.totalTransitions;
    }

    // Step 6: Invoke the 'entered the state' callback.
    _enteredStateHelper(event, newState, &payload);

    std::function<void()> nextCallback;
    {
        // Step 7: pick the next event and clear the processing flag.
        std::lock_guard<std::mutex> lck(_lock);
        if (!_currentState.eventQueue.empty()) {
            nextCallback = std::move(_currentState.eventQueue.front());  // Keep the queue not empty.
            _currentState.eventQueue.front() = nullptr;  // Make sure to signal other threads to not work on this queue.
        }
        _currentState.blockedForProcessingAnEvent = false;  // We are done, even though we can have another step.
    }

    if (nextCallback) {
        // Step 8: execute the next callback and then remove it from the queue.
        nextCallback();
        std::lock_guard<std::mutex> lck(_lock);
        assert(_currentState.eventQueue.front() == nullptr);
        _currentState.eventQueue.pop_front();
    }
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_leavingStateHelper(State fromState, State newState) {
    switch (fromState) {
{{@foreach(it.machine.states) => key, val}}
    case State::{{key}}:
        onLeaving{{it.generator.capitalize(key)}}State (newState);
        break;
{{/foreach}}
    }
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_enteringStateHelper(Event event, State newState, void* payload) {
{{@each(it.generator.allEventToStatePairs()) => pair, index}}
    if (event == Event::{{pair[0]}} && newState == State::{{pair[1]}}) {
        {{it.generator.capitalize(pair[0])}}Payload* typedPayload = static_cast<{{it.generator.capitalize(pair[0])}}Payload*>(payload);
        onEnteringState{{it.generator.capitalize(pair[1])}}On{{pair[0]}}(newState, typedPayload);
        return;
    }
{{/each}}
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_transitionActionsHelper(State fromState, Event event, void* payload) {
{{@foreach(it.machine.states) => state, val}}
{{@each(it.generator.stateEventActions(state)) => pair, index}}
    if (fromState == State::{{state}} && event == Event::{{pair[0]}}) {
        {{it.generator.capitalize(pair[0])}}Payload* typedPayload = static_cast<{{it.generator.capitalize(pair[0])}}Payload*>(payload);
        SMSpec().{{pair[1]}}(this, typedPayload);
    }
{{/each}}
{{/foreach}}
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_enteredStateHelper(Event event, State newState, void* payload) {
{{@each(it.generator.allEventToStatePairs()) => pair, index}}
    if (event == Event::{{pair[0]}} && newState == State::{{pair[1]}}) {
        {{it.generator.capitalize(pair[0])}}Payload* typedPayload = static_cast<{{it.generator.capitalize(pair[0])}}Payload*>(payload);
        onEnteredState{{it.generator.capitalize(pair[1])}}On{{pair[0]}}(std::move(*typedPayload));
        return;
    }
{{/each}}
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::accessContextLocked(std::function<void(StateMachineContext& userContext)> callback) {
    std::lock_guard<std::mutex> lck(_lock);
    callback(_context);  // User can modify the context under lock.
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::logTransition(TransitionPhase phase, State currentState, State nextState) const {
    switch (phase) {
    case TransitionPhase::LEAVING_STATE:
        std::clog << phase << currentState << ", transitioning to " << nextState;
        break;
    case TransitionPhase::ENTERING_STATE:
        std::clog << phase << nextState << " from " << currentState;
        break;
    case TransitionPhase::ENTERED_STATE:
        std::clog << phase << currentState;
        break;
    default:
        std::clog << "ERROR ";
        break;
    }
    std::clog << std::endl;
}


}  // namespace

