// This is an automatically generated header, do not edit.

#include <iostream>
#include <functional>
#include <tuple>
#include <sstream>
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

// State machine as declared in Xstate library for FetchSM.
class FetchSM {
  public:
    // 
    using TransitionToStatesPair = std::pair<FetchSMEvent,
            std::vector<FetchSMState>>;

    FetchSM();

    virtual ~FetchSM();

    /**
     * All valid transitions from the current state of the State Machine.
     */
    const std::vector<TransitionToStatesPair>& validTransitionsFromCurrentState();

    /**
     * All valid transitions from the specified state. The transition to state graph
     * is code genrated from the model and cannot change.
     */
    static const std::vector<TransitionToStatesPair>& validTransitionsFromIdleState();
    static const std::vector<TransitionToStatesPair>& validTransitionsFromLoadingState();
    static const std::vector<TransitionToStatesPair>& validTransitionsFromSuccessState();
    static const std::vector<TransitionToStatesPair>& validTransitionsFromFailureState();

    /**
     * All valid transitions from the specified state.
     */
    static inline const std::vector<TransitionToStatesPair>& validTransitionsFrom(FetchSMState state) {
        switch (state) {
          case FetchSMState::idle:
            return validTransitionsFromIdleState();
          case FetchSMState::loading:
            return validTransitionsFromLoadingState();
          case FetchSMState::success:
            return validTransitionsFromSuccessState();
          case FetchSMState::failure:
            return validTransitionsFromFailureState();
          default: {
            std::stringstream ss;
            ss << "invalid SM state " << state;
            throw std::runtime_error(ss.str());
          } break;
        }
    }
};


}  // namespace

