# CS253A: Advanced Track Project

## Objective 
    Add a theory for Trigonometric functions in CVC4.
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

Note: The documentation for CVC4 is deprecated as of 2016. The project structure, various implementations

## Documentation

### Activating Trigonometric Theory
Just as other theories in CVC4, one needs to activate the Trigonometric mode if trigonometric  

```smt2
    
```

## Future Tasks
* Handling of shared terms with various theories(Arithmetic, Boolean, UF).
* Handling of assertions with foreign terms.
* Optimization of the lookup mechanism


