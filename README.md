# C++ State Machine generator for Xstate

This package allows to convert TypeScript language State Machine developed
using [Xstate](https://github.com/davidkpiano/xstate) into C++ generated SM, no coding required.

* Project location: https://github.com/shuvalov-mdb/xstate-cpp-generator
* NPM TypeScript package location: https://www.npmjs.com/package/xstate-cpp-generator
* Copyright Andrew Shuvalov, MIT [License](https://github.com/shuvalov-mdb/xstate-cpp-generator/blob/master/LICENSE)

## Tutorials and Documentation
* Quick start is right below
* [Tutorial](TUTORIAL.md)
* [Video tutorial](https://youtu.be/_cRquls_lOg)

## Features

* Design and test the State Machine in [Xstate](https://github.com/davidkpiano/xstate) and then convert to C++ without any changes
  * Use the [online vizualizer](https://xstate.js.org/viz/) to debug the State Machine
* SM basic features supported: [States](https://xstate.js.org/docs/guides/states.html), [Events](https://xstate.js.org/docs/guides/events.html), [Transitions](https://xstate.js.org/docs/guides/transitions.html)
  * SM extra features supported: [Actions](https://xstate.js.org/docs/guides/actions.html#declarative-actions)
* Generated C++ is fully synchronized, safe to use in multi-threaded environemnt without any changes
* No external dependencies except STL. No boost dependency.
* Callback model:
  * Entry, Exit and Trasition [Actions](https://xstate.js.org/docs/guides/actions.html#declarative-actions) are code
   generated as static methods in the template object used to declare the State Machine and can be implemented by the user
  * Every state and transtion callbacks are generated as virtual methods that can be overloaded by subclassing
* Arbitrary user-defined data structure (called Context) can be stored in the SM
* Any event can have an arbitrary user-defined payload attached. The event payload is propagated to related callbacks

## Install and Quick Start Tutorial

### 1. Install the xstate-cpp-generator TypeScript package, locally (or globally with `-g` option):

```bash
   npm install xstate-cpp-generator
```
### 2. Create a simple Xstate model file `engineer.ts` with few lines to trigger C++ generation at the end:
(this example is located at https://github.com/shuvalov-mdb/xstate-cpp-generator/tree/master/demo-project)

```TypeScript
const CppGen = require('xstate-cpp-generator');
const path = require('path');

import { Machine } from 'xstate';

const engineerMachine = Machine({
    id: 'engineer',
    initial: 'sleeping',
    states: {
        sleeping: {
            entry: 'startWakeupTimer',
            exit: 'morningRoutine',
            on: {
                'TIMER': { target: 'working', actions: ['startHungryTimer', 'startTiredTimer'] },
            }
        },
        working: {
            entry: ['checkEmail', 'startHungryTimer', 'checkIfItsWeekend' ],
            on: {
                'HUNGRY': { target: 'eating', actions: ['checkEmail']},
                'TIRED': { target: 'sleeping' },
                'ENOUGH': { target: 'weekend' }
            },
        },
        eating: {
            entry: 'startShortTimer',
            exit: [ 'checkEmail', 'startHungryTimer' ],
            on: {
                'TIMER': { target: 'working', actions: ['startHungryTimer'] },
                'TIRED': { target: 'sleeping' }
            }
        },
        weekend: {
            type: 'final',
        }
    }
});

CppGen.generateCpp({
    xstateMachine: engineerMachine,
    destinationPath: "",
    namespace: "engineer_demo",
    pathForIncludes: "",
    tsScriptName: path.basename(__filename)
  });

```
To visualize this State Machine copy-paste the 'Machine' method call to the [online vizualizer](https://xstate.js.org/viz/).

### 3. Generate C++
Install all required dependencies:

```bash
   npm install
```

And run the C++ generator:
```bash
   ts-node engineer.ts
```
You should see new generated files:
```
engineer_sm.h  engineer_sm.cpp  engineer_test.cpp
```

The `engineer_test.cpp` is an automatically generated Unit Test for the model. Create a simple `SConscript` file to compile it:

```
env = Environment()

LIBS =''

common_libs = ['gtest_main', 'gtest', 'pthread']
env.Append( LIBS = common_libs )

env.Append(CCFLAGS=['-fsanitize=address,undefined',
                    '-fno-omit-frame-pointer'],
           LINKFLAGS='-fsanitize=address,undefined')

env.Program('engineer_test', ['engineer_sm.cpp', 'engineer_test.cpp'], 
            LIBS, LIBPATH='/opt/gtest/lib:/usr/local/lib', CXXFLAGS="-std=c++17")

```
and run it with:
```
   scons
   ./engineer_test
```


## Release Notes

### V 1.0.3
* Full support of entry, exit and transition Actions
* Multi-threading bugfixes
### V 1.0.4
* Converted `onEnteredState()` from move sematics `&&` to shared_ptr
* Started Tutorial
