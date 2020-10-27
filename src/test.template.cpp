// This test is automatically generated, do not edit.

#include "{{it.properties.pathForIncludes}}/{{it.generator.outputHeaderShortname}}"

#include <gtest/gtest.h>

namespace {{it.properties.namespace }} {
namespace {

TEST(StaticSMTests, TransitionsInfo) {
{{@foreach(it.machine.states) => key, val}}
    {
        auto transitions = {{it.generator.class()}}ValidTransitionsFrom{{it.generator.capitalize(key)}}State();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValid{{it.generator.class()}}Event(transition.first));
        }
    }
{{/foreach}}
}

class SMTestFixture : public ::testing::Test {
  public:
    void SetUp() override {
        _sm.reset(new {{it.generator.class()}}<>());
    }

    std::unique_ptr<{{it.generator.class()}}<>> _sm;
};

TEST_F(SMTestFixture, State) {
    int count = 0;
    for (; count < 10; ++count) {
        auto currentState = _sm->currentState();
        ASSERT_EQ(currentState.totalTransitions, count);
        auto validTransitions = _sm->validTransitionsFromCurrentState();
        if (validTransitions.empty()) {
            break;
        }
        // Make a random transition.
        const {{it.generator.class()}}TransitionToStatesPair& transition = validTransitions[std::rand() % validTransitions.size()];
        const {{it.generator.class()}}Event event = transition.first;
        switch (event) {
{{@each(it.generator.events()) => val, index}}
        case {{it.generator.class()}}Event::{{val}}: {
            {{it.generator.class()}}<>::{{it.generator.capitalize(val)}}Payload payload;
            _sm->postEvent{{it.generator.capitalize(val)}} (std::move(payload));
        } break;
{{/each}}
        default:
            ASSERT_TRUE(false) << "This should never happen";
        }

        currentState = _sm->currentState();
        ASSERT_EQ(currentState.lastEvent, event);
    }
    std::cout << "Made " << count << " transitions" << std::endl;
}

}  // namespace
}  // namespace {{it.properties.namespace }}
