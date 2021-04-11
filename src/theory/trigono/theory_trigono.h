#include "cvc4_private.h"

#ifndef __CVC4__THEORY__TRIGONO__THEORY_TRIGONO_H
#define __CVC4__THEORY__TRIGONO__THEORY_TRIGONO_H

#include "context/cdhashmap.h"
#include "context/cdhashset.h"
#include "context/cdlist.h"
#include "context/cdqueue.h"
#include "theory/theory.h"
#include "theory/theory_state.h"
#include "theory/decision_strategy.h"
#include "theory/trigono/theory_trigono_rewriter.h"
#include "theory/inference_manager_buffered.h"
#include "util/statistics_registry.h"


namespace CVC4 {
namespace theory {

class TheoryModel;

namespace trigono {

class TheoryTrigono : public Theory {
  typedef context::CDList<Node> NodeList;
  typedef context::CDHashSet<Node, NodeHashFunction> NodeSet;
  typedef context::CDHashMap<Node, Node, NodeHashFunction> NodeNodeMap;

 private:
  /** all lemmas sent */
  NodeSet d_lemmas_produced_c;

  /** True node for predicates = true */
  Node d_true;

  /** True node for predicates = false */
  Node d_false;

  //whether bounds have been initialized
  bool d_bounds_init;

  TheoryTrigonoRewriter d_rewriter;
  /** A (default) theory state object */

  TheoryState d_state;
  /** A buffered inference manager */

  /** A buffered inference manager */
  InferenceManagerBuffered d_im;
 public:

  /** Constructs a new instance of TheoryTrigono w.r.t. the provided contexts. */
  TheoryTrigono(context::Context* c,
               context::UserContext* u,
               OutputChannel& out,
               Valuation valuation,
               const LogicInfo& logicInfo,
               ProofNodeManager* pnm = nullptr);

  void check(Effort);
  /** get the official theory rewriter of this theory */
  TheoryRewriter* getTheoryRewriter() override;

 /**
 * Returns true if this theory needs an equality engine, which is assigned
 * to it (d_equalityEngine) by the equality engine manager during
 * TheoryEngine::finishInit, prior to calling finishInit for this theory.
 * If this method returns true, it stores instructions for the notifications
 * this Theory wishes to receive from its equality engine.
 */
 // bool needsEqualityEngine(EeSetupInfo& esi) override;
  /** finish initialization */
  void finishInit() override;

  std::string identify() const override { return std::string("THEORY_TRIGONO"); }


};/* class TheoryTrigono */

}/* CVC4::theory::trigono namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* __CVC4__THEORY__TRIGONO__THEORY_TRIGONO_H */
