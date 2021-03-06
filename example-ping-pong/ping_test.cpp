// This test is automatically generated, do not edit.

#include "example-ping-pong/ping_sm.h"

#include <gtest/gtest.h>

namespace mongo {
namespace {

TEST(StaticSMTests, TransitionsInfo) {
    {
        auto transitions = PingSMValidTransitionsFromInitState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidPingSMEvent(transition.first));
        }
    }
    {
        auto transitions = PingSMValidTransitionsFromPingingState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidPingSMEvent(transition.first));
        }
    }
}

/**
 * This generated unit test demostrates the simplest usage of State Machine without
 * subclassing.
 */
TEST(StaticSMTests, States) {
    PingSM<> machine;
    int count = 0;
    for (; count < 10; ++count) {
        auto currentState = machine.currentState();
        ASSERT_EQ(currentState.totalTransitions, count);
        auto validTransitions = machine.validTransitionsFromCurrentState();
        if (validTransitions.empty()) {
            break;
        }
        // Make a random transition.
        const PingSMTransitionToStatesPair& transition = validTransitions[std::rand() % validTransitions.size()];
        const PingSMEvent event = transition.first;
        switch (event) {
        case PingSMEvent::START: {
            PingSM<>::StartPayload payload;
            machine.postEventStart (std::move(payload));
        } break;
        case PingSMEvent::PONG: {
            PingSM<>::PongPayload payload;
            machine.postEventPong (std::move(payload));
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

// Sample payload for the Start event.
// The only restriction - it cannot be named EventStartPayload
// because this name is reserved for the Spec structure.
struct MyStartPayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 0;
    static constexpr char staticText[] = "it's Start payload";
};
// Sample payload for the Pong event.
// The only restriction - it cannot be named EventPongPayload
// because this name is reserved for the Spec structure.
struct MyPongPayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 1;
    static constexpr char staticText[] = "it's Pong payload";
};

// Spec struct contains just a bunch of 'using' declarations to stich all types together
// and avoid variable template argument for the SM class declaration.
struct MySpec {
    // Spec should always contain some 'using' for the StateMachineContext.
    using StateMachineContext = UserContext;

    // Then it should have a list of 'using' declarations for every event payload.
    // The name EventStartPayload is reserved by convention for every event.
    using EventStartPayload = MyStartPayload;
    // The name EventPongPayload is reserved by convention for every event.
    using EventPongPayload = MyPongPayload;

    /**
     * This block is for transition actions.
     */
    static void savePongActorAddress (PingSM<MySpec>* sm, std::shared_ptr<EventStartPayload> payload) {
        std::clog << payload->str << " " << payload->staticText << " inside savePongActorAddress" << std::endl;
        sm->accessContextLocked([payload] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState = std::string(payload->staticText);
        });
    }
    static void spawnPongActor (PingSM<MySpec>* sm, std::shared_ptr<EventStartPayload> payload) {
        std::clog << payload->str << " " << payload->staticText << " inside spawnPongActor" << std::endl;
        sm->accessContextLocked([payload] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState = std::string(payload->staticText);
        });
    }
    static void sendPingToPongActor (PingSM<MySpec>* sm, std::shared_ptr<EventPongPayload> payload) {
        std::clog << payload->str << " " << payload->staticText << " inside sendPingToPongActor" << std::endl;
        sm->accessContextLocked([payload] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState = std::string(payload->staticText);
        });
    }

    /**
     * This block is for entry and exit state actions.
     */
    static void sendPingToPongActor (PingSM<MySpec>* sm) {
        std::clog << "Do sendPingToPongActor" << std::endl;
    }

};

// And finally the more feature rich State Machine can be subclassed from the generated class
// PingSM, which gives the possibility to overload the virtual methods.
class MyTestStateMachine : public PingSM<MySpec> {
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
    void onLeavingInitState(State nextState) final {
        logTransition(PingSMTransitionPhase::LEAVING_STATE, State::init, nextState);
        accessContextLocked([this] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState.reset();  // As example we erase some data in the context.
        });
    }
    void onLeavingPingingState(State nextState) final {
        logTransition(PingSMTransitionPhase::LEAVING_STATE, State::pinging, nextState);
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

    void postEvent(PingSMEvent event) {
        switch (event) {
        case PingSMEvent::START: {
            std::shared_ptr<PingSM<MySpec>::StartPayload> payload =
                std::make_shared<PingSM<MySpec>::StartPayload>();
            _sm->postEventStart (payload);
        } break;
        case PingSMEvent::PONG: {
            std::shared_ptr<PingSM<MySpec>::PongPayload> payload =
                std::make_shared<PingSM<MySpec>::PongPayload>();
            _sm->postEventPong (payload);
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
        const PingSMTransitionToStatesPair& transition = validTransitions[std::rand() % validTransitions.size()];
        const PingSMEvent event = transition.first;
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