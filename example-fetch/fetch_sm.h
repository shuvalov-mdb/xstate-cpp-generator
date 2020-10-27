// This is an automatically generated header, do not edit.

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
 * Enum to indicate the current state transition phase in callbacks.
 */
enum class FetchSMTransitionPhase { 
    UNDEFINED = 0,
    LEAVING_STATE,
    ENTERING_STATE,
    ENTERED_STATE,
    TRANSITION_NOT_FOUND
};

std::ostream& operator << (std::ostream& os, const FetchSMTransitionPhase& phase);

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
    using EventFetchPayload = std::unique_ptr<std::nullptr_t>;
    using EventResolvePayload = std::unique_ptr<std::nullptr_t>;
    using EventRejectPayload = std::unique_ptr<std::nullptr_t>;
    using EventRetryPayload = std::unique_ptr<std::nullptr_t>;
};

/**
 *  State machine as declared in Xstate library for FetchSM.
 *  SMSpec is a convenient template struct, which allows to specify various definitions used by generated code. In a simple
 *  case it's not needed and a convenient default is provided.
 */
template <typename SMSpec = DefaultFetchSMSpec<std::nullptr_t>>
class FetchSM {
  public:
    using TransitionToStatesPair = FetchSMTransitionToStatesPair;
    using State = FetchSMState;
    using Event = FetchSMEvent;
    using TransitionPhase = FetchSMTransitionPhase;
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

    FetchSM() {}

    virtual ~FetchSM() {}

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
    void postEventFetch (FetchPayload&& payload);
    void postEventResolve (ResolvePayload&& payload);
    void postEventReject (RejectPayload&& payload);
    void postEventRetry (RetryPayload&& payload);

    /**
     * All valid transitions from the current state of the State Machine.
     */
    const std::vector<FetchSMTransitionToStatesPair>& validTransitionsFromCurrentState() const {
        std::lock_guard<std::mutex> lck(_lock);
        return validTransitionsFrom(_currentState.currentState);
    }

    /**
     * The block of virtual callback methods the derived class can override to extend the SM functionality.
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
    virtual void onLeavingIdleState(State nextState) const {
        logTransition(FetchSMTransitionPhase::LEAVING_STATE, State::idle, nextState);
    }
    virtual void onLeavingLoadingState(State nextState) const {
        logTransition(FetchSMTransitionPhase::LEAVING_STATE, State::loading, nextState);
    }
    virtual void onLeavingSuccessState(State nextState) const {
        logTransition(FetchSMTransitionPhase::LEAVING_STATE, State::success, nextState);
    }
    virtual void onLeavingFailureState(State nextState) const {
        logTransition(FetchSMTransitionPhase::LEAVING_STATE, State::failure, nextState);
    }

    /**
     * 'onEnteringState' callbacks are invoked right before entering a new state. The internal 
     * '_currentState' data still points to the existing state.
     * @param payload mutable payload, ownership remains with the caller. To take ownership of the payload to override 
     *   another calback from the 'onEntered*State' below.
     */
    virtual void onEnteringStateLoadingOnFETCH(State nextState, FetchPayload* payload) const {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::loading);
    }
    virtual void onEnteringStateSuccessOnRESOLVE(State nextState, ResolvePayload* payload) const {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::success);
    }
    virtual void onEnteringStateFailureOnREJECT(State nextState, RejectPayload* payload) const {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::failure);
    }
    virtual void onEnteringStateLoadingOnRETRY(State nextState, RetryPayload* payload) const {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::loading);
    }

    /**
     * 'onEnteredState' callbacks are invoked after SM moved to new state. The internal 
     * '_currentState' data already points to the existing state.
     * @param payload ownership is transferred to the user.
     */
    virtual void onEnteredStateLoadingOnFETCH(FetchPayload&& payload) const {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::loading);
    }
    virtual void onEnteredStateSuccessOnRESOLVE(ResolvePayload&& payload) const {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::success);
    }
    virtual void onEnteredStateFailureOnREJECT(RejectPayload&& payload) const {
        std::lock_guard<std::mutex> lck(_lock);
        logTransition(FetchSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::failure);
    }
    virtual void onEnteredStateLoadingOnRETRY(RetryPayload&& payload) const {
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
    void _postEventHelper(State state, Event event, Payload&& payload);

    void _leavingStateHelper(State fromState, State newState);

    // The implementation will cast the void* of 'payload' back to full type to invoke the callback.
    void _enteringStateHelper(Event event, State newState, void* payload);

    // The implementation will cast the void* of 'payload' back to full type to invoke the callback.
    void _enteredStateHelper(Event event, State newState, void* payload);

    mutable std::mutex _lock;

    CurrentState _currentState;
    // Arbitrary user-defined data structure, see above.
    typename SMSpec::StateMachineContext _context;
};

