# C++ State Machine generator for Xstate

This package allows to convert TypeScript language State Machine developed
using [Xstate](https://github.com/davidkpiano/xstate) into C++ generated SM, no coding required.

Project location: https://github.com/shuvalov-mdb/xstate-cpp-generator
Copyright Andrew Shuvalov, MIT [License](https://github.com/shuvalov-mdb/xstate-cpp-generator/blob/master/LICENSE)

## Features

* Design and test the State Machine in [Xstate](https://github.com/davidkpiano/xstate) and then convert to C++ without any changes
  * Use the [online vizualizer](https://xstate.js.org/viz/) to debug the State Machine
* SM basics: [States](https://xstate.js.org/docs/guides/states.html), [Events](https://xstate.js.org/docs/guides/events.html), [Transitions](https://xstate.js.org/docs/guides/transitions.html)
* Generated C++ is fully synchronized, safe to use in multi-threaded environemnt without any changes
* No exteral dependencies except STL. No boost dependency.
* Callback model based on subclassing and virtual methods
  * Callbacks are invoked when: leaving a state, entering a state, after entering a state
* Arbitrary user-defined data structure (called Context) can be stored in the SM
* Any event can have an arbitrary user-defined payload attached. The event payload is propagated to related callbacks

## Install and Quick Start Tutorial

Install the xstate-cpp-generator TypeScript package, locally (or globally with `-g` option):

```bash
   npm install xstate-cpp-generator
```
Create a simple Xstate model file `ping.ts` with few lines to trigger C++ generation at the end:

```TypeScript
const CppGen = require('xstate-cpp-generator');
const path = require('path');

import { Machine, createMachine, assign } from 'xstate';

const pingPongMachine = Machine({
    id: 'ping',
    initial: 'init',
    states: {
        init: {
            on: {
                'START': { target: 'pinging', actions: ['savePongActorAddress', 'spawnPongActor'] }
            }
        },
        pinging: {
            onEntry: 'sendPingToPongActor',
            on: {
                'PONG': { target: 'pinging', actions: ['sendPingToPongActor']}
            }
        }
    }
});


CppGen.generateCpp({
    xstateMachine: pingPongMachine,
    destinationPath: "",
    namespace: "mongo",
    pathForIncludes: "example-ping-pong",
    tsScriptName: path.basename(__filename)
  });
```

And generate C++ with:

```bash
   ts-node ping.ts
```
You should see new generated files:
```
ping_sm.cpp        ping_sm.h          ping_test.cpp
```
