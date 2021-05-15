# Adding a theory into CVC4
While attempting to add a theory for trigonometric functions into CVC4, I documented the outline of the steps that I 
followed through process. In this documentation, I have tried to address the issues I faced in the process.These include lack of proper and 
updated documentation of the implementation.

### The CVC4 architecture
Here is a rough attempt at portraying the CVC4 architecture based on discussion notes from the developers.
This might be useful once you start navigating the codebase following this flow of control. Although, in my case I did not need to meddle with
the Propositional Engine and the SAT Solver (yet!).
<div align="center"><img src="https://i.ibb.co/6NkhXhC/Hnet-com-image-1.jpg" width="500" height="650" alt="CVC4 Architecture Diagram"></div>

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
There...There! Now you are all set to add some theories to CVC4.

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

The ```README.WHATS-NEXT``` file in the ```src/theory/trigono``` directory is a very good insight into the components of a theory.
## Integrating the Theory
Once you have generated the theory files in the previous steps, you need to include these into the project build.
<br>To do this you will have to make the following additions:
- ```src/CMakeLists.txt```
```c++
libcvc4_add_sources(
.......
theory/trigono/theory_trigono.cpp                     // All theory source files to the src cmake list
theory/trigono/theory_trigono.h
theory/trigono/theory_trigono_rewriter.cpp
theory/trigono/theory_trigono_rewriter.h
theory/trigono/theory_trigono_type_rules.h
)
.......
set(KINDS_FILES
  ......
  ${PROJECT_SOURCE_DIR}/src/theory/quantifiers/kinds
  ${PROJECT_SOURCE_DIR}/src/theory/trigono/kinds)      // Add the theory kinds file to the src cmake list
........
```
-  ```src/options/CMakeLists.txt```
```c++
set(options_toml_files
   ......
   uf_options.toml
   trigono_options.toml                                // Add Theory options file to the options make list
   )
```
   
## Registering the Theory


## Taking Input
So, even if you have been able to just embed the theory structure and are now able to build the project.<br>
We will look at how to support ```smtlib2``` for input.

### Parser
For the parser to correctly pattern match the rules and later semantically evaluate the expression, we must add relevant
markers for the new theory.

- src/parser/smt2/smt2.cpp
```c++
// This function is called when the Trigonometric theory is enabled.
// It adds all the operators in the theory to be taken into cognizance while parsing through the rules.
void Smt2::addTrigonoOperators()   
{            
  addOperator(api::TRIG_SINE, "t_sin");
  addOperator(api::TRIG_COSINE, "t_cos");
  .........
  addOperator(api::TRIG_ARCSECANT, "t_arcsec");
  addOperator(api::TRIG_ARCCOTANGENT, "t_arccot");
}

// Checks if theory enabled and invokes the corresponding function
if (d_logic.isTheoryEnabled(theory::THEORY_TRIGONO)) { 
    defineVar("trigono.pi", d_solver->mkTerm(api::PI));
    addTrigonoOperators();
  }
```
- ```src/parser/smt2/smt2.h```
```c++
  void addTrigonoOperators(); // Add the function declaration in the header.
```

### CVC4 API

