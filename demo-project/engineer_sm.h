/** 
 * This header is automatically generated using the Xstate to C++ code generator:
 *    https://github.com/shuvalov-mdb/xstate-cpp-generator , @author Andrew Shuvalov
 *
 * Please do not edit. If changes are needed, regenerate using the TypeScript template 'engineer.ts'.
 * Generated at Thu Oct 29 2020 17:12:02 GMT+0000 (Coordinated Universal Time) from Xstate definition 'engineer.ts'.
 * The simplest command line to run the generation:
 *     ts-node 'engineer.ts'
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

namespace engineer_demo {

// All states declared in the SM EngineerSM.
enum class EngineerSMState {
    UNDEFINED_OR_ERROR_STATE = 0,
    sleeping,
    working,
    eating,
};

std::string EngineerSMStateToString(EngineerSMState state);

std::ostream& operator << (std::ostream& os, const EngineerSMState& state);

// @returns true if 'state' is a valid State.
bool isValidEngineerSMState(EngineerSMState state);

// All events declared in the SM EngineerSM.
enum class EngineerSMEvent {
    UNDEFINED_OR_ERROR_EVENT = 0,
    TIMER,
    TIRED,
    HUNGRY,
};

std::string EngineerSMEventToString(EngineerSMEvent event);

std::ostream& operator << (std::ostream& os, const EngineerSMEvent& event);

// @returns true if 'event' is a valid Event.
bool isValidEngineerSMEvent(EngineerSMEvent event);

// As a transition could be conditional (https://xstate.js.org/docs/guides/guards.html#guards-condition-functions)
// one event is mapped to a vector of possible transitions.
using EngineerSMTransitionToStatesPair = std::pair<EngineerSMEvent,
        std::vector<EngineerSMState>>;

/**
 * All valid transitions from the specified state. The transition to state graph
 * is code genrated from the model and cannot change.
 */
const std::vector<EngineerSMTransitionToStatesPair>& EngineerSMValidTransitionsFromSleepingState();
const std::vector<EngineerSMTransitionToStatesPair>& EngineerSMValidTransitionsFromWorkingState();
const std::vector<EngineerSMTransitionToStatesPair>& EngineerSMValidTransitionsFromEatingState();

/**
 * Enum to indicate the current state transition phase in callbacks. This enum is used only for logging
 * and is not part of any State Machine logic.
 */
enum class EngineerSMTransitionPhase { 
    UNDEFINED = 0,
    LEAVING_STATE,
    ENTERING_STATE,
    ENTERED_STATE,
    TRANSITION_NOT_FOUND
};

std::ostream& operator << (std::ostream& os, const EngineerSMTransitionPhase& phase);

template <typename SMSpec> class EngineerSM;  // Forward declaration to use in Spec.

/**
 * Convenient default SM spec structure to parameterize the State Machine.
 * It can be replaced with a custom one if the SM events do not need any payload to be attached, and if there
 * is no guards, and no other advanced features.
 */
template <typename SMContext = std::nullptr_t>
struct DefaultEngineerSMSpec {
    /** 
     * Generic data structure stored in the State Machine to keep some user-defined state that can be modified
     * when transitions happen.
     */
    using StateMachineContext = SMContext;

    /**
     * Each Event has a payload attached, which is passed in to the related callbacks.
     * The type should be movable for efficiency.
     */
    using EventTimerPayload = std::unique_ptr<std::nullptr_t>;
    using EventTiredPayload = std::unique_ptr<std::nullptr_t>;
    using EventHungryPayload = std::unique_ptr<std::nullptr_t>;

    /**
     * Actions are modeled in the Xstate definition, see https://xstate.js.org/docs/guides/actions.html.
     * This block is for transition actions.
     */
    static void startHungryTimer (EngineerSM<DefaultEngineerSMSpec>* sm, EventTimerPayload*) {}
    static void startTiredTimer (EngineerSM<DefaultEngineerSMSpec>* sm, EventTimerPayload*) {}
    static void checkEmail (EngineerSM<DefaultEngineerSMSpec>* sm, EventHungryPayload*) {}

    /**
     * This block is for entry and exit state actions.
     */
    static void startWakeupTimer (EngineerSM<DefaultEngineerSMSpec>* sm) {}
    static void checkEmail (EngineerSM<DefaultEngineerSMSpec>* sm) {}
    static void startHungryTimer (EngineerSM<DefaultEngineerSMSpec>* sm) {}
    static void startShortTimer (EngineerSM<DefaultEngineerSMSpec>* sm) {}
    static void morningRoutine (EngineerSM<DefaultEngineerSMSpec>* sm) {}
    static void startTiredTimer (EngineerSM<DefaultEngineerSMSpec>* sm) {}
};

