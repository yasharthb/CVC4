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

void TheoryTrigono::check(Effort level) {
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
/* TheoryTrigono::check() */

}/* CVC4::theory::trigono namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */
