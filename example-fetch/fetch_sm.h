// This is an automatically generated header, do not edit.

#include <vector>
#include <tuple>
#include <functional>

namespace undefined {

// All states declared in the SM.
enum class FetchSMStates {
        idle,
        loading,
        success,
        failure,
    };

// All transitions declared in the SM.
enum class FetchSMTransitions {
        FETCH,
        RESOLVE,
        REJECT,
        RETRY,
    };

class FetchSM {
  public:
    using TransitionToStatePair = std::tuple<>;

    FetchSM();

    virtual ~FetchSM();

        static const std::vector;
        static const std::vector;
        static const std::vector;
        static const std::vector;
    

};


}  // namespace

