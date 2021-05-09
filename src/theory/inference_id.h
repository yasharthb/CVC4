/*********************                                                        */
/*! \file inference_id.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Gereon Kremer, Yoni Zohar
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Inference enumeration.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__INFERENCE_ID_H
#define CVC4__THEORY__INFERENCE_ID_H

#include <map>
#include <vector>

#include "util/safe_print.h"

namespace CVC4 {
namespace theory {

/** Types of inferences used in the procedure
 *
 * Note: The order in this enum matters in certain cases (e.g. inferences
 * related to normal forms in strings), where inferences that come first are
 * generally preferred.
 *
 * Notice that an inference is intentionally distinct from PfRule. An
 * inference captures *why* we performed a reasoning step, and a PfRule
 * rule captures *what* reasoning step was used. For instance, the inference
 * LEN_SPLIT translates to PfRule::SPLIT. The use of stats on inferences allows
 * us to know that we performed N splits (PfRule::SPLIT) because we wanted
 * to split on lengths for string equalities (Inference::LEN_SPLIT).
 */
enum class InferenceId
{
  // ---------------------------------- arith theory
  //-------------------- preprocessing
  ARITH_PP_ELIM_OPERATORS,
  //-------------------- nonlinear core
  // simple congruence x=y => f(x)=f(y)
  ARITH_NL_CONGRUENCE,
  // shared term value split (for naive theory combination)
  ARITH_NL_SHARED_TERM_VALUE_SPLIT,
  // checkModel found a conflict with a quadratic equality
  ARITH_NL_CM_QUADRATIC_EQ,
  //-------------------- nonlinear incremental linearization solver
  // splitting on zero (NlSolver::checkSplitZero)
  ARITH_NL_SPLIT_ZERO,
  // based on sign (NlSolver::checkMonomialSign)
  ARITH_NL_SIGN,
  // based on comparing (abs) model values (NlSolver::checkMonomialMagnitude)
  ARITH_NL_COMPARISON,
  // based on inferring bounds (NlSolver::checkMonomialInferBounds)
  ARITH_NL_INFER_BOUNDS,
  // same as above, for inferences that introduce new terms
  ARITH_NL_INFER_BOUNDS_NT,
  // factoring (NlSolver::checkFactoring)
  ARITH_NL_FACTOR,
  // resolution bound inferences (NlSolver::checkMonomialInferResBounds)
  ARITH_NL_RES_INFER_BOUNDS,
  // tangent planes (NlSolver::checkTangentPlanes)
  ARITH_NL_TANGENT_PLANE,
  //-------------------- nonlinear transcendental solver
  // purification of arguments to transcendental functions
  ARITH_NL_T_PURIFY_ARG,
  // initial refinement (TranscendentalSolver::checkTranscendentalInitialRefine)
  ARITH_NL_T_INIT_REFINE,
  // pi bounds
  ARITH_NL_T_PI_BOUND,
  // monotonicity (TranscendentalSolver::checkTranscendentalMonotonic)
  ARITH_NL_T_MONOTONICITY,
  // tangent refinement (TranscendentalSolver::checkTranscendentalTangentPlanes)
  ARITH_NL_T_TANGENT,
  // secant refinement, the dual of the above inference
  ARITH_NL_T_SECANT,
  //-------------------- nonlinear iand solver
  // initial refinements (IAndSolver::checkInitialRefine)
  ARITH_NL_IAND_INIT_REFINE,
  // value refinements (IAndSolver::checkFullRefine)
  ARITH_NL_IAND_VALUE_REFINE,
  // sum refinements (IAndSolver::checkFullRefine)
  ARITH_NL_IAND_SUM_REFINE,
  // bitwise refinements (IAndSolver::checkFullRefine)
  ARITH_NL_IAND_BITWISE_REFINE,
  //-------------------- nonlinear cad solver
  // conflict / infeasible subset obtained from cad
  ARITH_NL_CAD_CONFLICT,
  // excludes an interval for a single variable
  ARITH_NL_CAD_EXCLUDED_INTERVAL,
  //-------------------- nonlinear icp solver
  // conflict obtained from icp
  ARITH_NL_ICP_CONFLICT,
  // propagation / contraction of variable bounds from icp
  ARITH_NL_ICP_PROPAGATION,
  // ---------------------------------- end arith theory

