#include "theory/trigono/theory_trigono.h"

#include <map>

#include "base/map_util.h"
#include "expr/kind.h"
#include "options/quantifiers_options.h"
#include "options/smt_options.h"
#include "smt/logic_exception.h"
#include "theory/decision_manager.h"
#include "theory/quantifiers/term_database.h"
#include "theory/quantifiers/term_util.h"
#include "theory/quantifiers_engine.h"
#include "theory/rewriter.h"
#include "theory/trigono/theory_trigono_rewriter.h"
#include "theory/theory_model.h"
#include "theory/valuation.h"

using namespace std;
using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace trigono {

/** Constructs a new instance of TheoryTrigono w.r.t. the provided contexts. */
TheoryTrigono::TheoryTrigono(context::Context* c,
                           context::UserContext* u,
                           OutputChannel& out,
                           Valuation valuation,
                           const LogicInfo& logicInfo,
                           ProofNodeManager* pnm)
    : Theory(THEORY_TRIGONO, c, u, out, valuation, logicInfo, pnm),
    d_lemmas_produced_c(u),
    d_bounds_init(false),
    d_state(c, u, valuation),
    d_im(*this, d_state, nullptr, "theory::trigono")
{
  d_true = NodeManager::currentNM()->mkConst<bool>(true);
  d_false = NodeManager::currentNM()->mkConst<bool>(false);

  // indicate we are using the default theory state object
  d_theoryState = &d_state;
  d_inferManager = &d_im;
}

TheoryRewriter* TheoryTrigono::getTheoryRewriter() { return &d_rewriter; }

//bool TheoryTrigono::needsEqualityEngine(EeSetupInfo& esi)
//{
//  return d_internal->needsEqualityEngine(esi);
//}

void TheoryTrigono::finishInit()
{
  //Debug("theory") << "TheoryTrigono::finishInit(): processing" << std::endl;
  if (getLogicInfo().isTheoryEnabled(THEORY_ARITH)
      && getLogicInfo().areTranscendentalsUsed())
  {
    d_valuation.setUnevaluatedKind(kind::TRIG_SINE);
    d_valuation.setUnevaluatedKind(kind::TRIG_COSINE);
    d_valuation.setUnevaluatedKind(kind::TRIG_TANGENT);
    d_valuation.setUnevaluatedKind(kind::TRIG_COSECANT);
    d_valuation.setUnevaluatedKind(kind::TRIG_COTANGENT);
    d_valuation.setUnevaluatedKind(kind::TRIG_SECANT);

    d_valuation.setUnevaluatedKind(kind::TRIG_PI);
  }
}

void TheoryTrigono::check(Effort level) {
  Debug("theory") << "TheoryTrigono::check(): processing " << level << std::endl;
  if (done() && !fullEffort(level)) {
    return;
  }

  TimerStat::CodeTimer checkTimer(d_checkTime);

  while(!done()) {
    // Get all the assertions
    Assertion assertion = get();
    TNode fact = assertion.d_assertion;

    Debug("trigono") << "TheoryTrigono::check(): processing " << fact << std::endl;

    // Do the work
    switch(fact.getKind()) {

    /* cases for all the theory's kinds go here... */

    default:
      Trace("Trigono-debug")
          << "Unhandled Kind" << fact.getKind() << std::endl;
    }
  }

}

bool TheoryTrigono::needsCheckLastEffort() {
//  if (d_nonlinearExtension != nullptr)
//  {
//    return d_nonlinearExtension->needsCheckLastEffort();
//  }
  return true;
}

/* TheoryTrigono::check() */

}/* CVC4::theory::trigono namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */
