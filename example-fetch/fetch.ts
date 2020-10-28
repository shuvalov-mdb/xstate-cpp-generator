import { generateCpp }  from '../src/cpp_state_machine_generator';
const path = require('path');

import { Machine, createMachine, assign } from 'xstate';

const fetchMachine = Machine({
    id: 'fetch',
    initial: 'idle',
    context: {
      retries: 0
    },
    states: {
      idle: {
        on: {
          FETCH: 'loading'
        }
      },
      loading: {
        on: {
          RESOLVE: 'success',
          REJECT: 'failure'
        }
      },
      success: {
        type: 'final'
      },
      failure: {
        on: {
          RETRY: 'loading'
        }
      }
    }
  });

generateCpp({
  xstateMachine: fetchMachine,
  destinationPath: "",
  namespace: "mongo",
  pathForIncludes: "example-fetch",
  tsScriptName: path.basename(__filename)
});
