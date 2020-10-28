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

/**
 * This generated unit test demostrates the simplest usage of State Machine without
 * subclassing.
 */
TEST(StaticSMTests, States) {
    FetchSM<> machine;
    int count = 0;
    for (; count < 10; ++count) {
        auto currentState = machine.currentState();
        ASSERT_EQ(currentState.totalTransitions, count);
        auto validTransitions = machine.validTransitionsFromCurrentState();
        if (validTransitions.empty()) {
            break;
        }
        // Make a random transition.
        const FetchSMTransitionToStatesPair& transition = validTransitions[std::rand() % validTransitions.size()];
        const FetchSMEvent event = transition.first;
        switch (event) {
        case FetchSMEvent::FETCH: {
            FetchSM<>::FetchPayload payload;
            machine.postEventFetch (std::move(payload));
        } break;
        case FetchSMEvent::RESOLVE: {
            FetchSM<>::ResolvePayload payload;
            machine.postEventResolve (std::move(payload));
        } break;
        case FetchSMEvent::REJECT: {
            FetchSM<>::RejectPayload payload;
            machine.postEventReject (std::move(payload));
        } break;
        case FetchSMEvent::RETRY: {
            FetchSM<>::RetryPayload payload;
            machine.postEventRetry (std::move(payload));
        } break;
        default:
            ASSERT_TRUE(false) << "This should never happen";
        }

        currentState = machine.currentState();
        ASSERT_EQ(currentState.lastEvent, event);
    }
    std::cout << "Made " << count << " transitions" << std::endl;
}

// User context is some arbitrary payload attached to the State Machine. If none is supplied,
// some dummy empty context still exists.
struct UserContext {
    std::string hello = "This is my context";
    int data = 1;
    // We will count how many times the payload ID of 1 was observed.
    int countOfIdOneSeen = 0;
    std::optional<std::string> dataToKeepWhileInState;
};

// Every Event can have some arbitrary user defined payload. It can be
// any type, as class or some STL type like std::unique_ptr or std::vector.

// Sample payload for the Fetch event.
// The only restriction - it cannot be named EventFetchPayload
// because this name is reserved for the Spec structure.
struct MyFetchPayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 0;
    static constexpr char staticText[] = "it's Fetch payload";
};
// Sample payload for the Resolve event.
// The only restriction - it cannot be named EventResolvePayload
// because this name is reserved for the Spec structure.
struct MyResolvePayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 1;
    static constexpr char staticText[] = "it's Resolve payload";
};
// Sample payload for the Reject event.
// The only restriction - it cannot be named EventRejectPayload
// because this name is reserved for the Spec structure.
struct MyRejectPayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 2;
    static constexpr char staticText[] = "it's Reject payload";
};
// Sample payload for the Retry event.
// The only restriction - it cannot be named EventRetryPayload
// because this name is reserved for the Spec structure.
struct MyRetryPayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 3;
    static constexpr char staticText[] = "it's Retry payload";
};

// Spec struct contains just a bunch of 'using' declarations to stich all types together
// and avoid variable template argument for the SM class declaration.
struct MySpec {
    // Spec should always contain some 'using' for the StateMachineContext.
    using StateMachineContext = UserContext;

    // Then it should have a list of 'using' declarations for every event payload.
    // The name EventFetchPayload is reserved by convention for every event.
    using EventFetchPayload = MyFetchPayload;
    // The name EventResolvePayload is reserved by convention for every event.
    using EventResolvePayload = MyResolvePayload;
    // The name EventRejectPayload is reserved by convention for every event.
    using EventRejectPayload = MyRejectPayload;
    // The name EventRetryPayload is reserved by convention for every event.
    using EventRetryPayload = MyRetryPayload;
};

// And finally the more feature rich State Machine can be subclassed from the generated class
// FetchSM, which gives the possibility to overload the virtual methods.
class MyTestStateMachine : public FetchSM<MySpec> {
  public:
    ~MyTestStateMachine() final;

    // Overload the logging method to use the log system of your project.
    void logTransition(TransitionPhase phase, State currentState, State nextState) const final {
        std::cout << "MyTestStateMachine the phase " << phase;
        switch (phase) {
        case TransitionPhase::LEAVING_STATE:
            std::cout << currentState << ", transitioning to " << nextState;
            break;
        case TransitionPhase::ENTERING_STATE:
            std::cout << nextState << " from " << currentState;
            break;
        case TransitionPhase::ENTERED_STATE:
            std::cout << currentState;
            break;
        default:
            assert(false && "This is impossible");
            break;
        }
        std::cout << std::endl;
    }

    // Overload 'onLeaving' method to cleanup some state or do some other action.
    void onLeavingIdleState(State nextState) final {
        logTransition(FetchSMTransitionPhase::LEAVING_STATE, State::idle, nextState);
        accessContextLocked([this] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState.reset();  // As example we erase some data in the context.
        });
    }
    void onLeavingLoadingState(State nextState) final {
        logTransition(FetchSMTransitionPhase::LEAVING_STATE, State::loading, nextState);
        accessContextLocked([this] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState.reset();  // As example we erase some data in the context.
        });
    }
    void onLeavingSuccessState(State nextState) final {
        logTransition(FetchSMTransitionPhase::LEAVING_STATE, State::success, nextState);
        accessContextLocked([this] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState.reset();  // As example we erase some data in the context.
        });
    }
    void onLeavingFailureState(State nextState) final {
        logTransition(FetchSMTransitionPhase::LEAVING_STATE, State::failure, nextState);
        accessContextLocked([this] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState.reset();  // As example we erase some data in the context.
        });
    }

};

class SMTestFixture : public ::testing::Test {
  public:
    void SetUp() override {
        _sm.reset(new MyTestStateMachine);
    }

    std::unique_ptr<MyTestStateMachine> _sm;
};

TEST_F(SMTestFixture, States) {
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