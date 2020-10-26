const Sqrl = require('squirrelly');
const Xst = require('xstate');
const program = require('commander');
const chalk = require('chalk');
const figlet = require('figlet');

const Gen = require('./generator');

program
  .version('1.0.0')
  .description("Generate C++ State Machine from Xstate definitions")
  .requiredOption('-p, --project <project>', 'Specify the project file [project.json]')
  .parse(process.argv);

console.log(
    chalk.red(
        figlet.textSync('Xstate c++ gen', { horizontalLayout: 'full' })
    )
);

let generator = new Gen.Generator(program.project);
generator.generate();