template <typename SMSpec>
inline void FetchSM<SMSpec>::postEventFetch (FetchSM::FetchPayload&& payload) {
    State currentState;
    {
        std::lock_guard<std::mutex> lck(_lock);
        // If the SM is currently processing another event, adds this one to the queue. The thread processing
        // that event is responsible to drain the queue, this is why we also check for the queue size.
        if (_currentState.blockedForProcessingAnEvent || !_currentState.eventQueue.empty()) {
            std::function<void()> eventCb{[ this, p{std::make_shared<FetchPayload>(std::move(payload))} ] () mutable {
                postEventFetch (std::move(*p));
            }};
            _currentState.eventQueue.emplace_back(eventCb);
            return;  // Returns immediately, the event will be posted asynchronously.
        }

        currentState = _currentState.currentState;
        _currentState.blockedForProcessingAnEvent = true;
    }
    // Event processing is done outside of the '_lock' as the 'blockedForProcessingAnEvent' flag is guarding us.
    _postEventHelper(currentState, FetchSM::Event::FETCH, std::move(payload));
}

template <typename SMSpec>
inline void FetchSM<SMSpec>::postEventResolve (FetchSM::ResolvePayload&& payload) {
    State currentState;
    {
        std::lock_guard<std::mutex> lck(_lock);
        // If the SM is currently processing another event, adds this one to the queue. The thread processing
        // that event is responsible to drain the queue, this is why we also check for the queue size.
        if (_currentState.blockedForProcessingAnEvent || !_currentState.eventQueue.empty()) {
            std::function<void()> eventCb{[ this, p{std::make_shared<ResolvePayload>(std::move(payload))} ] () mutable {
                postEventResolve (std::move(*p));
            }};
            _currentState.eventQueue.emplace_back(eventCb);
            return;  // Returns immediately, the event will be posted asynchronously.
        }

        currentState = _currentState.currentState;
        _currentState.blockedForProcessingAnEvent = true;
    }
    // Event processing is done outside of the '_lock' as the 'blockedForProcessingAnEvent' flag is guarding us.
    _postEventHelper(currentState, FetchSM::Event::RESOLVE, std::move(payload));
}

template <typename SMSpec>
inline void FetchSM<SMSpec>::postEventReject (FetchSM::RejectPayload&& payload) {
    State currentState;
    {
        std::lock_guard<std::mutex> lck(_lock);
        // If the SM is currently processing another event, adds this one to the queue. The thread processing
        // that event is responsible to drain the queue, this is why we also check for the queue size.
        if (_currentState.blockedForProcessingAnEvent || !_currentState.eventQueue.empty()) {
            std::function<void()> eventCb{[ this, p{std::make_shared<RejectPayload>(std::move(payload))} ] () mutable {
                postEventReject (std::move(*p));
            }};
            _currentState.eventQueue.emplace_back(eventCb);
            return;  // Returns immediately, the event will be posted asynchronously.
        }

        currentState = _currentState.currentState;
        _currentState.blockedForProcessingAnEvent = true;
    }
    // Event processing is done outside of the '_lock' as the 'blockedForProcessingAnEvent' flag is guarding us.
    _postEventHelper(currentState, FetchSM::Event::REJECT, std::move(payload));
}

