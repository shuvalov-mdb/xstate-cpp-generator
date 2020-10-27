// This test is automatically generated, do not edit.

#include "example-fetch/fetch_sm.h"

#include <gtest/gtest.h>

namespace mongo {
namespace {

TEST(StaticSMTests, TransitionsInfo) {
    {
        auto transitions = FetchSMValidTransitionsFromIdleState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidFetchSMEvent(transition.first));
        }
    }
    {
        auto transitions = FetchSMValidTransitionsFromLoadingState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidFetchSMEvent(transition.first));
        }
    }
    {
        auto transitions = FetchSMValidTransitionsFromSuccessState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidFetchSMEvent(transition.first));
        }
    }
    {
        auto transitions = FetchSMValidTransitionsFromFailureState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidFetchSMEvent(transition.first));
        }
    }
}

class SMTestFixture : public ::testing::Test {
  public:
    void SetUp() override {
        _sm.reset(new FetchSM<>());
    }

    std::unique_ptr<FetchSM<>> _sm;
};

TEST_F(SMTestFixture, State) {
    int count = 0;
    for (; count < 10; ++count) {
        auto currentState = _sm->currentState();
        auto validTransitions = _sm->validTransitionsFromCurrentState();
        if (validTransitions.empty()) {
            break;
        }
        const FetchSMTransitionToStatesPair& transition = validTransitions[std::rand() % validTransitions.size()];
    }
}

}  // namespace
}  // namespace mongo