const Xst = require('xstate');
const fs = require('fs');
const Sqrl = require('squirrelly');
const path = require('path');

import { Machine, MachineConfig, StateNode, StatesConfig, StateSchema, Event, EventObject } from 'xstate';
import { CppStateMachineGeneratorProperties } from './cpp_state_machine_generator';

export class Generator {
    readonly properties: CppStateMachineGeneratorProperties;
    machine: StateNode;
    outputHeaderShortname: string;
    outputHeader: string;
    outputCppCode: string;
    outputTest: string;

    constructor(properties: CppStateMachineGeneratorProperties) {
        this.properties = properties;
        this.machine = properties.xstateMachine;  // Saved for faster access.
        this.outputHeaderShortname = this.machine.config.id + '_sm.h';
        this.outputHeader = path.join(this.properties.destinationPath, this.outputHeaderShortname);
        this.outputCppCode = path.join(this.properties.destinationPath, this.machine.config.id + '_sm.cpp');
        this.outputTest = path.join(this.properties.destinationPath, this.machine.config.id + '_test.cpp');
    }

    generate() {
        //console.log(this.machine);
        console.log('\n\n');
        this.genCppFiles();
    }

    genCppFiles() {
        for (const [template, outputFile] of [
            [path.join(__dirname, 'base.template.h'), this.outputHeaderShortname],
            [path.join(__dirname, 'base.template.cpp'), this.outputCppCode],
            [path.join(__dirname, 'test.template.cpp'), this.outputTest],
        ] as const) {
            const fileContents = fs.readFileSync(template, 'utf8');
            var result = Sqrl.render(fileContents, {
                machine: this.machine,
                properties: this.properties,
                generator: this
            });
            fs.writeFileSync(outputFile, result);
        }
    }

    capitalize(str: string) {
        return str[0].toUpperCase() + str.substr(1).toLowerCase();
    }

    class() {
        var name = this.machine.config.id;
        return this.capitalize(name) + "SM";
    }

    events() {
        var result = new Set<string>();
        Object.keys(this.machine.states).forEach(nodeName => {
            var stateObj: StateNode<any, any, EventObject> = this.machine.states[nodeName];
            Object.keys(stateObj.on).forEach(eventName => {
                result.add(eventName);
            });
        });
        return Array.from(result.values());
    }

    transitionsForState(state: string): [string, string[]][] {
        let result: [string, string[]][] = [];
        var stateObj: StateNode<any, any, EventObject> = this.machine.states[state];
        Object.keys(stateObj.on).forEach(eventName => {
            var targetStates = stateObj.on[eventName];
            console.log('\n\n----------------');
            console.log(eventName);
            //console.log(targetStates);
            let targets: string[] = [];
            targetStates.forEach(targetState => {
                console.log(targetState['target']);
                console.log(targetState['target'][0]);
                targets.push(targetState['target'][0].key);
            });
            result.push([eventName, targets]);
        });

        return result;
    }

    initialState(): string {
        if (this.machine.initial != null) {
            return this.machine.initial.toString();
        }
        return "ERROR";
    }
}
