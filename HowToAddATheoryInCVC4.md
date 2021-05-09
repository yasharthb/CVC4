# Adding a theory into CVC4
The documentation available with CVC4 for the addition of the theories is outdated as of 2016.
Thus, while attempting to add a theory for trigonometric functions into CVC4, I documented the outline of the process.
In this documentation, I have tried to address the issues I faced in the process.

## Getting Started

## Generating Theory Structure
The theory templates provided in the repository are mostly outdated and
might be incompatible when you plan to use them in your project.
Here, I have provided modified templates for the CVC4 theory addition.

## Integrating the Theory
Once you have generated the theory files in the previous steps. You need to include these into the project build.

## Registering the Theory


## Taking Input
So, even if you have been able to just embed the theory structure and are now able to build the project.

### Lexer
Firstly, you need to enable the lexer with the lexemes you need as a part of your new theory.
### Parser

## Giving Output

### Printer

## Activating the Theory

## The Theory Logic

### Kinds

### Theory

### Rewriter

### Type Rules