; COMMAND-LINE: --nl-ext
; EXPECT: unsat
(set-logic QF_UFNRAT)
(declare-fun x () Real)
(assert (not (= (t_cos 0.0) 1.0)))
(check-sat)
