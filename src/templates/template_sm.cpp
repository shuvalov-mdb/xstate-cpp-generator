/**
 * This code is automatically generated using the Xstate to C++ code generator:
 *    https://github.com/shuvalov-mdb/xstate-cpp-generator , @author Andrew Shuvalov
 */

#include "{{it.properties.pathForIncludes}}{{it.generator.outputHeaderShortname}}"

namespace {{it.properties.namespace }} {

std::string {{it.generator.class()}}StateToString({{it.generator.class()}}State state) {
    switch (state) {
        case {{it.generator.class()}}State::UNDEFINED_OR_ERROR_STATE:
            return "UNDEFINED";
{{@foreach(it.machine.states) => key, val}}
        case {{it.generator.class()}}State::{{key}}:
            return "{{it.generator.class()}}State::{{key}}";
{{/foreach}}
        default:
            return "ERROR";
    }
}

std::ostream& operator << (std::ostream& os, const {{it.generator.class()}}State& state) {
    os << {{it.generator.class()}}StateToString(state);
    return os;
}


bool isValid{{it.generator.class()}}State({{it.generator.class()}}State state) {
    if (state == {{it.generator.class()}}State::UNDEFINED_OR_ERROR_STATE) { return true; }
{{@foreach(it.machine.states) => key, val}}
    if (state == {{it.generator.class()}}State::{{key}}) { return true; }
{{/foreach}}
    return false;
}

std::string {{it.generator.class()}}EventToString({{it.generator.class()}}Event event) {
    switch (event) {
        case {{it.generator.class()}}Event::UNDEFINED_OR_ERROR_EVENT:
            return "UNDEFINED";
{{@each(it.generator.events()) => val, index}}
        case {{it.generator.class()}}Event::{{val}}:
            return "{{it.generator.class()}}Event::{{val}}";
{{/each}}
        default:
            return "ERROR";
    }
}

bool isValid{{it.generator.class()}}Event({{it.generator.class()}}Event event) {
    if (event == {{it.generator.class()}}Event::UNDEFINED_OR_ERROR_EVENT) { return true; }
{{@each(it.generator.events()) => val, index}}
    if (event == {{it.generator.class()}}Event::{{val}}) { return true; }
{{/each}}
    return false;
}

std::ostream& operator << (std::ostream& os, const {{it.generator.class()}}Event& event) {
    os << {{it.generator.class()}}EventToString(event);
    return os;
}

std::ostream& operator << (std::ostream& os, const {{it.generator.class()}}TransitionPhase& phase) {
    switch (phase) {
    case {{it.generator.class()}}TransitionPhase::LEAVING_STATE:
        os << "Leaving state ";
        break;
    case {{it.generator.class()}}TransitionPhase::ENTERING_STATE:
        os << "Entering state ";
        break;
    case {{it.generator.class()}}TransitionPhase::ENTERED_STATE:
        os << "Entered state ";
        break;
    case {{it.generator.class()}}TransitionPhase::TRANSITION_NOT_FOUND:
        os << "Transition not found ";
        break;
    default:
        os << "ERROR ";
        break;
    }
    return os;
}


{{@foreach(it.machine.states) => key, val}}
// static
const std::vector<{{it.generator.class()}}TransitionToStatesPair>&
{{it.generator.class()}}ValidTransitionsFrom{{it.generator.capitalize(key)}}State() {
    static const auto* transitions = new const std::vector<{{it.generator.class()}}TransitionToStatesPair> {
    {{@each(it.generator.transitionsForState(key)) => pair, index}}
    { {{it.generator.class()}}Event::{{pair[0]}}, {
      {{@each(pair[1]) => target, index}}
        {{it.generator.class()}}State::{{target}}
      {{/each}}
     } },
    {{/each}}
    };
    return *transitions;
}

{{/foreach}}


}  // namespace {{it.properties.namespace }}
