/*********************                                                        */
/*! \file instantiate.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Mathias Preiner, Morgan Deters
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief instantiate
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__QUANTIFIERS__INSTANTIATE_H
#define CVC4__THEORY__QUANTIFIERS__INSTANTIATE_H

#include <map>

#include "context/cdhashset.h"
#include "expr/lazy_proof.h"
#include "expr/node.h"
#include "expr/proof.h"
#include "theory/quantifiers/inst_match_trie.h"
#include "theory/quantifiers/quant_util.h"
#include "util/statistics_registry.h"

namespace CVC4 {
namespace theory {

class QuantifiersEngine;

namespace quantifiers {

class TermDb;
class QuantifiersState;
class QuantifiersInferenceManager;
class QuantifiersRegistry;

/** Instantiation rewriter
 *
 * This class is used for cases where instantiation lemmas can be rewritten by
 * external utilities. Examples of this include virtual term substitution and
 * nested quantifier elimination techniques.
 */
class InstantiationRewriter
{
 public:
  InstantiationRewriter() {}
  virtual ~InstantiationRewriter() {}

  /** rewrite instantiation
   *
   * The node inst is the instantiation of quantified formula q for terms.
   * This method returns the rewritten form of the instantiation.
   *
   * The flag doVts is whether we must apply virtual term substitution to the
   * instantiation.
   *
   * Returns a TrustNode of kind REWRITE, corresponding to the rewrite of inst
   * and its proof generator.
   */
  virtual TrustNode rewriteInstantiation(Node q,
                                         std::vector<Node>& terms,
                                         Node inst,
                                         bool doVts) = 0;
};

/** Instantiate
 *
 * This class is used for generating instantiation lemmas.  It maintains an
 * instantiation trie, which is represented by a different data structure
 * depending on whether incremental solving is enabled (see d_inst_match_trie
 * and d_c_inst_match_trie).
 *
 * Below, we say an instantiation lemma for q = forall x. F under substitution
 * { x -> t } is the formula:
 *   ( ~forall x. F V F * { x -> t } )
 * where * is application of substitution.
 *
 * Its main interface is ::addInstantiation(...), which is called by many of
 * the quantifiers modules, which enqueues instantiation lemmas in quantifiers
 * engine via calls to QuantifiersInferenceManager::addPendingLemma.
 *
 * It also has utilities for constructing instantiations, and interfaces for
 * getting the results of the instantiations produced during check-sat calls.
 */
class Instantiate : public QuantifiersUtil
{
  typedef context::CDHashMap<Node, uint32_t, NodeHashFunction> NodeUIntMap;

 public:
  Instantiate(QuantifiersEngine* qe,
              QuantifiersState& qs,
              QuantifiersInferenceManager& qim,
              QuantifiersRegistry& qr,
              ProofNodeManager* pnm = nullptr);
  ~Instantiate();

  /** reset */
  bool reset(Theory::Effort e) override;
  /** register quantifier */
  void registerQuantifier(Node q) override;
  /** identify */
  std::string identify() const override { return "Instantiate"; }
  /** check incomplete */
  bool checkComplete() override;

  //--------------------------------------rewrite objects
  /** add instantiation rewriter */
  void addRewriter(InstantiationRewriter* ir);
  /** notify flush lemmas
   *
   * This is called just before the quantifiers engine flushes its lemmas to
   * the output channel.
   */
  void notifyFlushLemmas();
  //--------------------------------------end rewrite objects

