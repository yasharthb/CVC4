/*********************                                                        */
/*! \file sat_proof_manager.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Haniel Barbosa
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The proof manager for Minisat
 **/

#include "cvc4_private.h"

#ifndef CVC4__SAT_PROOF_MANAGER_H
#define CVC4__SAT_PROOF_MANAGER_H

#include "context/cdhashset.h"
#include "expr/buffered_proof_generator.h"
#include "expr/expr.h"
#include "expr/lazy_proof_chain.h"
#include "expr/node.h"
#include "expr/proof.h"
#include "expr/proof_node_manager.h"
#include "prop/minisat/core/SolverTypes.h"
#include "prop/sat_solver_types.h"

namespace Minisat {
class Solver;
}

namespace CVC4 {

namespace prop {

/**
 * This class is responsible for managing the proof output of SmtEngine, as
 * well as setting up the global proof checker and proof node manager.
 */
class SatProofManager
{
 public:
  SatProofManager(Minisat::Solver* solver,
                  TheoryProxy* proxy,
                  context::UserContext* userContext,
                  ProofNodeManager* pnm);

  void startResChain(const Minisat::Clause& start);
  // resolution with unit clause ~lit, to be justified
  //
  // @param redundant whether lit is redundant, in which case it will be handled
  // differently
  void addResolutionStep(Minisat::Lit lit, bool redundant = false);
  // resolution with clause using lit as pivot. Sign determines whether it's
  // being removed positively from the given clause or the implicit one it's
  // being resolved against
  void addResolutionStep(const Minisat::Clause& clause, Minisat::Lit lit);
  void endResChain(Minisat::Lit lit);
  void endResChain(const Minisat::Clause& clause);

  /**
   * The justification of a literal is built according to its *current*
   * justification in the SAT solver. Thus the proof of the node corresponding
   * to lit is *always* overwritten *unless* that'd result in a cyclic
   * proof. The SAT solver can produce cyclic proofs in instances where literals
   * and clauses coincide, for example
   *
   *  [proof]
   *
   * is cyclic at the node level but not at the SAT solver level. We avoid
   * cyclic proofs by computing, for the justification of a given literal, the
   * necessary assumptions. If the node is in the assumptions, we do not add the
   * resolution step.
   */
  void tryJustifyingLit(
      prop::SatLiteral lit,
      std::unordered_set<TNode, TNodeHashFunction>& assumptions);

  void tryJustifyingLit(prop::SatLiteral lit);

  void finalizeProof(Node inConflictNode,
                     const std::vector<SatLiteral>& inConflict);
  /**
   * @param adding whethen the conflict is coming from a freshly added clause
   */
  void finalizeProof(const Minisat::Clause& inConflict, bool adding = false);
  void finalizeProof(Minisat::Lit inConflict, bool adding = false);
  void finalizeProof();
  void storeUnitConflict(Minisat::Lit inConflict);

  CDProof* getProof();

  void registerSatLitAssumption(Minisat::Lit lit);
  void registerSatAssumptions(const std::vector<Node>& assumps);

 private:
  void endResChain(Node conclusion, const std::set<SatLiteral>& conclusionLits);

  // If the redundant literal has a reason, we add that as the resolution step,
  // with the redundant literal as resolvent, otherwise it's a step with the
  // negation of the redundant literal as the res step clause.
  //
  // Moreover, if the reason contains literals that do not show up in the
  // conclusion of the resolution chain, they count as redundant literals as
  // well mark literal as redundant
  void processRedundantLit(SatLiteral lit,
                           const std::set<SatLiteral>& conclusionLits,
                           std::set<SatLiteral>& visited,
                           unsigned pos);

  /** The sat solver to which we are managing proofs */
  Minisat::Solver* d_solver;
  /** A pointer to theory proxy */
  TheoryProxy* d_proxy;

  /** The proof node manager */
  ProofNodeManager* d_pnm;

  /** Resolution steps (links) accumulator for chain resolution. Each pair has a
   * clause and the pivot for the resolution step it is involved on. */
  std::vector<std::pair<Node, Node>> d_resLinks;

  /** redundant literals removed from the resolution chain conclusion */
  std::vector<SatLiteral> d_redundantLits;

  /**
   * Associates clauses to their local proofs. These proofs are local and
   * possibly updated during solving. When the final conclusion is reached, a
   * final proof is built based on the proofs saved here.
   */
  LazyCDProofChain d_resChains;

  /** The proof generator for resolution chains */
  BufferedProofGenerator d_resChainPg;

  /** The resolution proof of false */
  CDProof d_proof;

  /** The false node */
  Node d_false;

  /** All clauses added to the SAT solver, kept in a context-dependend manner.
   */
  context::CDHashSet<Node, NodeHashFunction> d_assumptions;

  /**
   * A placeholder that may be used to store the literal with the final
   * conflict
   */
  SatLiteral d_conflictLit;

  Node getClauseNode(SatLiteral satLit);
  Node getClauseNode(const Minisat::Clause& clause);
  void printClause(const Minisat::Clause& clause);
}; /* class SatProofManager */

}  // namespace prop
}  // namespace CVC4

#endif /* CVC4__SAT_PROOF_MANAGER_H */