# Adding a theory into CVC4
While attempting to add a theory for trigonometric functions into CVC4, I documented the outline of the steps that I 
followed through process.
In this documentation, I have tried to address the issues I faced in the process.<br> These include lack of proper and 
updated documentation. The documentation available with CVC4 around the addition of the theories is deprecated as of 2016.<br>

## Getting Started
To start with, set up CVC4 in ```development``` mode in your local device. For doing this, follow these build and setup instructions
in the file [INSTALL.md](https://github.com/CVC4/CVC4/blob/master/INSTALL.md).

All commands in this document have been ran on my ```Ubuntu 18.04 LTS``` system. To my understanding, this should work in
a similar fashion in ```Ubuntu 16.04+ / Debian 8.0+``` systems.<br>
You may want to adapt these to your system specific equivalents if need be.

## Generating Theory Structure
There are two kinds of theory additions you might do into CVC4: 
- New Theory: a new decision procedure which may or may not be existing already in the solver as a part of a theory.
- Alternate Theory: an alternate implementation of an existing host theory. Flag to select one implementation ```--use-theory``` 

In this documentation, we shall have a look at adding a new theory which was applicable in the case of trigonometric theory
which was not available as a distinct entity but as transcendentals, a sub part of the arithmetic theory.

The theory templates provided in the repository are incompatible with the current state of the project.
Here, I have provided slightly modified templates for the CVC4 theory addition that one might want to use.

## Integrating the Theory
Once you have generated the theory files in the previous steps, you need to include these into the project build.

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
The successful addition of the core logic for the theory is a tricky task. Depending on what the needs of the theory.

### Rewriter

### Type Rules