  // ---------------------------------- trigono theory
  TRIGONO_EQ,
  TROGONO_NOT_EQ,
  TRIGONO_OUT_OF_BOUND,
  // ---------------------------------- end trigono theory

  // ---------------------------------- arrays theory
  ARRAYS_EXT,
  ARRAYS_READ_OVER_WRITE,
  ARRAYS_READ_OVER_WRITE_1,
  ARRAYS_READ_OVER_WRITE_CONTRA,
  // ---------------------------------- end arrays theory

  // ---------------------------------- bags theory
  BAG_NON_NEGATIVE_COUNT,
  BAG_MK_BAG_SAME_ELEMENT,
  BAG_MK_BAG,
  BAG_EQUALITY,
  BAG_DISEQUALITY,
  BAG_EMPTY,
  BAG_UNION_DISJOINT,
  BAG_UNION_MAX,
  BAG_INTERSECTION_MIN,
  BAG_DIFFERENCE_SUBTRACT,
  BAG_DIFFERENCE_REMOVE,
  BAG_DUPLICATE_REMOVAL,
  // ---------------------------------- end bags theory

  // ---------------------------------- bitvector theory
  BV_BITBLAST_CONFLICT,
  BV_LAZY_CONFLICT,
  BV_LAZY_LEMMA,
  BV_SIMPLE_LEMMA,
  BV_SIMPLE_BITBLAST_LEMMA,
  BV_EXTF_LEMMA,
  BV_EXTF_COLLAPSE,
  // ---------------------------------- end bitvector theory

  // ---------------------------------- datatypes theory
  // (= k t) for fresh k
  DATATYPES_PURIFY,
  // (= (C t1 ... tn) (C s1 .. sn)) => (= ti si)
  DATATYPES_UNIF,
  // ((_ is Ci) t) => (= t (Ci (sel_1 t) ... (sel_n t)))
  DATATYPES_INST,
  // (or ((_ is C1) t) V ... V ((_ is Cn) t))
  DATATYPES_SPLIT,
  // (or ((_ is Ci) t) V (not ((_ is Ci) t)))
  DATATYPES_BINARY_SPLIT,
  // (not ((_ is C1) t)) ^ ... [j] ... ^ (not ((_ is Cn) t)) => ((_ is Cj) t)
  DATATYPES_LABEL_EXH,
  // (= t (Ci t1 ... tn)) => (= (sel_j t) rewrite((sel_j (Ci t1 ... tn))))
  DATATYPES_COLLAPSE_SEL,
  // (= (Ci t1...tn) (Cj t1...tn)) => false
  DATATYPES_CLASH_CONFLICT,
  // ((_ is Ci) t) ^ (= t (Cj t1 ... tn)) => false
  DATATYPES_TESTER_CONFLICT,
  // ((_ is Ci) t) ^ ((_ is Cj) s) ^ (= t s) => false
  DATATYPES_TESTER_MERGE_CONFLICT,
  // bisimilarity for codatatypes
  DATATYPES_BISIMILAR,
  // corecursive singleton equality
  DATATYPES_REC_SINGLETON_EQ,
  // corecursive singleton equality (not (= k1 k2)) for fresh k1, k2
  DATATYPES_REC_SINGLETON_FORCE_DEQ,
  // cycle conflict for datatypes
  DATATYPES_CYCLE,
  //-------------------- datatypes size/height
  // (>= (dt.size t) 0)
  DATATYPES_SIZE_POS,
  // (=> (= (dt.height t) 0) => (and (= (dt.height (sel_1 t)) 0) .... ))
  DATATYPES_HEIGHT_ZERO,
  // ---------------------------------- end datatypes theory

