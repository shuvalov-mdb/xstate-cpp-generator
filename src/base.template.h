// This is an automatically generated header, do not edit.

#include <iostream>
#include <functional>
#include <tuple>
#include <sstream>
#include <vector>

namespace {{it.properties.namespace }} {

// All states declared in the SM {{it.generator.class()}}.
enum class {{it.generator.class()}}State {
    UNDEFINED_OR_ERROR_STATE = 0,
{{@foreach(it.machine.states) => key, val}}
    {{key}},
{{/foreach}}
};

std::string {{it.generator.class()}}StateToString({{it.generator.class()}}State state);

std::ostream& operator << (std::ostream& os, const {{it.generator.class()}}State& state);

// @returns true if 'state' is a valid State.
bool isValid{{it.generator.class()}}State({{it.generator.class()}}State state);

// All events declared in the SM {{it.generator.class()}}.
enum class {{it.generator.class()}}Event {
    UNDEFINED_OR_ERROR_EVENT = 0,
{{@each(it.generator.events()) => val, index}}
    {{val}},
{{/each}}
};

std::string {{it.generator.class()}}EventToString({{it.generator.class()}}Event event);

std::ostream& operator << (std::ostream& os, const {{it.generator.class()}}Event& event);

// @returns true if 'event' is a valid Event.
bool isValid{{it.generator.class()}}Event({{it.generator.class()}}Event event);

// State machine as declared in Xstate library for {{it.generator.class()}}.
class {{it.generator.class()}} {
  public:
    // 
    using TransitionToStatesPair = std::pair<{{it.generator.class()}}Event,
            std::vector<{{it.generator.class()}}State>>;

    {{it.generator.class()}}();

    virtual ~{{it.generator.class()}}();

    /**
     * All valid transitions from the current state of the State Machine.
     */
    const std::vector<TransitionToStatesPair>& validTransitionsFromCurrentState();

    /**
     * All valid transitions from the specified state. The transition to state graph
     * is code genrated from the model and cannot change.
     */
{{@foreach(it.machine.states) => key, val}}
    static const std::vector<TransitionToStatesPair>& validTransitionsFrom{{it.generator.capitalize(key)}}State();
{{/foreach}}

    /**
     * All valid transitions from the specified state.
     */
    static inline const std::vector<TransitionToStatesPair>& validTransitionsFrom({{it.generator.class()}}State state) {
        switch (state) {
{{@foreach(it.machine.states) => key, val}}
          case {{it.generator.class()}}State::{{key}}:
            return validTransitionsFrom{{it.generator.capitalize(key)}}State();
{{/foreach}}
          default: {
            std::stringstream ss;
            ss << "invalid SM state " << state;
            throw std::runtime_error(ss.str());
          } break;
        }
    }
};


}  // namespace

