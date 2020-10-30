/** 
 * This header is automatically generated using the Xstate to C++ code generator:
 *    https://github.com/shuvalov-mdb/xstate-cpp-generator , @author Andrew Shuvalov
 *
 * Please do not edit. If changes are needed, regenerate using the TypeScript template 'fetch.ts'.
 * Generated at Fri Oct 30 2020 16:43:48 GMT+0000 (Coordinated Universal Time) from Xstate definition 'fetch.ts'.
 * The simplest command line to run the generation:
 *     ts-node 'fetch.ts'
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

namespace mongo {

// All states declared in the SM FetchSM.
enum class FetchSMState {
    UNDEFINED_OR_ERROR_STATE = 0,
    idle,
    loading,
    success,
    failure,
};

std::string FetchSMStateToString(FetchSMState state);

std::ostream& operator << (std::ostream& os, const FetchSMState& state);

// @returns true if 'state' is a valid State.
bool isValidFetchSMState(FetchSMState state);

// All events declared in the SM FetchSM.
enum class FetchSMEvent {
    UNDEFINED_OR_ERROR_EVENT = 0,
    FETCH,
    RESOLVE,
    REJECT,
    RETRY,
};

std::string FetchSMEventToString(FetchSMEvent event);

std::ostream& operator << (std::ostream& os, const FetchSMEvent& event);

// @returns true if 'event' is a valid Event.
bool isValidFetchSMEvent(FetchSMEvent event);

// As a transition could be conditional (https://xstate.js.org/docs/guides/guards.html#guards-condition-functions)
// one event is mapped to a vector of possible transitions.
using FetchSMTransitionToStatesPair = std::pair<FetchSMEvent,
        std::vector<FetchSMState>>;

/**
 * All valid transitions from the specified state. The transition to state graph
 * is code genrated from the model and cannot change.
 */
const std::vector<FetchSMTransitionToStatesPair>& FetchSMValidTransitionsFromIdleState();
const std::vector<FetchSMTransitionToStatesPair>& FetchSMValidTransitionsFromLoadingState();
const std::vector<FetchSMTransitionToStatesPair>& FetchSMValidTransitionsFromSuccessState();
const std::vector<FetchSMTransitionToStatesPair>& FetchSMValidTransitionsFromFailureState();

/**
 * Enum to indicate the current state transition phase in callbacks. This enum is used only for logging
 * and is not part of any State Machine logic.
 */
enum class FetchSMTransitionPhase { 
    UNDEFINED = 0,
    LEAVING_STATE,
    ENTERING_STATE,
    ENTERED_STATE,
    TRANSITION_NOT_FOUND
};

std::ostream& operator << (std::ostream& os, const FetchSMTransitionPhase& phase);

template <typename SMSpec> class FetchSM;  // Forward declaration to use in Spec.

/**
 * Convenient default SM spec structure to parameterize the State Machine.
 * It can be replaced with a custom one if the SM events do not need any payload to be attached, and if there
 * is no guards, and no other advanced features.
 */
template <typename SMContext = std::nullptr_t>
struct DefaultFetchSMSpec {
    /** 
     * Generic data structure stored in the State Machine to keep some user-defined state that can be modified
     * when transitions happen.
     */
    using StateMachineContext = SMContext;

    /**
     * Each Event has a payload attached, which is passed in to the related callbacks.
     * The type should be movable for efficiency.
     */
    using EventFetchPayload = std::nullptr_t;
    using EventResolvePayload = std::nullptr_t;
    using EventRejectPayload = std::nullptr_t;
    using EventRetryPayload = std::nullptr_t;

    /**
     * Actions are modeled in the Xstate definition, see https://xstate.js.org/docs/guides/actions.html.
     * This block is for transition actions.
     */

    /**
     * This block is for entry and exit state actions.
     */
};

