//import type { StateNode } from 'xstate';
import { Generator }  from './generator';
import type { CppStateMachineGeneratorProperties } from './generator';
export type { CppStateMachineGeneratorProperties } from './generator';

export function generateCpp(properties: CppStateMachineGeneratorProperties) {
    let generator = new Generator(properties);
    generator.generate();
}

