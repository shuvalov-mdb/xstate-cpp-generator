/** 
 * This code is automatically generated using the Xstate to C++ code generator:
 *    https://github.com/shuvalov-mdb/xstate-cpp-generator , @author Andrew Shuvalov
 */

#include "example-fetch/fetch_sm.h"

namespace mongo {

std::string FetchSMStateToString(FetchSMState state) {
    switch (state) {
        case FetchSMState::UNDEFINED_OR_ERROR_STATE:
            return "UNDEFINED";
        case FetchSMState::idle:
            return "FetchSMState::idle";
        case FetchSMState::loading:
            return "FetchSMState::loading";
        case FetchSMState::success:
            return "FetchSMState::success";
        case FetchSMState::failure:
            return "FetchSMState::failure";
        default:
            return "ERROR";
    }
}

std::ostream& operator << (std::ostream& os, const FetchSMState& state) {
    os << FetchSMStateToString(state);
    return os;
}


bool isValidFetchSMState(FetchSMState state) {
    if (state == FetchSMState::UNDEFINED_OR_ERROR_STATE) { return true; }
    if (state == FetchSMState::idle) { return true; }
    if (state == FetchSMState::loading) { return true; }
    if (state == FetchSMState::success) { return true; }
    if (state == FetchSMState::failure) { return true; }
    return false;
}

std::string FetchSMEventToString(FetchSMEvent event) {
    switch (event) {
        case FetchSMEvent::UNDEFINED_OR_ERROR_EVENT:
            return "UNDEFINED";
        case FetchSMEvent::FETCH:
            return "FetchSMEvent::FETCH";
        case FetchSMEvent::RESOLVE:
            return "FetchSMEvent::RESOLVE";
        case FetchSMEvent::REJECT:
            return "FetchSMEvent::REJECT";
        case FetchSMEvent::RETRY:
            return "FetchSMEvent::RETRY";
        default:
            return "ERROR";
    }
}

bool isValidFetchSMEvent(FetchSMEvent event) {
    if (event == FetchSMEvent::UNDEFINED_OR_ERROR_EVENT) { return true; }
    if (event == FetchSMEvent::FETCH) { return true; }
    if (event == FetchSMEvent::RESOLVE) { return true; }
    if (event == FetchSMEvent::REJECT) { return true; }
    if (event == FetchSMEvent::RETRY) { return true; }
    return false;
}

std::ostream& operator << (std::ostream& os, const FetchSMEvent& event) {
    os << FetchSMEventToString(event);
    return os;
}

std::ostream& operator << (std::ostream& os, const FetchSMTransitionPhase& phase) {
    switch (phase) {
    case FetchSMTransitionPhase::LEAVING_STATE:
        os << "Leaving state ";
        break;
    case FetchSMTransitionPhase::ENTERING_STATE:
        os << "Entering state ";
        break;
    case FetchSMTransitionPhase::ENTERED_STATE:
        os << "Entered state ";
        break;
    case FetchSMTransitionPhase::TRANSITION_NOT_FOUND:
        os << "Transition not found ";
        break;
    default:
        os << "ERROR ";
        break;
    }
    return os;
}


// static 
const std::vector<FetchSMTransitionToStatesPair>& 
FetchSMValidTransitionsFromIdleState() {
    static const auto* transitions = new const std::vector<FetchSMTransitionToStatesPair> {
        { FetchSMEvent::FETCH, { 
              FetchSMState::loading           } },
        };
    return *transitions;
}

// static 
const std::vector<FetchSMTransitionToStatesPair>& 
FetchSMValidTransitionsFromLoadingState() {
    static const auto* transitions = new const std::vector<FetchSMTransitionToStatesPair> {
        { FetchSMEvent::RESOLVE, { 
              FetchSMState::success           } },
        { FetchSMEvent::REJECT, { 
              FetchSMState::failure           } },
        };
    return *transitions;
}

// static 
const std::vector<FetchSMTransitionToStatesPair>& 
FetchSMValidTransitionsFromSuccessState() {
    static const auto* transitions = new const std::vector<FetchSMTransitionToStatesPair> {
        };
    return *transitions;
}

// static 
const std::vector<FetchSMTransitionToStatesPair>& 
FetchSMValidTransitionsFromFailureState() {
    static const auto* transitions = new const std::vector<FetchSMTransitionToStatesPair> {
        { FetchSMEvent::RETRY, { 
              FetchSMState::loading           } },
        };
    return *transitions;
}



}  // namespace mongo