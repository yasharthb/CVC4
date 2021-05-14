# Adding a theory into CVC4
While attempting to add a theory for trigonometric functions into CVC4, I documented the outline of the steps that I 
followed through process. In this documentation, I have tried to address the issues I faced in the process.These include lack of proper and 
updated documentation of the implementation.

## Getting Started
All commands in this document have been ran on my ```Ubuntu 18.04 LTS``` system. To my understanding, this should work in
a similar fashion in ```Ubuntu 16.04+ / Debian 8.0+``` systems. You may want to adapt these to your system specific equivalents if need be.

To start with, set up CVC4 in ```development``` mode in your local device.<br>
Clone the project and create a new branch to work on.

```bash 
git clone https://github.com/yasharthb/CVC4
cd CVC4
git branch trigono
git checkout trigono
```

For building and installing CVC4, follow these instructions from the file [INSTALL.md](https://github.com/yasharthb/CVC4/blob/trigono/INSTALL.md).

CVC4 now uses ```Antlr``` as the parser generator, so to fetch and build Antlr from source, run the following script

```bash
./contrib/get-antlr-3.4  # Fetches and builds Antlr from source, based on system specs
```

## Generating Theory Structure
There are two kinds of theory additions you might do into CVC4: 
- New Theory: a new decision procedure which may or may not be existing already in the solver as a part of a theory.
- Alternate Theory: an alternate implementation of an existing host theory. Flag to select one implementation ```--use-theory``` 

In this documentation, we shall have a look at adding a new theory which was applicable in the case of trigonometric theory
which was not available as a distinct entity but as transcendentals, a sub part of the arithmetic theory.

The theory templates provided in the repository are incompatible with the current state of the project.<br>
Here, I have provided slightly modified templates for the CVC4 theory addition that one might want to use.<br>
To generate theory structure files use the following commands:
```bash
./contrib/new-theory trigono  # Use your theory name instead
```
On running the above command, you'll may expect an output on the lines of:
```bash
Theory of trigono
Theory directory: src/theory/trigono
Theory id: THEORY_TRIGONO
Theory class: CVC4::theory::trigono::TheoryTrigono

Creating src/theory/trigono/kinds...
Creating src/theory/trigono/README.WHATS-NEXT...
Creating src/theory/trigono/theory_trigono.cpp...
Creating src/theory/trigono/theory_trigono.h...
Creating src/theory/trigono/theory_trigono_rewriter.h...
Creating src/theory/trigono/theory_trigono_type_rules.h...
.........
Creating src/options/trigono_options.toml...
.........
.........
```
Now, once these files have been added, build the project, configure and make.
```bash
cd <build_dir>   # default is ./build
cd ..
./configure   #with any other options, like using a 'debug' configuration and specifying antlr.
make
```

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
The successful addition of the core logic for the theory is a tricky task. Depending on what the needs of the theory are, 
one might need to implement various virtual functions belonging to ```Theory``` Class which is has 

### Rewriter

### Type Rules

## Resources
Some of these resources were very helpful in understanding the working of CVC4 as well as adding new functionalities into
it.
- [Design of Theory Solvers in CVC4](https://homepage.divms.uiowa.edu/~ajreynol/pres-theory-solvers14.pdf)
- [A tour of the CVC4 Solver](https://www.cs.utexas.edu/users/hunt/FMCAD/FMCAD14/slides/cvc4-tutorial-fmcad.pdf)
- [CVC4 Architecture Notes(Hand written)](https://raw.githubusercontent.com/wiki/cvc5/cvc5/doc/cvc4-architecture-notes.pdf)
- [How to write a theory in CVC4 (old instructions)](http://cvc4.cs.stanford.edu/wiki/How_to_write_a_theory_in_CVC4_(old_instructions))
