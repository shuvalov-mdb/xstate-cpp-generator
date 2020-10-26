import { StateNode } from 'xstate';
import { Generator }  from './generator';

export interface CppStateMachineGeneratorProperties {
    /** Xsate Machine defined using Xstate API */
    xstateMachine: StateNode;
    /** Destination path for generated C++ files. */
    destinationPath: string;
    /** Namespace for the generated C++ files */
    namespace: string;
    /** When the generated .cpp files need to include the generated headers, this path will be used. */
    pathForIncludes: string;
}

export function generateCpp(properties: CppStateMachineGeneratorProperties) {
    let generator = new Generator(properties);
    generator.generate();
}