- ```src/api/cvc4cpp.cpp```
```c++
const static std::unordered_map<Kind, CVC4::Kind, KindHashFunction> s_kinds{
    ..........
    /* Trigono ---------------------------------------------------------- */
    {TRIG_SINE, CVC4::Kind::TRIG_SINE},
    {TRIG_COSINE, CVC4::Kind::TRIG_COSINE},
    {TRIG_TANGENT, CVC4::Kind::TRIG_TANGENT},
    .........
    {TRIG_ARCSECANT, CVC4::Kind::TRIG_ARCSECANT},
    {TRIG_ARCCOTANGENT, CVC4::Kind::TRIG_ARCCOTANGENT},
    ..........
    
const static std::unordered_map<CVC4::Kind, Kind, CVC4::kind::KindHashFunction>
    ........
    /* Trigono ------------------------------------------------------ */
    {CVC4::Kind::TRIG_SINE, TRIG_SINE},
    {CVC4::Kind::TRIG_COSINE, TRIG_COSINE},
    {CVC4::Kind::TRIG_TANGENT, TRIG_TANGENT},
    .......
    {CVC4::Kind::TRIG_ARCSECANT, TRIG_ARCSECANT},
    {CVC4::Kind::TRIG_ARCCOTANGENT, TRIG_ARCCOTANGENT},    
    ........
```
- ```src/api/cvc4cppkind.h```
```c++
 /* Trigono  -------------------------------------------------------------- */

  /**
  * Sine.
  * Parameters: 1
  *   -[1]: Term of sort Integer, Real
  * Create with:
  *   mkTerm(Kind kind, Term child)
  */
  TRIG_SINE,
  /**
   * Cosine.
   * Parameters: 1
   *   -[1]: Term of sort Integer, Real
   * Create with:
   *   mkTerm(Kind kind, Term child)
   */
  TRIG_COSINE,
  ........                                            // Add all kinds associated with the theory
    /**
   * Arc sine.
   * Parameters: 1
   *   -[1]: Term of sort Integer, Real
   * Create with:
   *   mkTerm(Kind kind, Term child)
   */
  TRIG_ARCSINE,
  /**
   * Arc cosine.
   * Parameters: 1
   *   -[1]: Term of sort Integer, Real
   * Create with:
   *   mkTerm(Kind kind, Term child)
   */
  TRIG_ARCCOSINE,
  ........
```
## Giving Output

### Printer
Not a lot to be done in the printer part. Also, without these additions the solver would print the internal attributes as the default case -- so not 
a mandatory step to start with.

- ```src/printer/smt2/smt2_printer.cpp```
```c++
// Matches through a set of cases to enable printing formats specific to a particular Term Node Kind
void Smt2Printer::toStream(std::ostream& out,TNode n, int toDepth, LetBinding* lbind) const{
  .......
  // trigono theory
  case kind::TRIG_SINE:                           // One needs to enable specific print format for each Kind
  case kind::TRIG_COSINE:
  .......
  case kind::TRIG_ARCCOTANGENT:
  case kind::TRIG_PI:
  out << smtKindString(k, d_variant) << " ";      // Call to the function below to print the token associated
  .......
  
// Maps Term Kinds to tokens that should get printed onto the console 
std::string Smt2Printer::smtKindString(Kind k, Variant v){
  .......
  //trigono theory
  case kind::TRIG_SINE: return "t_sin";          // Console Tokens corresponding to the Kinds
  case kind::TRIG_COSINE: return "t_cos";        // Generally kept same as the input tokens
  .......
  case kind::TRIG_ARCCOTANGENT: return "t_arccot";
  case kind::TRIG_PI: return "t.pi";
  .......
```

## Enabling the Theory
So, while you have come along a long way into using your theory, there is an important step remaining before you may 
just get the control to reach within your new theory. You need to be able to activate your theory through smt2 ```setlogic``` command
for usage in the model. 

For the Trigonometric theory, I have added triggers for enabling it wherever we find Non-Linear Arithmetic Transcendentals
in the picture. You'll have to figure if your triggers need a separate enabling flag or lie within a particular subset of
another logic configuration.

## The Theory Logic

### Kinds
The ```src/theory/built-in/kinds``` file has a fairly well documented a ```kind``` file and what all need to
be incorporated in it. I'll skip the explanation for now. You should realize what are the constants, functions and operators in your theory and should write them down in a 
manner analogous to the other theories like ```arith``` and ```sets```. In my case, ```arith``` was particularly very helpful.

```text
properties stable-infinite                           # This can be understood from src/theory/built-in/kinds
properties check

# Theory content goes here.

# types...
typerule TSINE "SimpleTypeRule<RReal, AReal>"         # You need a typerule for every type you define here
typerule TCOSINE "SimpleTypeRule<RReal, AReal>"       # In my case, I borrowed the builtin Generic Type Rules
.......
typerule TARCSECANT "SimpleTypeRule<RReal, AReal>"
typerule TARCCOTANGENT "SimpleTypeRule<RReal, AReal>"

# operators...
operator TSINE 1 "sine"                               # Each operator of the theory is declared here 
operator TCOSINE 1 "cosine"                           # Also, has a number of args & internal name associated
.........
operator TARCSECANT 1 "arc secant"
operator TARCCOTANGENT 1 "arc cotangent"
nullaryoperator TRIG_PI "pi"

endtheory
```

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
