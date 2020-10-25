const Xst = require('xstate');
const fs = require('fs');
const Sqrl = require('squirrelly');
const path = require('path');

import { Machine, MachineConfig } from 'xstate';

export class Generator {
    readonly config: string;
    project: any;
    model: any;
    machine: any;

    constructor(config: string) {
        this.config = config;
    }

    generate() {
        const fileContents = fs.readFileSync(this.config, 'utf8');
        this.project = JSON.parse(fileContents);
        if (this.project.model_type == "json") {
            this.loadXstateModelJson();
        } else {
            this.loadXstateModel();
        }
        this.project.output_header = path.join(this.project.destination_path, this.machine.config.id + '_sm.h');
        this.project.output_test = path.join(this.project.destination_path, this.machine.config.id + '_test.cpp');
        this.genCppFiles();
    }

    loadXstateModelJson() {
        console.log('Loading Xstate model from JSON file ' + this.project.model);
        this.model = JSON.parse(fs.readFileSync(this.project.model, 'utf8'));
        this.machine = Machine(this.model);
        console.log(this.machine);
    }

    loadXstateModel() {
        console.log('Loading Xstate model from file ' + this.project.model);
        const fileContents = fs.readFileSync(this.project.model, 'utf8');
    }

    genCppFiles() {
        for (const [template, outputFile] of [
            ['src/base.template.h', this.project.output_header],
            ['src/test.template.cpp', this.project.output_test],
        ] as const) {
            const fileContents = fs.readFileSync(template, 'utf8');
            var result = Sqrl.render(fileContents, {
                machine: this.machine,
                project: this.project,
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
}
