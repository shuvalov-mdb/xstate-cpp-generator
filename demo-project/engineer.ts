//const CppGen = require('xstate-cpp-generator');
import { generateCpp }  from '../src/cpp_state_machine_generator';
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
                'TIRED': { target: 'sleeping' }
            }
        },
        working: {
            entry: ['checkEmail', 'startHungryTimer' ],
            on: {
                'HUNGRY': { target: 'eating', actions: ['checkEmail']},
                'TIRED': { target: 'sleeping' }
            },
        },
        eating: {
            entry: 'startShortTimer',
            exit: [ 'checkEmail', 'startHungryTimer', 'startTiredTimer' ],
            on: {
                'TIMER': { target: 'working', actions: ['startHungryTimer'] },
                'TIRED': { target: 'sleeping' }
            }
        }
    }
});


//CppGen.
generateCpp({
    xstateMachine: engineerMachine,
    destinationPath: "",
    namespace: "engineer_demo",
    pathForIncludes: "",
    tsScriptName: path.basename(__filename)
  });