  //-------------------------------------- quantifiers theory
  // skolemization
  QUANTIFIERS_SKOLEMIZE,
  // Q1 <=> Q2, where Q1 and Q2 are alpha equivalent
  QUANTIFIERS_REDUCE_ALPHA_EQ,
  //-------------------- counterexample-guided instantiation
  // G2 => G1 where G2 is a counterexample literal for a nested quantifier whose
  // counterexample literal is G1.
  QUANTIFIERS_CEGQI_CEX_DEP,
  // 0 < delta
  QUANTIFIERS_CEGQI_VTS_LB_DELTA,
  // delta < c, for positive c
  QUANTIFIERS_CEGQI_VTS_UB_DELTA,
  // infinity > c
  QUANTIFIERS_CEGQI_VTS_LB_INF,
  //-------------------- sygus solver
  // preprocessing a sygus conjecture based on quantifier elimination, of the
  // form Q <=> Q_preprocessed
  QUANTIFIERS_SYGUS_QE_PREPROC,
  // G or ~G where G is the active guard for a sygus enumerator
  QUANTIFIERS_SYGUS_ENUM_ACTIVE_GUARD_SPLIT,
  // manual exclusion of a current solution
  QUANTIFIERS_SYGUS_EXCLUDE_CURRENT,
  // manual exclusion of a current solution for sygus-stream
  QUANTIFIERS_SYGUS_STREAM_EXCLUDE_CURRENT,
  // ~Q where Q is a PBE conjecture with conflicting examples
  QUANTIFIERS_SYGUS_EXAMPLE_INFER_CONTRA,
  //-------------------------------------- end quantifiers theory

  // ---------------------------------- sep theory
  // ensures that pto is a function: (pto x y) ^ ~(pto z w) ^ x = z => y != w
  SEP_PTO_NEG_PROP,
  // enforces injectiveness of pto: (pto x y) ^ (pto y w) ^ x = y => y = w
  SEP_PTO_PROP,
  // ---------------------------------- end sep theory

  // ---------------------------------- sets theory
  //-------------------- sets core solver
  SETS_COMPREHENSION,
  SETS_DEQ,
  SETS_DOWN_CLOSURE,
  SETS_EQ_MEM,
  SETS_EQ_MEM_CONFLICT,
  SETS_MEM_EQ,
  SETS_MEM_EQ_CONFLICT,
  SETS_PROXY,
  SETS_PROXY_SINGLETON,
  SETS_SINGLETON_EQ,
  SETS_UP_CLOSURE,
  SETS_UP_CLOSURE_2,
  SETS_UP_UNIV,
  SETS_UNIV_TYPE,
  //-------------------- sets cardinality solver
  // cycle of cardinalities, hence all sets have the same
  SETS_CARD_CYCLE,
  // two sets have the same cardinality
  SETS_CARD_EQUAL,
  SETS_CARD_GRAPH_EMP,
  SETS_CARD_GRAPH_EMP_PARENT,
  SETS_CARD_GRAPH_EQ_PARENT,
  SETS_CARD_GRAPH_EQ_PARENT_2,
  SETS_CARD_GRAPH_PARENT_SINGLETON,
  // cardinality is at least the number of elements we already know
  SETS_CARD_MINIMAL,
  // negative members are part of the universe
  SETS_CARD_NEGATIVE_MEMBER,
  // all sets have non-negative cardinality
  SETS_CARD_POSITIVE,
  // the universe is a superset of every set
  SETS_CARD_UNIV_SUPERSET,
  // cardinality of the universe is at most cardinality of the type
  SETS_CARD_UNIV_TYPE,
  //-------------------- sets relations solver
  SETS_RELS_IDENTITY_DOWN,
  SETS_RELS_IDENTITY_UP,
  SETS_RELS_JOIN_COMPOSE,
  SETS_RELS_JOIN_IMAGE_DOWN,
  SETS_RELS_JOIN_SPLIT_1,
  SETS_RELS_JOIN_SPLIT_2,
  SETS_RELS_PRODUCE_COMPOSE,
  SETS_RELS_PRODUCT_SPLIT,
  SETS_RELS_TCLOSURE_FWD,
  SETS_RELS_TRANSPOSE_EQ,
  SETS_RELS_TRANSPOSE_REV,
  SETS_RELS_TUPLE_REDUCTION,
  //-------------------------------------- end sets theory

