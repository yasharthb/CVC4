; EXPECT: sat
(set-logic QF_BV)
(set-option :solve-bv-as-int sum)
(set-option :incremental true)
(declare-fun _substvar_27_ () Bool)
(declare-const bv_40-3 (_ BitVec 40))
(assert (= bv_40-3 (_ bv0 40)))
(push 1)
(assert _substvar_27_)
(check-sat)