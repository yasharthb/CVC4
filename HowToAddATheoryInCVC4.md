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
./contrib/new-theory-updated trigono  # Use your theory name instead

                 # or

./contrib/new-theory trigono  # You may use the original skeletons; however, they'll not build before debug
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
./configure   #with any other options, like using a 'debug' configuration and specifying antlr.
make
```

The ```README.WHATS-NEXT``` file in the ```src/theory/trigono``` directory is a very good insight into the components of a theory.
## Incorporating the Theory
Once you have generated the theory files in the previous steps, you need to include these into the project build.
<br>To do this you will have to make the following additions:
- ```src/CMakeLists.txt```
```c++
// All theory source files to the src cmake list
libcvc4_add_sources(
.......
theory/trigono/theory_trigono.cpp                     
theory/trigono/theory_trigono.h
theory/trigono/theory_trigono_rewriter.cpp
theory/trigono/theory_trigono_rewriter.h
theory/trigono/theory_trigono_type_rules.h
)
.......

// Add the theory kinds file to the src cmake list set
set(KINDS_FILES
  ......
  ${PROJECT_SOURCE_DIR}/src/theory/quantifiers/kinds
  ${PROJECT_SOURCE_DIR}/src/theory/trigono/kinds)      
........
```
-  `src/options/CMakeLists.txt`
```c++
// Add Theory options file to the options make list set
set(options_toml_files
   ......
   uf_options.toml
   trigono_options.toml                                
   )
```
At this point, you might want to build the project and see if it compiles alright. It is recommended that you make this
happen before proceeding further.

## Registering the Theory
The new theory needs to be registered with the Theory Engine so that it might start sending assertions and notifying 
all associated terms to the theory. This can be done through a couple of additions.

- `src/theory/theory_engine.cpp`
```c++
// Added the new theory to the MACRO for executing statements for all theories
#define CVC4_FOR_EACH_THEORY                                   \
.....
CVC4_FOR_EACH_THEORY_STATEMENT(CVC4::theory::THEORY_ARITH)     \
CVC4_FOR_EACH_THEORY_STATEMENT(CVC4::theory::THEORY_TRIGONO)   \
CVC4_FOR_EACH_THEORY_STATEMENT(CVC4::theory::THEORY_FP)        \
.....
```
- ```src/theory/theory_id.cpp```
```c++
// Associates a THEORY_ID with its representational name. Default value being: UNKNOWN_THEORY
std::ostream& operator<<(std::ostream& out, TheoryId theoryId)
    ........
    case THEORY_ARITH: out << "THEORY_ARITH"; break;
    case THEORY_TRIGONO: out << "THEORY_TRIGONO"; break;
    case THEORY_BV: out << "THEORY_BV"; break;
    ........
    
// Associates a THEORY_ID with its representational name in Stats Prefixes. Default value being: unknown   
std::string getStatsPrefix(TheoryId theoryId)
    .......
    case THEORY_ARITH: return "theory::arith"; break;
    case THEORY_TRIGONO: return "theory::trigono"; break;
    case THEORY_BV: return "theory::bv"; break;
    .......
```
- `src/theory/inference_id.h`: Optional for now. Used when creating lemmas/raising conflicts while checking consistency.
```c++
// Define ENUMS for inferences that lead to new lemmas/ raising conflicts
enum class InferenceId
  ........
  // ---------------------------------- trigono theory
  TRIGONO_EQ,
  TROGONO_NOT_EQ,
  TRIGONO_OUT_OF_BOUND,
  // ---------------------------------- end trigono theory
  ........
```
## Enabling the Theory
So, while you have come along a long way into using your theory, there is an important step remaining before you may
just get the control to reach within your new theory. You need to be able to activate your theory through smt2 ```setlogic``` command
for usage in the model.

For the Trigonometric theory, I have added triggers for enabling it wherever we find Non-Linear Arithmetic Transcendentals
in the picture. You'll have to figure if your triggers need a separate enabling flag or lie within a particular subset of
another logic configuration.

- `src/theory/logic_info.cpp`
```c++
// Creates a mapping in the logicInfo object corresponding to all theories enabled based on the logic string.
void LogicInfo::setLogicString(std::string logicString)
        if (*p == 'T')
        {
          arithTranscendentals();
          enableTheory(THEORY_TRIGONO);
          p += 1;
        }
      } else if(!strncmp(p, "NIRA", 4)) {
      ...........
        if (*p == 'T')
        {
          arithTranscendentals();
          enableTheory(THEORY_TRIGONO);
          p += 1;
        }
      }
