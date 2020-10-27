// This is an automatically generated header, do not edit.

#pragma once

#include <iostream>
#include <deque>
#include <functional>
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

    FetchSM() {}

    virtual ~FetchSM() {}

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
    void _postEventHelper (Event event, Payload&& payload);

    mutable std::mutex _lock;

    CurrentState _currentState;
    // Arbitrary user-defined data structure, see above.
    typename SMSpec::StateMachineContext _context;
};

template <typename SMSpec>
inline void FetchSM<SMSpec>::postEventFetch (FetchSM::FetchPayload&& payload) {
    _postEventHelper(FetchSM::Event::FETCH, std::move(payload));
}

template <typename SMSpec>
inline void FetchSM<SMSpec>::postEventResolve (FetchSM::ResolvePayload&& payload) {
    _postEventHelper(FetchSM::Event::RESOLVE, std::move(payload));
}

template <typename SMSpec>
inline void FetchSM<SMSpec>::postEventReject (FetchSM::RejectPayload&& payload) {
    _postEventHelper(FetchSM::Event::REJECT, std::move(payload));
}

template <typename SMSpec>
inline void FetchSM<SMSpec>::postEventRetry (FetchSM::RetryPayload&& payload) {
    _postEventHelper(FetchSM::Event::RETRY, std::move(payload));
}


template<typename SMSpec>
template<typename Payload>
void FetchSM<SMSpec>::_postEventHelper (FetchSM::Event event, Payload&& payload) {
    std::lock_guard<std::mutex> lck(_lock);
    const bool processNow = _currentState.eventQueue.empty();
}


}  // namespace