  /** do instantiation specified by m
   *
   * This function returns true if the instantiation lemma for quantified
   * formula q for the substitution specified by terms is successfully enqueued
   * via a call to QuantifiersInferenceManager::addPendingLemma.
   *   mkRep : whether to take the representatives of the terms in the range of
   *           the substitution m,
   *   modEq : whether to check for duplication modulo equality in instantiation
   *           tries (for performance),
   *   doVts : whether we must apply virtual term substitution to the
   *           instantiation lemma.
   *
   * This call may fail if it can be determined that the instantiation is not
   * relevant or legal in the current context. This happens if:
   * (1) The substitution is not well-typed,
   * (2) The substitution involves terms whose instantiation level is above the
   *     allowed limit,
   * (3) The resulting instantiation is entailed in the current context using a
   *     fast entailment check (see TermDb::isEntailed),
   * (4) The range of the substitution is a duplicate of that of a previously
   *     added instantiation,
   * (5) The instantiation lemma is a duplicate of previously added lemma.
   *
   */
  bool addInstantiation(Node q,
                        std::vector<Node>& terms,
                        bool mkRep = false,
                        bool modEq = false,
                        bool doVts = false);
  /**
   * Same as above, but we also compute a vector failMask indicating which
   * values in terms led to the instantiation not being added when this method
   * returns false.  For example, if q is the formula
   *   forall xy. x>5 => P(x,y)
   * If terms = { 4, 0 }, then this method will return false since
   *   4>5 => P(4,0)
   * is entailed true based on rewriting. This method may additionally set
   * failMask to "10", indicating that x's value was critical, but y's value
   * was not. In other words, all instantiations including { x -> 4 } will also
   * lead to this method returning false.
   *
   * The bits of failMask are computed in a greedy fashion, in reverse order.
   * That is, we check whether each variable is critical one at a time, starting
   * from the end.
   *
   * The parameter expFull is whether try to set all bits of the fail mask to
   * 0. If this argument is true, then we only try to set a suffix of the
   * bits in failMask to false. The motivation for expFull=false is for callers
   * of this method that are enumerating tuples in lexiocographic order. The
   * number of false bits in the suffix of failMask tells the caller how many
   * "decimal" places to increment their iterator.
   */
  bool addInstantiationExpFail(Node q,
                               std::vector<Node>& terms,
                               std::vector<bool>& failMask,
                               bool mkRep = false,
                               bool modEq = false,
                               bool doVts = false,
                               bool expFull = true);
  /** record instantiation
   *
   * Explicitly record that q has been instantiated with terms. This is the
   * same as addInstantiation, but does not enqueue an instantiation lemma.
   */
  bool recordInstantiation(Node q,
                           std::vector<Node>& terms,
                           bool modEq = false,
                           bool addedLem = true);
  /** exists instantiation
   *
   * Returns true if and only if the instantiation already was added or
   * recorded by this class.
   *   modEq : whether to check for duplication modulo equality
   */
  bool existsInstantiation(Node q,
                           std::vector<Node>& terms,
                           bool modEq = false);
  //--------------------------------------general utilities
  /** get instantiation
   *
   * Returns the instantiation lemma for q under substitution { vars -> terms }.
   * doVts is whether to apply virtual term substitution to its body.
   *
   * If provided, pf is a lazy proof for which we store a proof of the
   * returned formula with free assumption q. This typically stores a
   * single INSTANTIATE step concluding the instantiated body of q from q.
   */
  Node getInstantiation(Node q,
                        std::vector<Node>& vars,
                        std::vector<Node>& terms,
                        bool doVts = false,
                        LazyCDProof* pf = nullptr);
  /** get instantiation
   *
   * Same as above but with vars equal to the bound variables of q.
   */
  Node getInstantiation(Node q, std::vector<Node>& terms, bool doVts = false);
  /** get term for type
   *
   * This returns an arbitrary term for type tn.
   * This term is chosen heuristically to be the best
   * term for instantiation. Currently, this
   * heuristic enumerates the first term of the
   * type if the type is closed enumerable, otherwise
   * an existing ground term from the term database if
   * one exists, or otherwise a fresh variable.
   */
  Node getTermForType(TypeNode tn);
  //--------------------------------------end general utilities

  /**
   * Debug print, called once per instantiation round. This prints
   * instantiations added this round to trace inst-per-quant-round, if
   * applicable, and prints to out if the option debug-inst is enabled.
   */
  void debugPrint(std::ostream& out);
  /** debug print model, called once, before we terminate with sat/unknown. */
  void debugPrintModel();

