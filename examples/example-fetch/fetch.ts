//import { generateCpp } from 'xstate-cpp-generator';
import { generateCpp } from '../../src';

import { Machine, createMachine, assign } from 'xstate';

import * as path from 'path';

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
  destinationPath: "generated/",
  namespace: "mongo",
  pathForIncludes: "example-fetch/",
  tsScriptName: path.basename(__filename)
});