/**
 *  State machine as declared in Xstate library for EngineerSM.
 *  SMSpec is a convenient template struct, which allows to specify various definitions used by generated code. In a simple
 *  case it's not needed and a convenient default is provided.
 * 
 *  State Machine is not an abstract class and can be used without subclassing at all,
 *  though its functionality will be limited in terms of callbacks.
 *  Even though it's a templated class, a default SMSpec is provided to make a simple
 *  State Machine without any customization. In the most simple form, a working 
 *  EngineerSM SM instance can be instantiated and used as in this example:
 * 
 *    EngineerSM<> machine;
 *    auto currentState = machine.currentState();
 *    EngineerSM<>::TimerPayload payloadTIMER;      // ..and init payload with data
 *    machine.postEventTimer (std::move(payloadTIMER));
 *    EngineerSM<>::TiredPayload payloadTIRED;      // ..and init payload with data
 *    machine.postEventTired (std::move(payloadTIRED));
 *    EngineerSM<>::HungryPayload payloadHUNGRY;      // ..and init payload with data
 *    machine.postEventHungry (std::move(payloadHUNGRY));
 * 
 *  Also see the generated unit tests in the example-* folders for more example code.
 */
template <typename SMSpec = DefaultEngineerSMSpec<std::nullptr_t>>
class EngineerSM {
  public:
    using TransitionToStatesPair = EngineerSMTransitionToStatesPair;
    using State = EngineerSMState;
    using Event = EngineerSMEvent;
    using TransitionPhase = EngineerSMTransitionPhase;
    using StateMachineContext = typename SMSpec::StateMachineContext;
    using TimerPayload = typename SMSpec::EventTimerPayload;
    using TiredPayload = typename SMSpec::EventTiredPayload;
    using HungryPayload = typename SMSpec::EventHungryPayload;

    /**
     * Structure represents the current in-memory state of the State Machine.
     */
    struct CurrentState {
        State currentState = EngineerSMState::sleeping;
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

    EngineerSM() {}

    virtual ~EngineerSM() {}

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
    void postEventTimer (TimerPayload&& payload);
    void postEventTired (TiredPayload&& payload);
    void postEventHungry (HungryPayload&& payload);

