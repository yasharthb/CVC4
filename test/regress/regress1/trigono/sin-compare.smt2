; COMMAND-LINE: --nl-ext --nl-ext-tplanes
; EXPECT: unsat
(set-logic QF_UFNRAT)
(declare-fun x () Real)
(assert (or (> (t_sin 0.1) (t_sin 0.2)) (> (t_sin 6.4) (t_sin 6.5))))
(check-sat)
