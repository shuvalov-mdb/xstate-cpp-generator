{
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
  }
  