/**
 * This header is automatically generated using the Xstate to C++ code generator:
 *    https://github.com/shuvalov-mdb/xstate-cpp-generator
 *    Copyright (c) 2020 Andrew Shuvalov
 *    License: MIT https://opensource.org/licenses/MIT
 *
 * Please do not edit. If changes are needed, regenerate using the TypeScript template '{{it.properties.tsScriptName}}'.
 * Generated at {{it.generator.annotation()}} from Xstate definition '{{it.properties.tsScriptName}}'.
 * The simplest command line to run the generation:
 *     ts-node '{{it.properties.tsScriptName}}'
 */

#pragma once

#include <cassert>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
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
    using Event{{it.generator.capitalize(val)}}Payload = std::nullptr_t;
{{/each}}

    /**
     * Actions are modeled in the Xstate definition, see https://xstate.js.org/docs/guides/actions.html.
     * This block is for transition actions.
     */
{{@each(it.generator.allTransitionActions()) => pair, index}}
    static void {{pair[1]}} ({{it.generator.class()}}<Default{{it.generator.class()}}Spec>* sm, std::shared_ptr<Event{{it.generator.capitalize(pair[0])}}Payload>) {}
{{/each}}

    /**
     * This block is for entry and exit state actions.
     */
{{@each(it.generator.allEntryExitActions()) => action, index}}
    static void {{action}} ({{it.generator.class()}}<Default{{it.generator.class()}}Spec>* sm) {}
{{/each}}
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
        /** Timestamp of the last transition, or the class instantiation if at initial state */
        std::chrono::system_clock::time_point lastTransitionTime = std::chrono::system_clock::now();
        /** Count of the transitions made so far */
        int totalTransitions = 0;
    };

    {{it.generator.class()}}() {
        _eventsConsumerThread = std::make_unique<std::thread>([this] {
            _eventsConsumerThreadLoop();  // Start when all class members are initialized.
        });
    }

    virtual ~{{it.generator.class()}}() {
        for (int i = 0; i < 10; ++i) {
            if (isTerminated()) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        if (!isTerminated()) {
            std::cerr << "State Machine {{it.generator.class()}} is terminating "
                << "without reaching the final state." << std::endl;
        }
        // Force it.
        {
            std::lock_guard<std::mutex> lck(_lock);
            _smIsTerminated = true;
            _eventQueueCondvar.notify_one();
        }
        _eventsConsumerThread->join();
    }

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
    void postEvent{{it.generator.capitalize(val)}} (std::shared_ptr<{{it.generator.capitalize(val)}}Payload> payload);
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
     * Warning: it is not allowed to call postEvent(), or currentState(), or any other method from inside
     * this callback as it will be a deadlock.
     * @param callback is executed safely under lock for full R/W access to the Context. Thus, this method
     *   can be invoked concurrently from any thread and any of the callbacks declared below.
     */
    void accessContextLocked(std::function<void(StateMachineContext& userContext)> callback);

    /**
     * @returns true if State Machine reached the final state. Note that final state is optional.
     */
    bool isTerminated() const {
        std::lock_guard<std::mutex> lck(_lock);
        return _smIsTerminated;
    }

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
    virtual void onEnteringState{{it.generator.capitalize(pair[1])}}On{{pair[0]}}(State nextState, std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload> payload) {
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
    virtual void onEnteredState{{it.generator.capitalize(pair[1])}}On{{pair[0]}}(std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload> payload) {
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
    void _postEventHelper(State state, Event event, std::shared_ptr<Payload> payload);

    void _eventsConsumerThreadLoop();

    void _leavingStateHelper(State fromState, State newState);

    // The implementation will cast the void* of 'payload' back to full type to invoke the callback.
    void _enteringStateHelper(Event event, State newState, void* payload);

    void _transitionActionsHelper(State fromState, Event event, void* payload);

    // The implementation will cast the void* of 'payload' back to full type to invoke the callback.
    void _enteredStateHelper(Event event, State newState, void* payload);

    std::unique_ptr<std::thread> _eventsConsumerThread;

    mutable std::mutex _lock;

    CurrentState _currentState;

    // The SM can process events only in a serialized way. This queue stores the events to be processed.
    std::queue<std::function<void()>> _eventQueue;
    // Variable to wake up the consumer.
    std::condition_variable _eventQueueCondvar;

    bool _insideAccessContextLocked = false;
    bool _smIsTerminated = false;

    // Arbitrary user-defined data structure, see above.
    typename SMSpec::StateMachineContext _context;
};

/******   Internal implementation  ******/

{{@each(it.generator.events()) => val, index}}
template <typename SMSpec>
inline void {{it.generator.class()}}<SMSpec>::postEvent{{it.generator.capitalize(val)}} (std::shared_ptr<{{it.generator.class()}}::{{it.generator.capitalize(val)}}Payload> payload) {
    if (_insideAccessContextLocked) {
        // Intentianally not locked, we are checking for deadline here...
        static constexpr char error[] = "It is prohibited to post an event from inside the accessContextLocked()";
        std::cerr << error << std::endl;
        assert(false);
    }
    std::lock_guard<std::mutex> lck(_lock);
    State currentState = _currentState.currentState;
    std::function<void()> eventCb{[ this, currentState, payload ] () mutable {
        _postEventHelper(currentState, {{it.generator.class()}}::Event::{{val}}, payload);
    }};
    _eventQueue.emplace(eventCb);
    _eventQueueCondvar.notify_one();
}

{{/each}}

template<typename SMSpec>
template<typename Payload>
void {{it.generator.class()}}<SMSpec>::_postEventHelper ({{it.generator.class()}}::State state,
    {{it.generator.class()}}::Event event, std::shared_ptr<Payload> payload) {

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
        return;
    }

    // This can be conditional if guards are implemented...
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
        if (newState == State::{{it.generator.finalState()}}) {
            _smIsTerminated = true;
            _eventQueueCondvar.notify_one();  // SM will be terminated...
        }
    }

    // Step 6: Invoke the 'entered the state' callback.
    _enteredStateHelper(event, newState, &payload);
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_eventsConsumerThreadLoop() {
    while (true) {
        std::function<void()> nextCallback;
        {
            std::unique_lock<std::mutex> ulock(_lock);
            while (_eventQueue.empty() && !_smIsTerminated) {
                _eventQueueCondvar.wait(ulock);
            }
            if (_smIsTerminated) {
                break;
            }
            // The lock is re-acquired when 'wait' returns.
            nextCallback = std::move(_eventQueue.front());
            _eventQueue.pop();
        }
        // Outside of the lock.
        if (nextCallback) {
            nextCallback();
        }
    }
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_leavingStateHelper(State fromState, State newState) {
    switch (fromState) {
{{@foreach(it.machine.states) => key, val}}
    case State::{{key}}:
        onLeaving{{it.generator.capitalize(key)}}State (newState);
{{@each(it.generator.allExitActions(key)) => action, index}}
        SMSpec::{{action}}(this);
{{/each}}
        break;
{{/foreach}}
    }
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_enteringStateHelper(Event event, State newState, void* payload) {
    switch (newState) {
{{@foreach(it.machine.states) => key, val}}
    case State::{{key}}:
{{@each(it.generator.allEntryActions(key)) => action, index}}
        SMSpec::{{action}}(this);
{{/each}}
        break;
{{/foreach}}
    }

{{@each(it.generator.allEventToStatePairs()) => pair, index}}
    if (event == Event::{{pair[0]}} && newState == State::{{pair[1]}}) {
        std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload>* typedPayload = static_cast<std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload>*>(payload);
        onEnteringState{{it.generator.capitalize(pair[1])}}On{{pair[0]}}(newState, *typedPayload);
        return;
    }
{{/each}}
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_transitionActionsHelper(State fromState, Event event, void* payload) {
{{@foreach(it.machine.states) => state, val}}
{{@each(it.generator.allTransitionActions(state)) => pair, index}}
    if (fromState == State::{{state}} && event == Event::{{pair[0]}}) {
        std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload>* typedPayload = static_cast<std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload>*>(payload);
        SMSpec::{{pair[1]}}(this, *typedPayload);
    }
{{/each}}
{{/foreach}}
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_enteredStateHelper(Event event, State newState, void* payload) {
{{@each(it.generator.allEventToStatePairs()) => pair, index}}
    if (event == Event::{{pair[0]}} && newState == State::{{pair[1]}}) {
        std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload>* typedPayload = static_cast<std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload>*>(payload);
        onEnteredState{{it.generator.capitalize(pair[1])}}On{{pair[0]}}(*typedPayload);
        return;
    }
{{/each}}
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::accessContextLocked(std::function<void(StateMachineContext& userContext)> callback) {
    std::lock_guard<std::mutex> lck(_lock);
    // This variable is preventing the user from posting an event while inside the callback,
    // as it will be a deadlock.
    _insideAccessContextLocked = true;
    callback(_context);  // User can modify the context under lock.
    _insideAccessContextLocked = false;
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
    case TransitionPhase::TRANSITION_NOT_FOUND:
        std::clog << phase << "from " << currentState;
        break;
    default:
        std::clog << "ERROR ";
        break;
    }
    std::clog << std::endl;
}


}  // namespace

