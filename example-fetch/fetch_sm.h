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

std::ostream& operator << (std::ostream& os, const FetchSMState& state);

// All transitions declared in the SM FetchSM.
enum class FetchSMTransition {
    UNDEFINED_OR_ERROR_TRANSITION = 0,
    FETCH,
    RESOLVE,
    REJECT,
    RETRY,
};

std::ostream& operator << (std::ostream& os, const FetchSMTransition& transition);

// State machine as declared in Xstate library for FetchSM.
class FetchSM {
  public:
    using TransitionToStatePair = std::tuple<FetchSMTransition, FetchSMState>;

    FetchSM();

    virtual ~FetchSM();

    /**
     * All valid transitions from the current state of the State Machine.
     */
    const std::vector<TransitionToStatePair>& validTransitionsFromCurrentState();

    /**
     * All valid transitions from the specified state. The transition to state graph
     * is code genrated from the model and cannot change.
     */
    static const std::vector<TransitionToStatePair>& validTransitionsFromIdleState();
    static const std::vector<TransitionToStatePair>& validTransitionsFromLoadingState();
    static const std::vector<TransitionToStatePair>& validTransitionsFromSuccessState();
    static const std::vector<TransitionToStatePair>& validTransitionsFromFailureState();

    /**
     * All valid transitions from the specified state.
     */
    static inline const std::vector<TransitionToStatePair>& validTransitionsFrom(FetchSMState state) {
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

