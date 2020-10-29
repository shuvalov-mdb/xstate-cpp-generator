#include "engineer_sm.h"

#include <chrono>
#include <thread>

namespace engineer_demo {

template<typename Function>
void startTimer(Function function, int delayMs) {
    std::thread t([=]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        function();
    });
    t.detach();
}

struct EngineerContext {
    // The demo will end after the Engineer wakes up 7 times.
    int wakeUpCount = 0;
};

struct EngineerSpec {
    // Spec should always contain some 'using' for the StateMachineContext.
    using StateMachineContext = EngineerContext;

    // Then it should have a list of 'using' declarations for every event payload.
    using EventTimerPayload = std::nullptr_t;
    using EventHungryPayload = std::nullptr_t;
    using EventTiredPayload = std::nullptr_t;

    /**
     * This block is for transition actions.
     */
    static void startHungryTimer (EngineerSM<EngineerSpec>* sm, EventTimerPayload* payload) {
        std::clog << "Start HungryTimer from timer event" << std::endl;
        startTimer([sm] {
            std::clog << "Ok, I'm hungry" << std::endl;
            sm->postEventHungry(std::nullptr_t());
        }, 100);
    }
    static void startTiredTimer (EngineerSM<EngineerSpec>* sm, EventTimerPayload* payload) {
        std::clog << "Start TiredTimer from timer event" << std::endl;
        startTimer([sm] {
            std::clog << "Ok, I'm tired" << std::endl;
            sm->postEventTired(std::nullptr_t());
        }, 1000);
    }
    static void checkEmail (EngineerSM<EngineerSpec>* sm, EventHungryPayload* payload) {
        std::clog << "Checking Email, while being hugry! ok..." << std::endl;
    }

    /**
     * This block is for entry and exit state actions.
     */
    static void startWakeupTimer (EngineerSM<EngineerSpec>* sm) {
        std::clog << "Do startWakeupTimer" << std::endl;
        startTimer([sm] {
            std::clog << "Hey wake up" << std::endl;
            sm->postEventTimer(std::nullptr_t());
        }, 1000);
    }
    static void checkEmail (EngineerSM<EngineerSpec>* sm) {
        std::clog << "Checking Email, hmmm..." << std::endl;
    }
    static void startHungryTimer (EngineerSM<EngineerSpec>* sm) {
        std::clog << "Start HungryTimer" << std::endl;
        startTimer([sm] {
            std::clog << "Ok, I'm hungry" << std::endl;
            sm->postEventHungry(std::nullptr_t());
        }, 100);
    }

    static void startShortTimer (EngineerSM<EngineerSpec>* sm) {
        std::clog << "Start short Timer" << std::endl;
        startTimer([sm] {
            std::clog << "Hey, timer is ringing." << std::endl;
            sm->postEventTimer(std::nullptr_t());
        }, 10);
    }

    static void morningRoutine (EngineerSM<EngineerSpec>* sm) {
        sm->accessContextLocked([] (StateMachineContext& userContext) {
            ++userContext.wakeUpCount;
            std::clog << "This is my " << userContext.wakeUpCount << " day of working..." << std::endl;
        });
    }

    static void startTiredTimer (EngineerSM<EngineerSpec>* sm) {
        std::clog << "Start TiredTimer" << std::endl;
        startTimer([sm] {
            std::clog << "Ok, I'm tired" << std::endl;
            sm->postEventTired(std::nullptr_t());
        }, 1000);
    }
};

}  // namespace

int main(int argc, char** argv) {
    engineer_demo::EngineerSM<engineer_demo::EngineerSpec> stateMachine;
    // Kick off the state machine with a timer event...
    stateMachine.postEventTimer(std::nullptr_t());

    int wakeUpCount = 0;  // We end the week after waking up 7 times.
    while (wakeUpCount < 7) {
        stateMachine.accessContextLocked([&wakeUpCount] (engineer_demo::EngineerContext& userContext) {
            wakeUpCount = userContext.wakeUpCount;
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        stateMachine.postEventTimer(std::nullptr_t());
    }
    return 0;
}
