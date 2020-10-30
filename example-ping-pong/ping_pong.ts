// This machine is part of some Actor libary, which is
// outside of the scope of this repository. This is why it
// has actions like 'spawnPongActor', which are defined
// elsewhere. 

import { generateCpp }  from '../src/cpp_state_machine_generator';
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


generateCpp({
    xstateMachine: pingPongMachine,
    destinationPath: "",
    namespace: "mongo",
    pathForIncludes: "example-ping-pong/",
    tsScriptName: path.basename(__filename)
  });
