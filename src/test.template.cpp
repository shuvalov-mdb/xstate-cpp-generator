// This test is automatically generated, do not edit.

#include "{{it.properties.pathForIncludes}}/{{it.generator.outputHeaderShortname}}"

#include <gtest/gtest.h>

namespace {{it.properties.namespace }} {
namespace {

TEST(StaticSMTests, TransitionsInfo) {
{{@foreach(it.machine.states) => key, val}}
    {
        auto transitions = {{it.generator.class()}}::validTransitionsFrom{{it.generator.capitalize(key)}}State();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValid{{it.generator.class()}}Event(transition.first));
        }
    }
{{/foreach}}

}

}  // namespace
}  // namespace {{it.properties.namespace }}