  //-------------------------------------- strings theory
  //-------------------- base solver
  // initial normalize singular
  //   x1 = "" ^ ... ^ x_{i-1} = "" ^ x_{i+1} = "" ^ ... ^ xn = "" =>
  //   x1 ++ ... ++ xn = xi
  STRINGS_I_NORM_S,
  // initial constant merge
  //   explain_constant(x, c) => x = c
  // Above, explain_constant(x,c) is a basic explanation of why x must be equal
  // to string constant c, which is computed by taking arguments of
  // concatenation terms that are entailed to be constants. For example:
  //  ( y = "AB" ^ z = "C" ) => y ++ z = "ABC"
  STRINGS_I_CONST_MERGE,
  // initial constant conflict
  //    ( explain_constant(x, c1) ^ explain_constant(x, c2) ^ x = y) => false
  // where c1 != c2.
  STRINGS_I_CONST_CONFLICT,
  // initial normalize
  // Given two concatenation terms, this is applied when we find that they are
  // equal after e.g. removing strings that are currently empty. For example:
  //   y = "" ^ z = "" => x ++ y = z ++ x
  STRINGS_I_NORM,
  // injectivity of seq.unit
  // (seq.unit x) = (seq.unit y) => x=y, or
  // (seq.unit x) = (seq.unit c) => x=c
  STRINGS_UNIT_INJ,
  // unit constant conflict
  // (seq.unit x) = C => false if |C| != 1.
  STRINGS_UNIT_CONST_CONFLICT,
  // injectivity of seq.unit for disequality
  // (seq.unit x) != (seq.unit y) => x != y, or
  // (seq.unit x) != (seq.unit c) => x != c
  STRINGS_UNIT_INJ_DEQ,
  // A split due to cardinality
  STRINGS_CARD_SP,
  // The cardinality inference for strings, see Liang et al CAV 2014.
  STRINGS_CARDINALITY,
  //-------------------- end base solver
  //-------------------- core solver
  // A cycle in the empty string equivalence class, e.g.:
  //   x ++ y = "" => x = ""
  // This is typically not applied due to length constraints implying emptiness.
  STRINGS_I_CYCLE_E,
  // A cycle in the containment ordering.
  //   x = y ++ x => y = "" or
  //   x = y ++ z ^ y = x ++ w => z = "" ^ w = ""
  // This is typically not applied due to length constraints implying emptiness.
  STRINGS_I_CYCLE,
  // Flat form constant
  //   x = y ^ x = z ++ c ... ^ y = z ++ d => false
  // where c and d are distinct constants.
  STRINGS_F_CONST,
  // Flat form unify
  //   x = y ^ x = z ++ x' ... ^ y = z ++ y' ^ len(x') = len(y') => x' = y'
  // Notice flat form instances are similar to normal form inferences but do
  // not involve recursive explanations.
  STRINGS_F_UNIFY,
  // Flat form endpoint empty
  //   x = y ^ x = z ^ y = z ++ y' => y' = ""
  STRINGS_F_ENDPOINT_EMP,
  // Flat form endpoint equal
  //   x = y ^ x = z ++ x' ^ y = z ++ y' => x' = y'
  STRINGS_F_ENDPOINT_EQ,
  // Flat form not contained
  // x = c ^ x = y => false when rewrite( contains( y, c ) ) = false
  STRINGS_F_NCTN,
  // Normal form equality conflict
  //   x = N[x] ^ y = N[y] ^ x=y => false
  // where Rewriter::rewrite(N[x]=N[y]) = false.
  STRINGS_N_EQ_CONF,
  // Given two normal forms, infers that the remainder one of them has to be
  // empty. For example:
  //    If x1 ++ x2 = y1 and x1 = y1, then x2 = ""
  STRINGS_N_ENDPOINT_EMP,
  // Given two normal forms, infers that two components have to be the same if
  // they have the same length. For example:
  //   If x1 ++ x2 = x3 ++ x4 and len(x1) = len(x3) then x1 = x3
  STRINGS_N_UNIFY,
  // Given two normal forms, infers that the endpoints have to be the same. For
  // example:
  //   If x1 ++ x2 = x3 ++ x4 ++ x5 and x1 = x3 then x2 = x4 ++ x5
  STRINGS_N_ENDPOINT_EQ,
  // Given two normal forms with constant endpoints, infers a conflict if the
  // endpoints do not agree. For example:
  //   If "abc" ++ ... = "bc" ++ ... then conflict
  STRINGS_N_CONST,
  // infer empty, for example:
  //     (~) x = ""
  // This is inferred when we encounter an x such that x = "" rewrites to a
  // constant. This inference is used for instance when we otherwise would have
  // split on the emptiness of x but the rewriter tells us the emptiness of x
  // can be inferred.
  STRINGS_INFER_EMP,
  // string split constant propagation, for example:
  //     x = y, x = "abc", y = y1 ++ "b" ++ y2
  //       implies y1 = "a" ++ y1'
  STRINGS_SSPLIT_CST_PROP,
  // string split variable propagation, for example:
  //     x = y, x = x1 ++ x2, y = y1 ++ y2, len( x1 ) >= len( y1 )
  //       implies x1 = y1 ++ x1'
  // This is inspired by Zheng et al CAV 2015.
  STRINGS_SSPLIT_VAR_PROP,
  // length split, for example:
  //     len( x1 ) = len( y1 ) V len( x1 ) != len( y1 )
  // This is inferred when e.g. x = y, x = x1 ++ x2, y = y1 ++ y2.
  STRINGS_LEN_SPLIT,
  // length split empty, for example:
  //     z = "" V z != ""
  // This is inferred when, e.g. x = y, x = z ++ x1, y = y1 ++ z
  STRINGS_LEN_SPLIT_EMP,
  // string split constant
  //    x = y, x = "c" ++ x2, y = y1 ++ y2, y1 != ""
  //      implies y1 = "c" ++ y1'
  // This is a special case of F-Split in Figure 5 of Liang et al CAV 2014.
  STRINGS_SSPLIT_CST,
  // string split variable, for example:
  //    x = y, x = x1 ++ x2, y = y1 ++ y2
  //      implies x1 = y1 ++ x1' V y1 = x1 ++ y1'
  // This is rule F-Split in Figure 5 of Liang et al CAV 2014.
  STRINGS_SSPLIT_VAR,
  // flat form loop, for example:
  //    x = y, x = x1 ++ z, y = z ++ y2
  //      implies z = u2 ++ u1, u in ( u1 ++ u2 )*, x1 = u2 ++ u, y2 = u ++ u1
  //        for fresh u, u1, u2.
  // This is the rule F-Loop from Figure 5 of Liang et al CAV 2014.
  STRINGS_FLOOP,
  // loop conflict ???
  STRINGS_FLOOP_CONFLICT,
  // Normal form inference
  // x = y ^ z = y => x = z
  // This is applied when y is the normal form of both x and z.
  STRINGS_NORMAL_FORM,
  // Normal form not contained, same as FFROM_NCTN but for normal forms
  STRINGS_N_NCTN,
  // Length normalization
  //   x = y => len( x ) = len( y )
  // Typically applied when y is the normal form of x.
  STRINGS_LEN_NORM,
  // When x ++ x' ++ ... != "abc" ++ y' ++ ... ^ len(x) != len(y), we apply the
  // inference:
  //   x = "" v x != ""
  STRINGS_DEQ_DISL_EMP_SPLIT,
  // When x ++ x' ++ ... != "abc" ++ y' ++ ... ^ len(x) = 1, we apply the
  // inference:
  //   x = "a" v x != "a"
  STRINGS_DEQ_DISL_FIRST_CHAR_EQ_SPLIT,
  // When x ++ x' ++ ... != "abc" ++ y' ++ ... ^ len(x) != "", we apply the
  // inference:
  //   ni = x ++ x' ++ ... ^ nj = "abc" ++ y' ++ ... ^ x != "" --->
  //     x = k1 ++ k2 ^ len(k1) = 1 ^ (k1 != "a" v x = "a" ++  k2)
  STRINGS_DEQ_DISL_FIRST_CHAR_STRING_SPLIT,
  // When x ++ x' ++ ... != y ++ y' ++ ... ^ len(x) != len(y), we apply the
  // inference:
  //   ni = x ++ x' ++ ... ^ nj = y ++ y' ++ ... ^ ni != nj ^ len(x) != len(y)
  //     --->
  //       len(k1) = len(x) ^ len(k2) = len(y) ^ (y = k1 ++ k3 v x = k1 ++ k2)
  STRINGS_DEQ_DISL_STRINGS_SPLIT,
  // When x ++ x' ++ ... != y ++ y' ++ ... ^ len(x) = len(y), we apply the
  // inference:
  //   x = y v x != y
  STRINGS_DEQ_STRINGS_EQ,
  // When x ++ x' ++ ... != y ++ y' ++ ... and we do not know how the lengths
  // of x and y compare, we apply the inference:
  //   len(x) = len(y) v len(x) != len(y)
  STRINGS_DEQ_LENS_EQ,
  // When px ++ x ++ ... != py ^ len(px ++ x ++ ...) = len(py), we apply the
  // following inference that infers that the remainder of the longer normal
  // form must be empty:
  //   ni = px ++ x ++ ... ^ nj = py ^ len(ni) = len(nj) --->
  //     x = "" ^ ...
  STRINGS_DEQ_NORM_EMP,
  // When two strings are disequal s != t and the comparison of their lengths
  // is unknown, we apply the inference:
  //   len(s) != len(t) V len(s) = len(t)
  STRINGS_DEQ_LENGTH_SP,
  //-------------------- end core solver
  //-------------------- codes solver
  // str.to_code( v ) = rewrite( str.to_code(c) )
  // where v is the proxy variable for c.
  STRINGS_CODE_PROXY,
  // str.code(x) = -1 V str.code(x) != str.code(y) V x = y
  STRINGS_CODE_INJ,
  //-------------------- end codes solver
  //-------------------- regexp solver
  // regular expression normal form conflict
  //   ( x in R ^ x = y ^ rewrite((str.in_re y R)) = false ) => false
  // where y is the normal form computed for x.
  STRINGS_RE_NF_CONFLICT,
  // regular expression unfolding
  // This is a general class of inferences of the form:
  //   (x in R) => F
  // where F is formula expressing the next step of checking whether x is in
  // R.  For example:
  //   (x in (R)*) =>
  //   x = "" V x in R V ( x = x1 ++ x2 ++ x3 ^ x1 in R ^ x2 in (R)* ^ x3 in R)
  STRINGS_RE_UNFOLD_POS,
  // Same as above, for negative memberships
  STRINGS_RE_UNFOLD_NEG,
  // intersection inclusion conflict
  //   (x in R1 ^ ~ x in R2) => false  where [[includes(R2,R1)]]
  // Where includes(R2,R1) is a heuristic check for whether R2 includes R1.
  STRINGS_RE_INTER_INCLUDE,
  // intersection conflict, using regexp intersection computation
  //   (x in R1 ^ x in R2) => false   where [[intersect(R1, R2) = empty]]
  STRINGS_RE_INTER_CONF,
  // intersection inference
  //   (x in R1 ^ y in R2 ^ x = y) => (x in re.inter(R1,R2))
  STRINGS_RE_INTER_INFER,
  // regular expression delta
  //   (x = "" ^ x in R) => C
  // where "" in R holds if and only if C holds.
  STRINGS_RE_DELTA,
  // regular expression delta conflict
  //   (x = "" ^ x in R) => false
  // where R does not accept the empty string.
  STRINGS_RE_DELTA_CONF,
  // regular expression derive ???
  STRINGS_RE_DERIVE,
  //-------------------- end regexp solver
  //-------------------- extended function solver
  // Standard extended function inferences from context-dependent rewriting
  // produced by constant substitutions. See Reynolds et al CAV 2017. These are
  // inferences of the form:
  //   X = Y => f(X) = t   when   rewrite( f(Y) ) = t
  // where X = Y is a vector of equalities, where some of Y may be constants.
  STRINGS_EXTF,
  // Same as above, for normal form substitutions.
  STRINGS_EXTF_N,
  // Decompositions based on extended function inferences from context-dependent
  // rewriting produced by constant substitutions. This is like the above, but
  // handles cases where the inferred predicate is not necessarily an equality
  // involving f(X). For example:
  //   x = "A" ^ contains( y ++ x, "B" ) => contains( y, "B" )
  // This is generally only inferred if contains( y, "B" ) is a known term in
  // the current context.
  STRINGS_EXTF_D,
  // Same as above, for normal form substitutions.
  STRINGS_EXTF_D_N,
  // Extended function equality rewrite. This is an inference of the form:
  //   t = s => P
  // where P is a predicate implied by rewrite( t = s ).
  // Typically, t is an application of an extended function and s is a constant.
  // It is generally only inferred if P is a predicate over known terms.
  STRINGS_EXTF_EQ_REW,
  // contain transitive
  //   ( str.contains( s, t ) ^ ~contains( s, r ) ) => ~contains( t, r ).
  STRINGS_CTN_TRANS,
  // contain decompose
  //  str.contains( x, str.++( y1, ..., yn ) ) => str.contains( x, yi ) or
  //  ~str.contains( str.++( x1, ..., xn ), y ) => ~str.contains( xi, y )
  STRINGS_CTN_DECOMPOSE,
  // contain neg equal
  //   ( len( x ) = len( s ) ^ ~contains( x, s ) ) => x != s
  STRINGS_CTN_NEG_EQUAL,
  // contain positive
  //   str.contains( x, y ) => x = w1 ++ y ++ w2
  // where w1 and w2 are skolem variables.
  STRINGS_CTN_POS,
  // All reduction inferences of the form:
  //   f(x1, .., xn) = y ^ P(x1, ..., xn, y)
  // where f is an extended function, y is the purification variable for
  // f(x1, .., xn) and P is the reduction predicate for f
  // (see theory_strings_preprocess).
  STRINGS_REDUCTION,
  //-------------------- end extended function solver
  //-------------------- prefix conflict
  // prefix conflict (coarse-grained)
  STRINGS_PREFIX_CONFLICT,
  //-------------------- end prefix conflict
  //-------------------------------------- end strings theory