/**
 *  State machine as declared in Xstate library for FetchSM.
 *  SMSpec is a convenient template struct, which allows to specify various definitions used by generated code. In a simple
 *  case it's not needed and a convenient default is provided.
 * 
 *  State Machine is not an abstract class and can be used without subclassing at all,
 *  though its functionality will be limited in terms of callbacks.
 *  Even though it's a templated class, a default SMSpec is provided to make a simple
 *  State Machine without any customization. In the most simple form, a working 
 *  FetchSM SM instance can be instantiated and used as in this example:
 * 
 *    FetchSM<> machine;
 *    auto currentState = machine.currentState();
 *    FetchSM<>::FetchPayload payloadFETCH;      // ..and init payload with data
 *    machine.postEventFetch (std::move(payloadFETCH));
 *    FetchSM<>::ResolvePayload payloadRESOLVE;      // ..and init payload with data
 *    machine.postEventResolve (std::move(payloadRESOLVE));
 *    FetchSM<>::RejectPayload payloadREJECT;      // ..and init payload with data
 *    machine.postEventReject (std::move(payloadREJECT));
 *    FetchSM<>::RetryPayload payloadRETRY;      // ..and init payload with data
 *    machine.postEventRetry (std::move(payloadRETRY));
 * 
 *  Also see the generated unit tests in the example-* folders for more example code.
 */
template <typename SMSpec = DefaultFetchSMSpec<std::nullptr_t>>
class FetchSM {
  public:
    using TransitionToStatesPair = FetchSMTransitionToStatesPair;
    using State = FetchSMState;
    using Event = FetchSMEvent;
    using TransitionPhase = FetchSMTransitionPhase;
    using StateMachineContext = typename SMSpec::StateMachineContext;
    using FetchPayload = typename SMSpec::EventFetchPayload;
    using ResolvePayload = typename SMSpec::EventResolvePayload;
    using RejectPayload = typename SMSpec::EventRejectPayload;
    using RetryPayload = typename SMSpec::EventRetryPayload;

    /**
     * Structure represents the current in-memory state of the State Machine.
     */
    struct CurrentState {
        State currentState = FetchSMState::idle;
        /** previousState could be undefined if SM is at initial state */
        State previousState;
        /** The event that transitioned the SM from previousState to currentState */
        Event lastEvent;
        /** Timestamp of the last transition, or the class instantiation if at initial state */
        std::chrono::system_clock::time_point lastTransitionTime = std::chrono::system_clock::now();
        /** Count of the transitions made so far */
        int totalTransitions = 0;
    };

    FetchSM() {
        _eventsConsumerThread = std::make_unique<std::thread>([this] {
            _eventsConsumerThreadLoop();  // Start when all class members are initialized.
        });
    }

    virtual ~FetchSM() {
        for (int i = 0; i < 10; ++i) {
            if (isTerminated()) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        if (!isTerminated()) {
            std::cerr << "State Machine FetchSM is terminating "
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
    void postEventFetch (std::shared_ptr<FetchPayload> payload);
    void postEventResolve (std::shared_ptr<ResolvePayload> payload);
    void postEventReject (std::shared_ptr<RejectPayload> payload);
    void postEventRetry (std::shared_ptr<RetryPayload> payload);

    /**
     * All valid transitions from the current state of the State Machine.
     */
    const std::vector<FetchSMTransitionToStatesPair>& validTransitionsFromCurrentState() const {
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
    virtual void onLeavingIdleState(State nextState) {
        logTransition(FetchSMTransitionPhase::LEAVING_STATE, State::idle, nextState);
    }
    virtual void onLeavingLoadingState(State nextState) {
        logTransition(FetchSMTransitionPhase::LEAVING_STATE, State::loading, nextState);
    }
    virtual void onLeavingSuccessState(State nextState) {
        logTransition(FetchSMTransitionPhase::LEAVING_STATE, State::success, nextState);
    }
    virtual void onLeavingFailureState(State nextState) {
        logTransition(FetchSMTransitionPhase::LEAVING_STATE, State::failure, nextState);
    }

    /**
     * 'onEnteringState' callbacks are invoked right before entering a new state. The internal 
     * '_currentState' data still points to the existing state.
     * @param payload mutable payload, ownership remains with the caller. To take ownership of the payload 
     *   override another calback from the 'onEntered*State' below.
     */
    virtual void onEnteringStateLoadingOnFETCH(State nextState, std::shared_ptr<FetchPayload> payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::loading);
    }
    virtual void onEnteringStateSuccessOnRESOLVE(State nextState, std::shared_ptr<ResolvePayload> payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::success);
    }
    virtual void onEnteringStateFailureOnREJECT(State nextState, std::shared_ptr<RejectPayload> payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::failure);
    }
    virtual void onEnteringStateLoadingOnRETRY(State nextState, std::shared_ptr<RetryPayload> payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::loading);
    }

