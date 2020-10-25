// This is an automatically generated header, do not edit.

namespace {{it.project.namespace}} {

enum class States {
    {{@foreach(it.machine.states) => key, val}}
    {{key}},
    {{/foreach}}
};


class {{it.generator.Class()}} {
  public:
    {{it.generator.Class()}}();
};


}  // namespace

