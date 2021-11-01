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

        // As SM is asynchronous, the state may lag the expected.
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            currentState = machine.currentState();
            if (currentState.lastEvent == event) {
                break;
            }
            std::clog << "Waiting for transition " << event << std::endl;
        }
    }
    std::clog << "Made " << count << " transitions" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
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

    /**
     * This block is for transition actions.
     */

    /**
     * This block is for entry and exit state actions.
     */

};

// And finally the more feature rich State Machine can be subclassed from the generated class
// FetchSM, which gives the possibility to overload the virtual methods.
class MyTestStateMachine : public FetchSM<MySpec> {
  public:
    ~MyTestStateMachine() final {}

    // Overload the logging method to use the log system of your project.
    void logTransition(TransitionPhase phase, State currentState, State nextState) const final {
        std::clog << "MyTestStateMachine the phase " << phase;
        switch (phase) {
        case TransitionPhase::LEAVING_STATE:
            std::clog << currentState << ", transitioning to " << nextState;
            break;
        case TransitionPhase::ENTERING_STATE:
            std::clog << nextState << " from " << currentState;
            break;
        case TransitionPhase::ENTERED_STATE:
            std::clog << currentState;
            break;
        default:
            assert(false && "This is impossible");
            break;
        }
        std::clog << std::endl;
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

    void postEvent(FetchSMEvent event) {
        switch (event) {
        case FetchSMEvent::FETCH: {
            std::shared_ptr<FetchSM<MySpec>::FetchPayload> payload =
                std::make_shared<FetchSM<MySpec>::FetchPayload>();
            _sm->postEventFetch (payload);
        } break;
        case FetchSMEvent::RESOLVE: {
            std::shared_ptr<FetchSM<MySpec>::ResolvePayload> payload =
                std::make_shared<FetchSM<MySpec>::ResolvePayload>();
            _sm->postEventResolve (payload);
        } break;
        case FetchSMEvent::REJECT: {
            std::shared_ptr<FetchSM<MySpec>::RejectPayload> payload =
                std::make_shared<FetchSM<MySpec>::RejectPayload>();
            _sm->postEventReject (payload);
        } break;
        case FetchSMEvent::RETRY: {
            std::shared_ptr<FetchSM<MySpec>::RetryPayload> payload =
                std::make_shared<FetchSM<MySpec>::RetryPayload>();
            _sm->postEventRetry (payload);
        } break;
        }
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
            std::clog << "No transitions from state " << currentState.currentState << std::endl;
            break;
        }
        // Make a random transition.
        const FetchSMTransitionToStatesPair& transition = validTransitions[std::rand() % validTransitions.size()];
        const FetchSMEvent event = transition.first;
        std::clog << "Post event " << event << std::endl;
        postEvent(event);

        // As SM is asynchronous, the state may lag the expected.
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            currentState = _sm->currentState();
            if (currentState.lastEvent == event && currentState.totalTransitions == count + 1) {
                break;
            }
            std::clog << "Waiting for transition " << event << std::endl;
        }
    }
    std::clog << "Made " << count << " transitions" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

}  // namespace
}  // namespace mongo