    /**
     * All valid transitions from the current state of the State Machine.
     */
    const std::vector<EngineerSMTransitionToStatesPair>& validTransitionsFromCurrentState() const {
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
    virtual void onLeavingSleepingState(State nextState) {
        logTransition(EngineerSMTransitionPhase::LEAVING_STATE, State::sleeping, nextState);
    }
    virtual void onLeavingWorkingState(State nextState) {
        logTransition(EngineerSMTransitionPhase::LEAVING_STATE, State::working, nextState);
    }
    virtual void onLeavingEatingState(State nextState) {
        logTransition(EngineerSMTransitionPhase::LEAVING_STATE, State::eating, nextState);
    }

    /**
     * 'onEnteringState' callbacks are invoked right before entering a new state. The internal 
     * '_currentState' data still points to the existing state.
     * @param payload mutable payload, ownership remains with the caller. To take ownership of the payload 
     *   override another calback from the 'onEntered*State' below.
     */
    virtual void onEnteringStateWorkingOnTIMER(State nextState, TimerPayload* payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(EngineerSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::working);
    }
    virtual void onEnteringStateSleepingOnTIRED(State nextState, TiredPayload* payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(EngineerSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::sleeping);
    }
    virtual void onEnteringStateEatingOnHUNGRY(State nextState, HungryPayload* payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(EngineerSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::eating);
    }

    /**
     * 'onEnteredState' callbacks are invoked after SM moved to new state. The internal 
     * '_currentState' data already points to the existing state.
     * It is guaranteed that the next transition will not start until this callback returns.
     * It is safe to call postEvent*() to trigger the next transition from this method.
     * @param payload ownership is transferred to the user.
     */
    virtual void onEnteredStateWorkingOnTIMER(TimerPayload&& payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(EngineerSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::working);
    }
    virtual void onEnteredStateSleepingOnTIRED(TiredPayload&& payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(EngineerSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::sleeping);
    }
    virtual void onEnteredStateEatingOnHUNGRY(HungryPayload&& payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(EngineerSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::eating);
    }


    /**
     * All valid transitions from the specified state.
     */
    static inline const std::vector<TransitionToStatesPair>& validTransitionsFrom(EngineerSMState state) {
        switch (state) {
          case EngineerSMState::sleeping:
            return EngineerSMValidTransitionsFromSleepingState();
          case EngineerSMState::working:
            return EngineerSMValidTransitionsFromWorkingState();
          case EngineerSMState::eating:
            return EngineerSMValidTransitionsFromEatingState();
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

template <typename SMSpec>
inline void EngineerSM<SMSpec>::postEventTimer (EngineerSM::TimerPayload&& payload) {
    State currentState;
    {
        std::lock_guard<std::mutex> lck(_lock);
        // If the SM is currently processing another event, adds this one to the queue. The thread processing
        // that event is responsible to drain the queue, this is why we also check for the queue size.
        if (_currentState.blockedForProcessingAnEvent || !_currentState.eventQueue.empty()) {
            std::function<void()> eventCb{[ this, p{std::make_shared<TimerPayload>(std::move(payload))} ] () mutable {
                postEventTimer (std::move(*p));
            }};
            _currentState.eventQueue.emplace_back(eventCb);
            return;  // Returns immediately, the event will be posted asynchronously.
        }

        currentState = _currentState.currentState;
        _currentState.blockedForProcessingAnEvent = true;
    }
    // Event processing is done outside of the '_lock' as the 'blockedForProcessingAnEvent' flag is guarding us.
    _postEventHelper(currentState, EngineerSM::Event::TIMER, std::move(payload));
}

template <typename SMSpec>
inline void EngineerSM<SMSpec>::postEventTired (EngineerSM::TiredPayload&& payload) {
    State currentState;
    {
        std::lock_guard<std::mutex> lck(_lock);
        // If the SM is currently processing another event, adds this one to the queue. The thread processing
        // that event is responsible to drain the queue, this is why we also check for the queue size.
        if (_currentState.blockedForProcessingAnEvent || !_currentState.eventQueue.empty()) {
            std::function<void()> eventCb{[ this, p{std::make_shared<TiredPayload>(std::move(payload))} ] () mutable {
                postEventTired (std::move(*p));
            }};
            _currentState.eventQueue.emplace_back(eventCb);
            return;  // Returns immediately, the event will be posted asynchronously.
        }

        currentState = _currentState.currentState;
        _currentState.blockedForProcessingAnEvent = true;
    }
    // Event processing is done outside of the '_lock' as the 'blockedForProcessingAnEvent' flag is guarding us.
    _postEventHelper(currentState, EngineerSM::Event::TIRED, std::move(payload));
}

template <typename SMSpec>
inline void EngineerSM<SMSpec>::postEventHungry (EngineerSM::HungryPayload&& payload) {
    State currentState;
    {
        std::lock_guard<std::mutex> lck(_lock);
        // If the SM is currently processing another event, adds this one to the queue. The thread processing
        // that event is responsible to drain the queue, this is why we also check for the queue size.
        if (_currentState.blockedForProcessingAnEvent || !_currentState.eventQueue.empty()) {
            std::function<void()> eventCb{[ this, p{std::make_shared<HungryPayload>(std::move(payload))} ] () mutable {
                postEventHungry (std::move(*p));
            }};
            _currentState.eventQueue.emplace_back(eventCb);
            return;  // Returns immediately, the event will be posted asynchronously.
        }

        currentState = _currentState.currentState;
        _currentState.blockedForProcessingAnEvent = true;
    }
    // Event processing is done outside of the '_lock' as the 'blockedForProcessingAnEvent' flag is guarding us.
    _postEventHelper(currentState, EngineerSM::Event::HUNGRY, std::move(payload));
}


template<typename SMSpec>
template<typename Payload>
void EngineerSM<SMSpec>::_postEventHelper (EngineerSM::State state, EngineerSM::Event event, Payload&& payload) {
    std::clog << "START EVENT " << event << std::endl;

    // Step 1: Invoke the guard callback. TODO: implement.

    // Step 2: check if the transition is valid.
    const std::vector<EngineerSMState>* targetStates = nullptr;
    const std::vector<EngineerSMTransitionToStatesPair>& validTransitions = validTransitionsFrom(state);
    for (const auto& transitionEvent : validTransitions) {
        if (transitionEvent.first == event) {
            targetStates = &transitionEvent.second;
        }
    }

    bool foundValidTransition = true;
    if (targetStates == nullptr || targetStates->empty()) {
        logTransition(TransitionPhase::TRANSITION_NOT_FOUND, state, state);
        std::lock_guard<std::mutex> lck(_lock);
        _currentState.blockedForProcessingAnEvent = false;  // We are done.
        foundValidTransition = false;
    }

    if (foundValidTransition) {
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
    }

    // Drain the queue...
    while (true) {
        std::function<void()> nextCallback;
        {
            // Step 7: pick the next event and clear the processing flag.
            std::lock_guard<std::mutex> lck(_lock);
            if (!_currentState.eventQueue.empty()) {
                nextCallback = std::move(_currentState.eventQueue.front());  // Keep the queue not empty.
                _currentState.eventQueue.front() = nullptr;  // Make sure to signal other threads to not work on this queue.
            }
            _currentState.blockedForProcessingAnEvent = false;  // We are done, even though we can have another step.
            if (_currentState.eventQueue.empty()) {
                break;
            }
        }

        if (nextCallback) {
            // Step 8: execute the next callback and then remove it from the queue.
            nextCallback();
            std::lock_guard<std::mutex> lck(_lock);
            assert(_currentState.eventQueue.front() == nullptr);
            _currentState.eventQueue.pop_front();

            if (_currentState.eventQueue.empty()) {
                break;  // No more events to process.
            }
            _currentState.blockedForProcessingAnEvent = true;
        }
    }
    std::clog << "DONE EVENT " << event << std::endl;
}

template<typename SMSpec>
void EngineerSM<SMSpec>::_leavingStateHelper(State fromState, State newState) {
    switch (fromState) {
    case State::sleeping:
        onLeavingSleepingState (newState);
        SMSpec::morningRoutine(this);
        break;
    case State::working:
        onLeavingWorkingState (newState);
        break;
    case State::eating:
        onLeavingEatingState (newState);
        SMSpec::checkEmail(this);
        SMSpec::startHungryTimer(this);
        SMSpec::startTiredTimer(this);
        break;
    }
}

template<typename SMSpec>
void EngineerSM<SMSpec>::_enteringStateHelper(Event event, State newState, void* payload) {
    switch (newState) {
    case State::sleeping:
        SMSpec::startWakeupTimer(this);
        break;
    case State::working:
        SMSpec::checkEmail(this);
        SMSpec::startHungryTimer(this);
        break;
    case State::eating:
        SMSpec::startShortTimer(this);
        break;
    }

    if (event == Event::TIMER && newState == State::working) {
        TimerPayload* typedPayload = static_cast<TimerPayload*>(payload);
        onEnteringStateWorkingOnTIMER(newState, typedPayload);
        return;
    }
    if (event == Event::TIRED && newState == State::sleeping) {
        TiredPayload* typedPayload = static_cast<TiredPayload*>(payload);
        onEnteringStateSleepingOnTIRED(newState, typedPayload);
        return;
    }
    if (event == Event::HUNGRY && newState == State::eating) {
        HungryPayload* typedPayload = static_cast<HungryPayload*>(payload);
        onEnteringStateEatingOnHUNGRY(newState, typedPayload);
        return;
    }
}

template<typename SMSpec>
void EngineerSM<SMSpec>::_transitionActionsHelper(State fromState, Event event, void* payload) {
    if (fromState == State::sleeping && event == Event::TIMER) {
        TimerPayload* typedPayload = static_cast<TimerPayload*>(payload);
        SMSpec::startHungryTimer(this, typedPayload);
    }
    if (fromState == State::sleeping && event == Event::TIMER) {
        TimerPayload* typedPayload = static_cast<TimerPayload*>(payload);
        SMSpec::startTiredTimer(this, typedPayload);
    }
    if (fromState == State::working && event == Event::HUNGRY) {
        HungryPayload* typedPayload = static_cast<HungryPayload*>(payload);
        SMSpec::checkEmail(this, typedPayload);
    }
    if (fromState == State::eating && event == Event::TIMER) {
        TimerPayload* typedPayload = static_cast<TimerPayload*>(payload);
        SMSpec::startHungryTimer(this, typedPayload);
    }
}

template<typename SMSpec>
void EngineerSM<SMSpec>::_enteredStateHelper(Event event, State newState, void* payload) {
    if (event == Event::TIMER && newState == State::working) {
        TimerPayload* typedPayload = static_cast<TimerPayload*>(payload);
        onEnteredStateWorkingOnTIMER(std::move(*typedPayload));
        return;
    }
    if (event == Event::TIRED && newState == State::sleeping) {
        TiredPayload* typedPayload = static_cast<TiredPayload*>(payload);
        onEnteredStateSleepingOnTIRED(std::move(*typedPayload));
        return;
    }
    if (event == Event::HUNGRY && newState == State::eating) {
        HungryPayload* typedPayload = static_cast<HungryPayload*>(payload);
        onEnteredStateEatingOnHUNGRY(std::move(*typedPayload));
        return;
    }
}

template<typename SMSpec>
void EngineerSM<SMSpec>::accessContextLocked(std::function<void(StateMachineContext& userContext)> callback) {
    std::lock_guard<std::mutex> lck(_lock);
    callback(_context);  // User can modify the context under lock.
}

template<typename SMSpec>
void EngineerSM<SMSpec>::logTransition(TransitionPhase phase, State currentState, State nextState) const {
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

