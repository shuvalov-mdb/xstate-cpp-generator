// This machine is part of some Actor libary, which is
// outside of the scope of this repository. This is why it
// has actions like 'spawnPongActor', which are defined
// elsewhere. 

//import { generateCpp } from 'xstate-cpp-generator';
import { generateCpp } from '../../src';

import { Machine, createMachine, assign } from 'xstate';

import * as path from 'path';

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
    destinationPath: "generated/",
    namespace: "mongo",
    pathForIncludes: "example-ping-pong/",
    tsScriptName: path.basename(__filename)
  });