    /**
     * 'onEnteredState' callbacks are invoked after SM moved to new state. The internal 
     * '_currentState' data already points to the existing state.
     * It is guaranteed that the next transition will not start until this callback returns.
     * It is safe to call postEvent*() to trigger the next transition from this method.
     * @param payload ownership is transferred to the user.
     */
    virtual void onEnteredStateLoadingOnFETCH(std::shared_ptr<FetchPayload> payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::loading);
    }
    virtual void onEnteredStateSuccessOnRESOLVE(std::shared_ptr<ResolvePayload> payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::success);
    }
    virtual void onEnteredStateFailureOnREJECT(std::shared_ptr<RejectPayload> payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::failure);
    }
    virtual void onEnteredStateLoadingOnRETRY(std::shared_ptr<RetryPayload> payload) {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::loading);
    }


    /**
     * All valid transitions from the specified state.
     */
    static inline const std::vector<TransitionToStatesPair>& validTransitionsFrom(FetchSMState state) {
        switch (state) {
          case FetchSMState::idle:
            return FetchSMValidTransitionsFromIdleState();
          case FetchSMState::loading:
            return FetchSMValidTransitionsFromLoadingState();
          case FetchSMState::success:
            return FetchSMValidTransitionsFromSuccessState();
          case FetchSMState::failure:
            return FetchSMValidTransitionsFromFailureState();
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

template <typename SMSpec>
inline void FetchSM<SMSpec>::postEventFetch (std::shared_ptr<FetchSM::FetchPayload> payload) {
    if (_insideAccessContextLocked) {
        // Intentianally not locked, we are checking for deadline here...
        static constexpr char error[] = "It is prohibited to post an event from inside the accessContextLocked()";
        std::cerr << error << std::endl;
        assert(false);
    }
    std::lock_guard<std::mutex> lck(_lock);
    State currentState = _currentState.currentState;
    std::function<void()> eventCb{[ this, currentState, payload ] () mutable {
        _postEventHelper(currentState, FetchSM::Event::FETCH, payload);
    }};
    _eventQueue.emplace(eventCb);
    _eventQueueCondvar.notify_one();
}

template <typename SMSpec>
inline void FetchSM<SMSpec>::postEventResolve (std::shared_ptr<FetchSM::ResolvePayload> payload) {
    if (_insideAccessContextLocked) {
        // Intentianally not locked, we are checking for deadline here...
        static constexpr char error[] = "It is prohibited to post an event from inside the accessContextLocked()";
        std::cerr << error << std::endl;
        assert(false);
    }
    std::lock_guard<std::mutex> lck(_lock);
    State currentState = _currentState.currentState;
    std::function<void()> eventCb{[ this, currentState, payload ] () mutable {
        _postEventHelper(currentState, FetchSM::Event::RESOLVE, payload);
    }};
    _eventQueue.emplace(eventCb);
    _eventQueueCondvar.notify_one();
}

template <typename SMSpec>
inline void FetchSM<SMSpec>::postEventReject (std::shared_ptr<FetchSM::RejectPayload> payload) {
    if (_insideAccessContextLocked) {
        // Intentianally not locked, we are checking for deadline here...
        static constexpr char error[] = "It is prohibited to post an event from inside the accessContextLocked()";
        std::cerr << error << std::endl;
        assert(false);
    }
    std::lock_guard<std::mutex> lck(_lock);
    State currentState = _currentState.currentState;
    std::function<void()> eventCb{[ this, currentState, payload ] () mutable {
        _postEventHelper(currentState, FetchSM::Event::REJECT, payload);
    }};
    _eventQueue.emplace(eventCb);
    _eventQueueCondvar.notify_one();
}

template <typename SMSpec>
inline void FetchSM<SMSpec>::postEventRetry (std::shared_ptr<FetchSM::RetryPayload> payload) {
    if (_insideAccessContextLocked) {
        // Intentianally not locked, we are checking for deadline here...
        static constexpr char error[] = "It is prohibited to post an event from inside the accessContextLocked()";
        std::cerr << error << std::endl;
        assert(false);
    }
    std::lock_guard<std::mutex> lck(_lock);
    State currentState = _currentState.currentState;
    std::function<void()> eventCb{[ this, currentState, payload ] () mutable {
        _postEventHelper(currentState, FetchSM::Event::RETRY, payload);
    }};
    _eventQueue.emplace(eventCb);
    _eventQueueCondvar.notify_one();
}


template<typename SMSpec>
template<typename Payload>
void FetchSM<SMSpec>::_postEventHelper (FetchSM::State state, 
    FetchSM::Event event, std::shared_ptr<Payload> payload) {

    // Step 1: Invoke the guard callback. TODO: implement.

    // Step 2: check if the transition is valid.
    const std::vector<FetchSMState>* targetStates = nullptr;
    const std::vector<FetchSMTransitionToStatesPair>& validTransitions = validTransitionsFrom(state);
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
        if (newState == State::success) {
            _smIsTerminated = true;
            _eventQueueCondvar.notify_one();  // SM will be terminated...
        }
    }

    // Step 6: Invoke the 'entered the state' callback.
    _enteredStateHelper(event, newState, &payload);
}

template<typename SMSpec>
void FetchSM<SMSpec>::_eventsConsumerThreadLoop() {
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
void FetchSM<SMSpec>::_leavingStateHelper(State fromState, State newState) {
    switch (fromState) {
    case State::idle:
        onLeavingIdleState (newState);
        break;
    case State::loading:
        onLeavingLoadingState (newState);
        break;
    case State::success:
        onLeavingSuccessState (newState);
        break;
    case State::failure:
        onLeavingFailureState (newState);
        break;
    }
}

template<typename SMSpec>
void FetchSM<SMSpec>::_enteringStateHelper(Event event, State newState, void* payload) {
    switch (newState) {
    case State::idle:
        break;
    case State::loading:
        break;
    case State::success:
        break;
    case State::failure:
        break;
    }

    if (event == Event::FETCH && newState == State::loading) {
        std::shared_ptr<FetchPayload>* typedPayload = static_cast<std::shared_ptr<FetchPayload>*>(payload);
        onEnteringStateLoadingOnFETCH(newState, *typedPayload);
        return;
    }
    if (event == Event::RESOLVE && newState == State::success) {
        std::shared_ptr<ResolvePayload>* typedPayload = static_cast<std::shared_ptr<ResolvePayload>*>(payload);
        onEnteringStateSuccessOnRESOLVE(newState, *typedPayload);
        return;
    }
    if (event == Event::REJECT && newState == State::failure) {
        std::shared_ptr<RejectPayload>* typedPayload = static_cast<std::shared_ptr<RejectPayload>*>(payload);
        onEnteringStateFailureOnREJECT(newState, *typedPayload);
        return;
    }
    if (event == Event::RETRY && newState == State::loading) {
        std::shared_ptr<RetryPayload>* typedPayload = static_cast<std::shared_ptr<RetryPayload>*>(payload);
        onEnteringStateLoadingOnRETRY(newState, *typedPayload);
        return;
    }
}

template<typename SMSpec>
void FetchSM<SMSpec>::_transitionActionsHelper(State fromState, Event event, void* payload) {
}

template<typename SMSpec>
void FetchSM<SMSpec>::_enteredStateHelper(Event event, State newState, void* payload) {
    if (event == Event::FETCH && newState == State::loading) {
        std::shared_ptr<FetchPayload>* typedPayload = static_cast<std::shared_ptr<FetchPayload>*>(payload);
        onEnteredStateLoadingOnFETCH(*typedPayload);
        return;
    }
    if (event == Event::RESOLVE && newState == State::success) {
        std::shared_ptr<ResolvePayload>* typedPayload = static_cast<std::shared_ptr<ResolvePayload>*>(payload);
        onEnteredStateSuccessOnRESOLVE(*typedPayload);
        return;
    }
    if (event == Event::REJECT && newState == State::failure) {
        std::shared_ptr<RejectPayload>* typedPayload = static_cast<std::shared_ptr<RejectPayload>*>(payload);
        onEnteredStateFailureOnREJECT(*typedPayload);
        return;
    }
    if (event == Event::RETRY && newState == State::loading) {
        std::shared_ptr<RetryPayload>* typedPayload = static_cast<std::shared_ptr<RetryPayload>*>(payload);
        onEnteredStateLoadingOnRETRY(*typedPayload);
        return;
    }
}

template<typename SMSpec>
void FetchSM<SMSpec>::accessContextLocked(std::function<void(StateMachineContext& userContext)> callback) {
    std::lock_guard<std::mutex> lck(_lock);
    // This variable is preventing the user from posting an event while inside the callback,
    // as it will be a deadlock.
    _insideAccessContextLocked = true;
    callback(_context);  // User can modify the context under lock.
    _insideAccessContextLocked = false;
}

template<typename SMSpec>
void FetchSM<SMSpec>::logTransition(TransitionPhase phase, State currentState, State nextState) const {
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

