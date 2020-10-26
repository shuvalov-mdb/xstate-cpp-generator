// This is an automatically generated header, do not edit.

#include <vector>
#include <tuple>
#include <functional>

namespace {{it.preperties}} {

// All states declared in the SM.
enum class {{it.generator.Class()}}States {
    {{@foreach(it.machine.states) => key, val}}
    {{key}},
    {{/foreach}}
};

// All transitions declared in the SM.
enum class {{it.generator.Class()}}Transitions {
    {{@each(it.generator.transitions()) => val, index}}
    {{val}},
    {{/each}}
};

class {{it.generator.Class()}} {
  public:
    using TransitionToStatePair = std::tuple<>;

    {{it.generator.Class()}}();

    virtual ~{{it.generator.Class()}}();

    {{@foreach(it.machine.states) => key, val}}
    static const std::vector;
    {{/foreach}}


};


}  // namespace