template <typename SMSpec>
inline void FetchSM<SMSpec>::postEventRetry (FetchSM::RetryPayload&& payload) {
    State currentState;
    {
        std::lock_guard<std::mutex> lck(_lock);
        // If the SM is currently processing another event, adds this one to the queue. The thread processing
        // that event is responsible to drain the queue, this is why we also check for the queue size.
        if (_currentState.blockedForProcessingAnEvent || !_currentState.eventQueue.empty()) {
            std::function<void()> eventCb{[ this, p{std::make_shared<RetryPayload>(std::move(payload))} ] () mutable {
                postEventRetry (std::move(*p));
            }};
            _currentState.eventQueue.emplace_back(eventCb);
            return;  // Returns immediately, the event will be posted asynchronously.
        }

        currentState = _currentState.currentState;
        _currentState.blockedForProcessingAnEvent = true;
    }
    // Event processing is done outside of the '_lock' as the 'blockedForProcessingAnEvent' flag is guarding us.
    _postEventHelper(currentState, FetchSM::Event::RETRY, std::move(payload));
}


template<typename SMSpec>
template<typename Payload>
void FetchSM<SMSpec>::_postEventHelper (FetchSM::State state, FetchSM::Event event, Payload&& payload) {

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
        std::lock_guard<std::mutex> lck(_lock);
        _currentState.blockedForProcessingAnEvent = false;  // We are done.
        return;
    }
    State newState = (*targetStates)[0];

    // Step 3: Invoke the 'leaving the state' callback.
    _leavingStateHelper(state, newState);

    // Step 4: Invoke the 'entering the state' callback.
    _enteringStateHelper(event, newState, &payload);

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
    if (event == Event::FETCH && newState == State::loading) {
        FetchPayload* typedPayload = static_cast<FetchPayload*>(payload);
        onEnteringStateLoadingOnFETCH(newState, typedPayload);
        return;
    }
    if (event == Event::RESOLVE && newState == State::success) {
        ResolvePayload* typedPayload = static_cast<ResolvePayload*>(payload);
        onEnteringStateSuccessOnRESOLVE(newState, typedPayload);
        return;
    }
    if (event == Event::REJECT && newState == State::failure) {
        RejectPayload* typedPayload = static_cast<RejectPayload*>(payload);
        onEnteringStateFailureOnREJECT(newState, typedPayload);
        return;
    }
    if (event == Event::RETRY && newState == State::loading) {
        RetryPayload* typedPayload = static_cast<RetryPayload*>(payload);
        onEnteringStateLoadingOnRETRY(newState, typedPayload);
        return;
    }
}

template<typename SMSpec>
void FetchSM<SMSpec>::_enteredStateHelper(Event event, State newState, void* payload) {
    if (event == Event::FETCH && newState == State::loading) {
        FetchPayload* typedPayload = static_cast<FetchPayload*>(payload);
        onEnteredStateLoadingOnFETCH(std::move(*typedPayload));
        return;
    }
    if (event == Event::RESOLVE && newState == State::success) {
        ResolvePayload* typedPayload = static_cast<ResolvePayload*>(payload);
        onEnteredStateSuccessOnRESOLVE(std::move(*typedPayload));
        return;
    }
    if (event == Event::REJECT && newState == State::failure) {
        RejectPayload* typedPayload = static_cast<RejectPayload*>(payload);
        onEnteredStateFailureOnREJECT(std::move(*typedPayload));
        return;
    }
    if (event == Event::RETRY && newState == State::loading) {
        RetryPayload* typedPayload = static_cast<RetryPayload*>(payload);
        onEnteredStateLoadingOnRETRY(std::move(*typedPayload));
        return;
    }
}

template<typename SMSpec>
void FetchSM<SMSpec>::logTransition(TransitionPhase phase, State currentState, State nextState) const {
    switch (phase) {
    case TransitionPhase::LEAVING_STATE:
        std::cout << phase << currentState << ", transitioning to " << nextState;
        break;
    case TransitionPhase::ENTERING_STATE:
        std::cout << phase << nextState << " from " << currentState;
        break;
    case TransitionPhase::ENTERED_STATE:
        std::cout << phase << currentState;
        break;
    default:
        std::cout << "ERROR ";
        break;
    }
    std::cout << std::endl;
}


}  // namespace