  //-------------------------------------- uf theory
  // Clause from the uf symmetry breaker
  UF_BREAK_SYMMETRY,
  //-------------------- cardinality extension to UF
  // The inferences below are described in Reynolds' thesis 2013.
  // conflict of the form (card_T n) => (not (distinct t1 ... tn))
  UF_CARD_CLIQUE,
  // conflict of the form (not (card_T1 n1)) ^ ... (not (card_Tk nk)) ^ (card n)
  // where n1 + ... + nk >= n, where (card n) is a combined cardinality
  // constraint.
  UF_CARD_COMBINED,
  // (not (card_T n)) => (distinct t1 ... tn)
  UF_CARD_ENFORCE_NEGATIVE,
  // used to make the index terms in cardinality constraints equal
  UF_CARD_EQUIV,
  // conflict of the form (not (card_T1 n)) ^ (card_T2 m) where the cardinality
  // of T2 can be assumed to be without loss of generality larger than T1 due to
  // monotonicity reasoning (Claessen et al 2011).
  UF_CARD_MONOTONE_COMBINED,
  // conflict of the form (not (card_T n)) ^ (card_T m) where n>m
  UF_CARD_SIMPLE_CONFLICT,
  // equality split requested by cardinality solver
  //  (or (= t1 t2) (not (= t1 t2))
  // to satisfy the cardinality constraints on the type of t1, t2.
  UF_CARD_SPLIT,
  //-------------------- end cardinality extension to UF
  //-------------------- HO extension to UF
  // Encodes an n-ary application as a chain of binary HO_APPLY applications
  //   (= (f t1 ... tn) (@ (@ ... (@ f t1) ...) tn))
  UF_HO_APP_ENCODE,
  // A lemma corresponding to the definition of a skolem k used to convert
  // HO_APPLY terms to APPLY_UF terms. This is of the form:
  //   (forall x1 ... xn) (@ (@ k x1) ... xn) = t
  // where notice that t is a function whose free variables (if any) are
  // x1 ... xn.
  UF_HO_APP_CONV_SKOLEM,
  // Adds an extensionality lemma to witness that disequal functions have
  // different applications
  //   (not (= (f sk1 .. skn) (g sk1 .. skn))
  UF_HO_EXTENSIONALITY,
  //-------------------- model-construction specific part
  // These rules are necessary to ensure that we build models properly. For more
  // details see Section 3.3 of Barbosa et al. CADE'19.
  //
  // Enforces that a regular APPLY_UF term in the model is equal to its HO_APPLY
  // equivalent by adding the equality as a lemma
  //   (= (f t1 ... tn) (@ (@ ... (@ f t1) ...) tn))
  UF_HO_MODEL_APP_ENCODE,
  // Adds an extensionality lemma to witness that disequal functions have
  // different applications
  //   (not (= (f sk1 .. skn) (g sk1 .. skn))
  UF_HO_MODEL_EXTENSIONALITY,
  //-------------------- end model-construction specific part
  //-------------------- end HO extension to UF
  //-------------------------------------- end uf theory

  //-------------------------------------- unknown
  UNKNOWN
};

/**
 * Converts an inference to a string. Note: This function is also used in
 * `safe_print()`. Changing this functions name or signature will result in
 * `safe_print()` printing "<unsupported>" instead of the proper strings for
 * the enum values.
 *
 * @param i The inference
 * @return The name of the inference
 */
const char* toString(InferenceId i);

/**
 * Writes an inference name to a stream.
 *
 * @param out The stream to write to
 * @param i The inference to write to the stream
 * @return The stream
 */
std::ostream& operator<<(std::ostream& out, InferenceId i);

}  // namespace theory
}  // namespace CVC4

#endif /* CVC4__THEORY__INFERENCE_H */
