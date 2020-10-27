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
        ASSERT_EQ(currentState.totalTransitions, count);
        auto validTransitions = _sm->validTransitionsFromCurrentState();
        if (validTransitions.empty()) {
            break;
        }
        // Make a random transition.
        const FetchSMTransitionToStatesPair& transition = validTransitions[std::rand() % validTransitions.size()];
        const FetchSMEvent event = transition.first;
        switch (event) {
        case FetchSMEvent::FETCH: {
            FetchSM<>::FetchPayload payload;
            _sm->postEventFetch (std::move(payload));
        } break;
        case FetchSMEvent::RESOLVE: {
            FetchSM<>::ResolvePayload payload;
            _sm->postEventResolve (std::move(payload));
        } break;
        case FetchSMEvent::REJECT: {
            FetchSM<>::RejectPayload payload;
            _sm->postEventReject (std::move(payload));
        } break;
        case FetchSMEvent::RETRY: {
            FetchSM<>::RetryPayload payload;
            _sm->postEventRetry (std::move(payload));
        } break;
        default:
            ASSERT_TRUE(false) << "This should never happen";
        }

        currentState = _sm->currentState();
        ASSERT_EQ(currentState.lastEvent, event);
    }
    std::cout << "Made " << count << " transitions" << std::endl;
}

}  // namespace
}  // namespace mongo