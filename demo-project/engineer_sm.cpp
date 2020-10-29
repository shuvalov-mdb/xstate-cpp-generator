/** 
 * This code is automatically generated using the Xstate to C++ code generator:
 *    https://github.com/shuvalov-mdb/xstate-cpp-generator , @author Andrew Shuvalov
 */

#include "engineer_sm.h"

namespace engineer_demo {

std::string EngineerSMStateToString(EngineerSMState state) {
    switch (state) {
        case EngineerSMState::UNDEFINED_OR_ERROR_STATE:
            return "UNDEFINED";
        case EngineerSMState::sleeping:
            return "EngineerSMState::sleeping";
        case EngineerSMState::working:
            return "EngineerSMState::working";
        case EngineerSMState::eating:
            return "EngineerSMState::eating";
        default:
            return "ERROR";
    }
}

std::ostream& operator << (std::ostream& os, const EngineerSMState& state) {
    os << EngineerSMStateToString(state);
    return os;
}


bool isValidEngineerSMState(EngineerSMState state) {
    if (state == EngineerSMState::UNDEFINED_OR_ERROR_STATE) { return true; }
    if (state == EngineerSMState::sleeping) { return true; }
    if (state == EngineerSMState::working) { return true; }
    if (state == EngineerSMState::eating) { return true; }
    return false;
}

std::string EngineerSMEventToString(EngineerSMEvent event) {
    switch (event) {
        case EngineerSMEvent::UNDEFINED_OR_ERROR_EVENT:
            return "UNDEFINED";
        case EngineerSMEvent::TIMER:
            return "EngineerSMEvent::TIMER";
        case EngineerSMEvent::TIRED:
            return "EngineerSMEvent::TIRED";
        case EngineerSMEvent::HUNGRY:
            return "EngineerSMEvent::HUNGRY";
        default:
            return "ERROR";
    }
}

bool isValidEngineerSMEvent(EngineerSMEvent event) {
    if (event == EngineerSMEvent::UNDEFINED_OR_ERROR_EVENT) { return true; }
    if (event == EngineerSMEvent::TIMER) { return true; }
    if (event == EngineerSMEvent::TIRED) { return true; }
    if (event == EngineerSMEvent::HUNGRY) { return true; }
    return false;
}

std::ostream& operator << (std::ostream& os, const EngineerSMEvent& event) {
    os << EngineerSMEventToString(event);
    return os;
}

std::ostream& operator << (std::ostream& os, const EngineerSMTransitionPhase& phase) {
    switch (phase) {
    case EngineerSMTransitionPhase::LEAVING_STATE:
        os << "Leaving state ";
        break;
    case EngineerSMTransitionPhase::ENTERING_STATE:
        os << "Entering state ";
        break;
    case EngineerSMTransitionPhase::ENTERED_STATE:
        os << "Entered state ";
        break;
    case EngineerSMTransitionPhase::TRANSITION_NOT_FOUND:
        os << "Transition not found ";
        break;
    default:
        os << "ERROR ";
        break;
    }
    return os;
}


// static 
const std::vector<EngineerSMTransitionToStatesPair>& 
EngineerSMValidTransitionsFromSleepingState() {
    static const auto* transitions = new const std::vector<EngineerSMTransitionToStatesPair> {
        { EngineerSMEvent::TIMER, { 
              EngineerSMState::working           } },
        { EngineerSMEvent::TIRED, { 
              EngineerSMState::sleeping           } },
        };
    return *transitions;
}

// static 
const std::vector<EngineerSMTransitionToStatesPair>& 
EngineerSMValidTransitionsFromWorkingState() {
    static const auto* transitions = new const std::vector<EngineerSMTransitionToStatesPair> {
        { EngineerSMEvent::HUNGRY, { 
              EngineerSMState::eating           } },
        { EngineerSMEvent::TIRED, { 
              EngineerSMState::sleeping           } },
        };
    return *transitions;
}

// static 
const std::vector<EngineerSMTransitionToStatesPair>& 
EngineerSMValidTransitionsFromEatingState() {
    static const auto* transitions = new const std::vector<EngineerSMTransitionToStatesPair> {
        { EngineerSMEvent::TIMER, { 
              EngineerSMState::working           } },
        { EngineerSMEvent::TIRED, { 
              EngineerSMState::sleeping           } },
        };
    return *transitions;
}



}  // namespace engineer_demo