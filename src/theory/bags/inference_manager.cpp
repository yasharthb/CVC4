/*********************                                                        */
/*! \file inference_manager.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Mudathir Mohamed, Morgan Deters, Dejan Jovanovic
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of the inference manager for the theory of bags
 **/

#include "theory/bags/inference_manager.h"

using namespace std;
using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace bags {

InferenceManager::InferenceManager(Theory& t,
                                   SolverState& s,
                                   ProofNodeManager* pnm)
    : InferenceManagerBuffered(t, s, pnm, "theory::bags"), d_state(s)
{
  d_true = NodeManager::currentNM()->mkConst(true);
  d_false = NodeManager::currentNM()->mkConst(false);
}

void InferenceManager::doPending()
{
  doPendingFacts();
  if (d_state.isInConflict())
  {
    // just clear the pending vectors, nothing else to do
    clearPendingLemmas();
    clearPendingPhaseRequirements();
    return;
  }
  doPendingLemmas();
  doPendingPhaseRequirements();
}

}  // namespace bags
}  // namespace theory
}  // namespace CVC4
