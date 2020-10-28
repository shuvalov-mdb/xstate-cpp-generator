/** 
 * This code is automatically generated using the Xstate to C++ code generator:
 *    https://github.com/shuvalov-mdb/xstate-cpp-generator , @author Andrew Shuvalov
 */

#include "example-ping-pong/ping_sm.h"

namespace mongo {

std::string PingSMStateToString(PingSMState state) {
    switch (state) {
        case PingSMState::UNDEFINED_OR_ERROR_STATE:
            return "UNDEFINED";
        case PingSMState::init:
            return "PingSMState::init";
        case PingSMState::pinging:
            return "PingSMState::pinging";
        default:
            return "ERROR";
    }
}

std::ostream& operator << (std::ostream& os, const PingSMState& state) {
    os << PingSMStateToString(state);
    return os;
}


bool isValidPingSMState(PingSMState state) {
    if (state == PingSMState::UNDEFINED_OR_ERROR_STATE) { return true; }
    if (state == PingSMState::init) { return true; }
    if (state == PingSMState::pinging) { return true; }
    return false;
}

std::string PingSMEventToString(PingSMEvent event) {
    switch (event) {
        case PingSMEvent::UNDEFINED_OR_ERROR_EVENT:
            return "UNDEFINED";
        case PingSMEvent::START:
            return "PingSMEvent::START";
        case PingSMEvent::PONG:
            return "PingSMEvent::PONG";
        default:
            return "ERROR";
    }
}

bool isValidPingSMEvent(PingSMEvent event) {
    if (event == PingSMEvent::UNDEFINED_OR_ERROR_EVENT) { return true; }
    if (event == PingSMEvent::START) { return true; }
    if (event == PingSMEvent::PONG) { return true; }
    return false;
}

std::ostream& operator << (std::ostream& os, const PingSMEvent& event) {
    os << PingSMEventToString(event);
    return os;
}

std::ostream& operator << (std::ostream& os, const PingSMTransitionPhase& phase) {
    switch (phase) {
    case PingSMTransitionPhase::LEAVING_STATE:
        os << "Leaving state ";
        break;
    case PingSMTransitionPhase::ENTERING_STATE:
        os << "Entering state ";
        break;
    case PingSMTransitionPhase::ENTERED_STATE:
        os << "Entered state ";
        break;
    case PingSMTransitionPhase::TRANSITION_NOT_FOUND:
        os << "Transition not found ";
        break;
    default:
        os << "ERROR ";
        break;
    }
    return os;
}


// static 
const std::vector<PingSMTransitionToStatesPair>& 
PingSMValidTransitionsFromInitState() {
    static const auto* transitions = new const std::vector<PingSMTransitionToStatesPair> {
        { PingSMEvent::START, { 
              PingSMState::pinging           } },
        };
    return *transitions;
}

// static 
const std::vector<PingSMTransitionToStatesPair>& 
PingSMValidTransitionsFromPingingState() {
    static const auto* transitions = new const std::vector<PingSMTransitionToStatesPair> {
        { PingSMEvent::PONG, { 
              PingSMState::pinging           } },
        };
    return *transitions;
}



}  // namespace mongo