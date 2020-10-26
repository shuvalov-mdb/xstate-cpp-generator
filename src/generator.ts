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
    outputTest: string;

    constructor(properties: CppStateMachineGeneratorProperties) {
        this.properties = properties;
        this.machine = properties.xstateMachine;  // Saved for faster access.
        this.outputHeaderShortname = this.machine.config.id + '_sm.h';
        this.outputHeader = path.join(this.properties.destinationPath, this.outputHeaderShortname);
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

    Class() {
        var name = this.machine.config.id;
        name = name[0].toUpperCase() + name.substr(1).toLowerCase();
        return name + "SM";
    }

    transitions() {
        var result = new Set<string>();
        Object.keys(this.machine.states).forEach(nodeName => {
            var node = this.machine.states[nodeName];
            console.log(node.on);
            Object.keys(node.on).forEach(transitionName => {
                result.add(transitionName);
                console.log(transitionName);
            });
        });
        return Array.from(result.values());
    }
}
