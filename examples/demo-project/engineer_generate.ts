//import { generateCpp } from 'xstate-cpp-generator';
import { generateCpp } from '../../src';
import { Machine } from 'xstate';

import * as path from 'path';

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


//CppGen.
generateCpp({
    xstateMachine: engineerMachine,
    destinationPath: "generated/",
    namespace: "engineer_demo",
    pathForIncludes: "",
    tsScriptName: path.basename(__filename)
  });
