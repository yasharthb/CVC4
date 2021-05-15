; COMMAND-LINE: --nl-ext-tf-tplanes
; EXPECT: unsat
(set-logic QF_NRAT)
(declare-fun x () Real)
(declare-fun y () Real)

(assert (= (t_sin x) 1))
(assert (= (t_cos y) 0.5))
(check-sat)
