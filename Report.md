# CS253A: Advanced Track Project

## Objective 
    Add a new theory/decision procedure for Trigonometric functions in CVC4.
## Tasks Completed

*  Introduced lexical tokens for Trigonometric theory into the lexer.
*  Added rules for these tokens to be recognized by the parser.
*  Linked the parser tokens to external kinds of the theory.
*  Mapped each external kind to an internal kind of the theory.
*  Created the structure of the new theory (theory, kinds, rules).
*  Integrated and registered the new theory with the theory engine.
*  Added logic to enable theory in a particular model.
*  Defined operators, constants and functions in the internal theory kinds.
*  Added logic to identify and separate assertions and terms belonging to the trigonometric theory.
*  Added new Inferences into the Inference manager pertaining to the trigonometric theory.   
*  PreNotifyTerm hook for all terms belonging to the new theory.
*  Added conflict mechanism in equality assertions in case of invalid assignments at NotifyTerm stage.

## Documentation
For a detailed explanation of the process to add a new theory procedure into CVC4, check out this file [HowToAddATheoryToCVC4.md](https://github.com/yasharthb/CVC4/blob/trigono/HowToAddATheoryInCVC4.md).

### Enabling Trigonometric Theory

Just as other theories in CVC4, one needs to enable the Trigonometric mode if trigonometric procedures are needed to evaluate a
particular expression. One can do the same by enabling Non-Linear Real Arithmetic and Transcendental logic. This can be 
done by adding ```N``` before ```RA, or IRA``` for the non-linear fragment of those arithmetic logics.
Transcendentals can be enabled by adding a ```T``` at the end of the Real Arithmetic fragments. Refer to the example below:

```smt2
   (set-logic QF_NRAT) # Enables Quantifier Free, Non-Linear, Real Arithmetic logic with Transendentals.
```
### Using new Trigonometric functions
To avoid a clash with the existing Trigonometric terms in Arithmetic theory, we have introduced
new function names for each function.
```smt2
    (t_sin x), (t_cos x), (t_tan x)           # Alias for (sin x), (cos x), (tan x)
    
    (t_arcsin x), (t_arccos x), (t_arctan x)  # Alias for (arcsin x), (arccos x), (arct_tan x)
    
    trig.pi                                   # Alias for pi
```

## Future Tasks
* Handling of shared terms with various theories(Arithmetic, Boolean, UF).
* Handling of assertions with foreign terms.
* Sophisticated mechanisms for creating faster converging assertions.
* Optimization of the lookup mechanism.

## Goals: Where to ?

> I wish to complete the decision procedure to a level of sophistication that we may be able to publish this as 
a research paper, before I graduate in December '21. This was my aspiration while opting for the project, if nothing more, it feels even more doable and fun now.

## Acknowledgement
> I'd like to thank both my mentors [Sujit Muduli](https://www.cse.iitk.ac.in/users/smuduli/) and [Prof. Subhajit Roy](https://www.cse.iitk.ac.in/users/subhajit/) for their support with me throughout the project. 
> Without them explaining the theoretical aspects to me, it would have been impossible for me to execute this project 
> without the domain acquaintance. Sujit also helped me with his insights from his CVC4 experience.
