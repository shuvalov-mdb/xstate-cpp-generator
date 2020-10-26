// This test is automatically generated, do not edit.

#include "example-fetch/fetch_sm.h"

#include <gtest/gtest.h>

namespace mongo {
namespace {

TEST(StaticSMTests, TransitionsInfo) {
    {
        auto transitions = FetchSM::validTransitionsFromIdleState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidFetchSMEvent(transition.first));
        }
    }
    {
        auto transitions = FetchSM::validTransitionsFromLoadingState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidFetchSMEvent(transition.first));
        }
    }
    {
        auto transitions = FetchSM::validTransitionsFromSuccessState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidFetchSMEvent(transition.first));
        }
    }
    {
        auto transitions = FetchSM::validTransitionsFromFailureState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidFetchSMEvent(transition.first));
        }
    }

}

}  // namespace
}  // namespace mongo