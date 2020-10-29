// This test is automatically generated, do not edit.

#include "engineer_sm.h"

#include <gtest/gtest.h>

namespace engineer_demo {
namespace {

TEST(StaticSMTests, TransitionsInfo) {
    {
        auto transitions = EngineerSMValidTransitionsFromSleepingState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidEngineerSMEvent(transition.first));
        }
    }
    {
        auto transitions = EngineerSMValidTransitionsFromWorkingState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidEngineerSMEvent(transition.first));
        }
    }
    {
        auto transitions = EngineerSMValidTransitionsFromEatingState();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValidEngineerSMEvent(transition.first));
        }
    }
}

/**
 * This generated unit test demostrates the simplest usage of State Machine without
 * subclassing.
 */
TEST(StaticSMTests, States) {
    EngineerSM<> machine;
    int count = 0;
    for (; count < 10; ++count) {
        auto currentState = machine.currentState();
        ASSERT_EQ(currentState.totalTransitions, count);
        auto validTransitions = machine.validTransitionsFromCurrentState();
        if (validTransitions.empty()) {
            break;
        }
        // Make a random transition.
        const EngineerSMTransitionToStatesPair& transition = validTransitions[std::rand() % validTransitions.size()];
        const EngineerSMEvent event = transition.first;
        switch (event) {
        case EngineerSMEvent::TIMER: {
            EngineerSM<>::TimerPayload payload;
            machine.postEventTimer (std::move(payload));
        } break;
        case EngineerSMEvent::TIRED: {
            EngineerSM<>::TiredPayload payload;
            machine.postEventTired (std::move(payload));
        } break;
        case EngineerSMEvent::HUNGRY: {
            EngineerSM<>::HungryPayload payload;
            machine.postEventHungry (std::move(payload));
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

// Sample payload for the Timer event.
// The only restriction - it cannot be named EventTimerPayload
// because this name is reserved for the Spec structure.
struct MyTimerPayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 0;
    static constexpr char staticText[] = "it's Timer payload";
};
// Sample payload for the Tired event.
// The only restriction - it cannot be named EventTiredPayload
// because this name is reserved for the Spec structure.
struct MyTiredPayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 1;
    static constexpr char staticText[] = "it's Tired payload";
};
// Sample payload for the Hungry event.
// The only restriction - it cannot be named EventHungryPayload
// because this name is reserved for the Spec structure.
struct MyHungryPayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 2;
    static constexpr char staticText[] = "it's Hungry payload";
};

// Spec struct contains just a bunch of 'using' declarations to stich all types together
// and avoid variable template argument for the SM class declaration.
struct MySpec {
    // Spec should always contain some 'using' for the StateMachineContext.
    using StateMachineContext = UserContext;

    // Then it should have a list of 'using' declarations for every event payload.
    // The name EventTimerPayload is reserved by convention for every event.
    using EventTimerPayload = MyTimerPayload;
    // The name EventTiredPayload is reserved by convention for every event.
    using EventTiredPayload = MyTiredPayload;
    // The name EventHungryPayload is reserved by convention for every event.
    using EventHungryPayload = MyHungryPayload;

    /**
     * This block is for transition actions.
     */
    static void startHungryTimer (EngineerSM<MySpec>* sm, EventTimerPayload* payload) {
        std::cout << payload->str << " " << payload->staticText << " inside startHungryTimer" << std::endl;
    }
    static void startTiredTimer (EngineerSM<MySpec>* sm, EventTimerPayload* payload) {
        std::cout << payload->str << " " << payload->staticText << " inside startTiredTimer" << std::endl;
    }
    static void checkEmail (EngineerSM<MySpec>* sm, EventHungryPayload* payload) {
        std::cout << payload->str << " " << payload->staticText << " inside checkEmail" << std::endl;
    }

    /**
     * This block is for entry and exit state actions.
     */
    static void startWakeupTimer (EngineerSM<MySpec>* sm) {
        std::cout << "Do startWakeupTimer" << std::endl;
    }
    static void checkEmail (EngineerSM<MySpec>* sm) {
        std::cout << "Do checkEmail" << std::endl;
    }
    static void startHungryTimer (EngineerSM<MySpec>* sm) {
        std::cout << "Do startHungryTimer" << std::endl;
    }
    static void startShortTimer (EngineerSM<MySpec>* sm) {
        std::cout << "Do startShortTimer" << std::endl;
    }
    static void morningRoutine (EngineerSM<MySpec>* sm) {
        std::cout << "Do morningRoutine" << std::endl;
    }
    static void startTiredTimer (EngineerSM<MySpec>* sm) {
        std::cout << "Do startTiredTimer" << std::endl;
    }

};

// And finally the more feature rich State Machine can be subclassed from the generated class
// EngineerSM, which gives the possibility to overload the virtual methods.
class MyTestStateMachine : public EngineerSM<MySpec> {
  public:
    ~MyTestStateMachine() final {}

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
    void onLeavingSleepingState(State nextState) final {
        logTransition(EngineerSMTransitionPhase::LEAVING_STATE, State::sleeping, nextState);
        accessContextLocked([this] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState.reset();  // As example we erase some data in the context.
        });
    }
    void onLeavingWorkingState(State nextState) final {
        logTransition(EngineerSMTransitionPhase::LEAVING_STATE, State::working, nextState);
        accessContextLocked([this] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState.reset();  // As example we erase some data in the context.
        });
    }
    void onLeavingEatingState(State nextState) final {
        logTransition(EngineerSMTransitionPhase::LEAVING_STATE, State::eating, nextState);
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

    void postEvent(EngineerSMEvent event) {
        switch (event) {
        case EngineerSMEvent::TIMER: {
            EngineerSM<MySpec>::TimerPayload payload;
            _sm->postEventTimer (std::move(payload));
        } break;
        case EngineerSMEvent::TIRED: {
            EngineerSM<MySpec>::TiredPayload payload;
            _sm->postEventTired (std::move(payload));
        } break;
        case EngineerSMEvent::HUNGRY: {
            EngineerSM<MySpec>::HungryPayload payload;
            _sm->postEventHungry (std::move(payload));
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
            break;
        }
        // Make a random transition.
        const EngineerSMTransitionToStatesPair& transition = validTransitions[std::rand() % validTransitions.size()];
        const EngineerSMEvent event = transition.first;
        postEvent(event);

        currentState = _sm->currentState();
        ASSERT_EQ(currentState.lastEvent, event);
    }
    std::cout << "Made " << count << " transitions" << std::endl;
}

}  // namespace
}  // namespace engineer_demo