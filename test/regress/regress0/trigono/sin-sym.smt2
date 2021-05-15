; COMMAND-LINE: --nl-ext --nl-ext-tplanes
; EXPECT: unsat
(set-logic QF_UFNRAT)
(declare-fun x () Real)
(assert (not (= (+ (t_sin 0.2) (t_sin (- 0.2))) 0.0)))
(check-sat)
