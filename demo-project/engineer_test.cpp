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
    {
        auto transitions = EngineerSMValidTransitionsFromWeekendState();
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
        case EngineerSMEvent::HUNGRY: {
            EngineerSM<>::HungryPayload payload;
            machine.postEventHungry (std::move(payload));
        } break;
        case EngineerSMEvent::TIRED: {
            EngineerSM<>::TiredPayload payload;
            machine.postEventTired (std::move(payload));
        } break;
        case EngineerSMEvent::ENOUGH: {
            EngineerSM<>::EnoughPayload payload;
            machine.postEventEnough (std::move(payload));
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

// Sample payload for the Timer event.
// The only restriction - it cannot be named EventTimerPayload
// because this name is reserved for the Spec structure.
struct MyTimerPayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 0;
    static constexpr char staticText[] = "it's Timer payload";
};
// Sample payload for the Hungry event.
// The only restriction - it cannot be named EventHungryPayload
// because this name is reserved for the Spec structure.
struct MyHungryPayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 1;
    static constexpr char staticText[] = "it's Hungry payload";
};
// Sample payload for the Tired event.
// The only restriction - it cannot be named EventTiredPayload
// because this name is reserved for the Spec structure.
struct MyTiredPayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 2;
    static constexpr char staticText[] = "it's Tired payload";
};
// Sample payload for the Enough event.
// The only restriction - it cannot be named EventEnoughPayload
// because this name is reserved for the Spec structure.
struct MyEnoughPayload {
    int data = 42;
    std::string str = "Hi";
    int someID = 3;
    static constexpr char staticText[] = "it's Enough payload";
};

// Spec struct contains just a bunch of 'using' declarations to stich all types together
// and avoid variable template argument for the SM class declaration.
struct MySpec {
    // Spec should always contain some 'using' for the StateMachineContext.
    using StateMachineContext = UserContext;

    // Then it should have a list of 'using' declarations for every event payload.
    // The name EventTimerPayload is reserved by convention for every event.
    using EventTimerPayload = MyTimerPayload;
    // The name EventHungryPayload is reserved by convention for every event.
    using EventHungryPayload = MyHungryPayload;
    // The name EventTiredPayload is reserved by convention for every event.
    using EventTiredPayload = MyTiredPayload;
    // The name EventEnoughPayload is reserved by convention for every event.
    using EventEnoughPayload = MyEnoughPayload;

    /**
     * This block is for transition actions.
     */
    static void startHungryTimer (EngineerSM<MySpec>* sm, std::shared_ptr<EventTimerPayload> payload) {
        std::clog << payload->str << " " << payload->staticText << " inside startHungryTimer" << std::endl;
        sm->accessContextLocked([payload] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState = std::string(payload->staticText);
        });
    }
    static void startTiredTimer (EngineerSM<MySpec>* sm, std::shared_ptr<EventTimerPayload> payload) {
        std::clog << payload->str << " " << payload->staticText << " inside startTiredTimer" << std::endl;
        sm->accessContextLocked([payload] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState = std::string(payload->staticText);
        });
    }
    static void checkEmail (EngineerSM<MySpec>* sm, std::shared_ptr<EventHungryPayload> payload) {
        std::clog << payload->str << " " << payload->staticText << " inside checkEmail" << std::endl;
        sm->accessContextLocked([payload] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState = std::string(payload->staticText);
        });
    }

    /**
     * This block is for entry and exit state actions.
     */
    static void startWakeupTimer (EngineerSM<MySpec>* sm) {
        std::clog << "Do startWakeupTimer" << std::endl;
    }
    static void checkEmail (EngineerSM<MySpec>* sm) {
        std::clog << "Do checkEmail" << std::endl;
    }
    static void startHungryTimer (EngineerSM<MySpec>* sm) {
        std::clog << "Do startHungryTimer" << std::endl;
    }
    static void checkIfItsWeekend (EngineerSM<MySpec>* sm) {
        std::clog << "Do checkIfItsWeekend" << std::endl;
    }
    static void startShortTimer (EngineerSM<MySpec>* sm) {
        std::clog << "Do startShortTimer" << std::endl;
    }
    static void morningRoutine (EngineerSM<MySpec>* sm) {
        std::clog << "Do morningRoutine" << std::endl;
    }

};

// And finally the more feature rich State Machine can be subclassed from the generated class
// EngineerSM, which gives the possibility to overload the virtual methods.
class MyTestStateMachine : public EngineerSM<MySpec> {
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
    void onLeavingWeekendState(State nextState) final {
        logTransition(EngineerSMTransitionPhase::LEAVING_STATE, State::weekend, nextState);
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
            std::shared_ptr<EngineerSM<MySpec>::TimerPayload> payload =
                std::make_shared<EngineerSM<MySpec>::TimerPayload>();
            _sm->postEventTimer (payload);
        } break;
        case EngineerSMEvent::HUNGRY: {
            std::shared_ptr<EngineerSM<MySpec>::HungryPayload> payload =
                std::make_shared<EngineerSM<MySpec>::HungryPayload>();
            _sm->postEventHungry (payload);
        } break;
        case EngineerSMEvent::TIRED: {
            std::shared_ptr<EngineerSM<MySpec>::TiredPayload> payload =
                std::make_shared<EngineerSM<MySpec>::TiredPayload>();
            _sm->postEventTired (payload);
        } break;
        case EngineerSMEvent::ENOUGH: {
            std::shared_ptr<EngineerSM<MySpec>::EnoughPayload> payload =
                std::make_shared<EngineerSM<MySpec>::EnoughPayload>();
            _sm->postEventEnough (payload);
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
        const EngineerSMTransitionToStatesPair& transition = validTransitions[std::rand() % validTransitions.size()];
        const EngineerSMEvent event = transition.first;
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
}  // namespace engineer_demo