  //--------------------------------------user-level interface utilities
  /** get instantiated quantified formulas
   *
   * Get the list of quantified formulas that were instantiated in the current
   * user context, store them in qs.
   */
  void getInstantiatedQuantifiedFormulas(std::vector<Node>& qs);
  /** get instantiation term vectors
   *
   * Get term vectors corresponding to for all instantiations lemmas added in
   * the current user context for quantified formula q, store them in tvecs.
   */
  void getInstantiationTermVectors(Node q,
                                   std::vector<std::vector<Node> >& tvecs);
  /** get instantiation term vectors
   *
   * Get term vectors for all instantiations lemmas added in the current user
   * context for quantified formula q, store them in tvecs.
   */
  void getInstantiationTermVectors(
      std::map<Node, std::vector<std::vector<Node> > >& insts);
  //--------------------------------------end user-level interface utilities

  /** Are proofs enabled for this object? */
  bool isProofEnabled() const;

  /** statistics class
   *
   * This tracks statistics on the number of instantiations successfully
   * enqueued on the quantifiers output channel, and the number of redundant
   * instantiations encountered by various criteria.
   */
  class Statistics
  {
   public:
    IntStat d_instantiations;
    IntStat d_inst_duplicate;
    IntStat d_inst_duplicate_eq;
    IntStat d_inst_duplicate_ent;
    Statistics();
    ~Statistics();
  }; /* class Instantiate::Statistics */
  Statistics d_statistics;

 private:
  /** record instantiation, return true if it was not a duplicate
   *
   * addedLem : whether an instantiation lemma was added for the vector we are
   *            recording. If this is false, we bookkeep the vector.
   * modEq : whether to check for duplication modulo equality in instantiation
   *         tries (for performance),
   */
  bool recordInstantiationInternal(Node q,
                                   std::vector<Node>& terms,
                                   bool modEq = false,
                                   bool addedLem = true);
  /** remove instantiation from the cache */
  bool removeInstantiationInternal(Node q, std::vector<Node>& terms);
  /**
   * Ensure that n has type tn, return a term equivalent to it for that type
   * if possible.
   */
  static Node ensureType(Node n, TypeNode tn);

  /** pointer to the quantifiers engine */
  QuantifiersEngine* d_qe;
  /** Reference to the quantifiers state */
  QuantifiersState& d_qstate;
  /** Reference to the quantifiers inference manager */
  QuantifiersInferenceManager& d_qim;
  /** The quantifiers registry */
  QuantifiersRegistry& d_qreg;
  /** pointer to the proof node manager */
  ProofNodeManager* d_pnm;
  /** cache of term database for quantifiers engine */
  TermDb* d_term_db;
  /** instantiation rewriter classes */
  std::vector<InstantiationRewriter*> d_instRewrite;

  /** statistics for debugging total instantiations per quantifier */
  NodeUIntMap d_total_inst_debug;
  /** statistics for debugging total instantiations per quantifier per round */
  std::map<Node, uint32_t> d_temp_inst_debug;

  /** list of all instantiations produced for each quantifier
   *
   * We store context (dependent, independent) versions. If incremental solving
   * is disabled, we use d_inst_match_trie for performance reasons.
   */
  std::map<Node, inst::InstMatchTrie> d_inst_match_trie;
  std::map<Node, inst::CDInstMatchTrie*> d_c_inst_match_trie;
  /**
   * The list of quantified formulas for which the domain of d_c_inst_match_trie
   * is valid.
   */
  context::CDHashSet<Node, NodeHashFunction> d_c_inst_match_trie_dom;

  /** explicitly recorded instantiations
   *
   * Sometimes an instantiation is recorded internally but not sent out as a
   * lemma, for instance, for partial quantifier elimination. This is a map
   * of these instantiations, for each quantified formula.
   */
  std::vector<std::pair<Node, std::vector<Node> > > d_recorded_inst;
  /**
   * A CDProof storing instantiation steps.
   */
  std::unique_ptr<CDProof> d_pfInst;
};

} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */

#endif /* CVC4__THEORY__QUANTIFIERS__INSTANTIATE_H */