```

- `src/smt/set_defaults.cpp`: If you need your Theory to be default in some scenarios(due to dependencies), make edits to this file.

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
// Create a mapping from external kind to Internal kind in the API
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

// Create a mapping from internal kinds to External kinds in the API
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
  ........                                  // Add all kinds associated with the theory, as ENUMS in the file
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
  case kind::TRIG_PI: return "trig.pi";
  .......
```

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
one might need to implement various virtual functions belonging to `Theory` Class. This step is very specific to the theory
being implemented. You may have a look at my implementations of the following functions **[WIP]**:

- `finishInit()`: Hook for kinds to be set as an unevaluated kind inside the theory where it is used, making it easy to check for illegal
  eliminations via TheoryModel.<br><br>
- `check()`: checks the current assigment's consistency. The most important check() invariant is that when check is 
  called with full effort and the current assignment is unsatisfiable it must either return a conflict or force the solver to split by adding a lemma.<br><br>
- `preNotifyFact()`: Hook usually used to check successful addition of the fact to the equality engine before notification. <br><br>
- `notifyFact()`: Informs the theory of a fact of the theory and expects to conflict raised if there is some error at this stage.<br><br>
- `needsEqualityEngine()`:  Checks whether the theory needs an Equality Engine and returns a boolean values.<br><br>
- `needsCheckLastEffort()`: Checks whether the theory needs a final high effort Check before the satisfiability is returned.<br><br>
- `propagate()*`: - propagate new literal assignments in the current context. Note that it can only propagate theory atoms, or
  negations of atoms, that already have a literal in the SAT solver. An invariant of the function is that it should never
  propagate a literal that is already assigned to true or false.
### Handling the assertions
Once you receive a fact from `d_fact` queue of your theory (Use the `get()` function for this), you need to establish its consistency. If the assignment
is invalid you must raise a conflict at `full effort`, else send an appropriate lemma that is asserted iteratively so to converge to 
an overall satisfiable solution. 

In my case, we are currently exploring options to assert back lemmas in form of domain specific value assignments for the Trigonometric assertions
that have been sent to the Theory. These assertions choose a set a of assignments consistent with the assertion queue,
and equality constraints in the equality engine. These constraints shall then be sent back to the SMT engine for 
satisfiability check. This happens in an iterative fashion till you arrive at a conflict making the check
unsatisfiable or till you have values that satify the assertions and *lastCheckEfforts* have been invoked. This is for the
SMT engine to decide and give back to the propositional engine.

For the Trigonometric theory, we shall be working on sophisticated procedures which can make this iterative process
converge faster needing lesser new lemmas. This can be done by keeping in consideration all facts in the current context
while deciding new assignments. These might include any previous assignments which were not satisfiable in the bigger picture.

### Rewriter
A Theory Rewriter is used to rewrite a node into the normal/canonical form of the theory in consideration. Depending on
the needs of the theory, you might require rewrites based.

For the Trigonometric theory, no adapted implementations were required. The script generated implementations of the of 
the `theory/trigono/theory_trigono_rewriter.h` by the `new-thoery-updated` script were sufficient to handle dummy
rewrites.

### Type Rules
Every type defined in the kinds file of a theory needs to be associate to a type rule, which guides the functionalities
of the type and provides implementations for operations that might be performed on/or using it.

For the implementation of Trigonometric functions, I used the in built Generic SimpleTypeRule from the `expr/type_checker_util.h`
with argument and return types specified.
```text
typerule TSINE "SimpleTypeRule<RReal, AReal>"           # <Return type Real, Argument type Real> 
```
You may define theory specific Type Rules for the needs of your theory if any of the builtin type rules in
`expr/type_checker_util.h` and `theory/builtin/theory_builtin_type_rules.h` do not match your needs.
<br><br>

> At the end of all implementation mentioned above, you get a new theory - **a "complete" (though slightly unsound) new theory!**

## Resources
Some of these resources were very helpful in understanding the working of CVC4 as well as adding new functionalities into
it.
- [Design of Theory Solvers in CVC4](https://homepage.divms.uiowa.edu/~ajreynol/pres-theory-solvers14.pdf)
- [A tour of the CVC4 Solver](https://www.cs.utexas.edu/users/hunt/FMCAD/FMCAD14/slides/cvc4-tutorial-fmcad.pdf)
- [CVC4 Architecture Notes(Hand written)](https://raw.githubusercontent.com/wiki/cvc5/cvc5/doc/cvc4-architecture-notes.pdf)
- [How to write a theory in CVC4 (old instructions)](http://cvc4.cs.stanford.edu/wiki/How_to_write_a_theory_in_CVC4_(old_instructions))
- [CVC4 API Docs (Excellent to learn about kinds)](https://cvc5.github.io/docs/cpp/kind.html)