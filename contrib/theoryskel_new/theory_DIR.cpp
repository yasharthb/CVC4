#include "theory/$dir/theory_$dir.h"
#include "theory/$dir/theory_$dir_rewriter.h"

#include "expr/kind.h"
#include "options/smt_options.h"
#include "smt/logic_exception.h"
#include "theory/decision_manager.h"
#include "theory/rewriter.h"
#include "theory/theory_model.h"

using namespace std;
using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace $dir {

/** Constructs a new instance of Theory$camel w.r.t. the provided contexts. */
Theory$camel::Theory$camel(context::Context* c,
                           context::UserContext* u,
                           OutputChannel& out,
                           Valuation valuation,
                           const LogicInfo& logicInfo,
                           ProofNodeManager* pnm)
    : Theory(THEORY_$id, c, u, out, valuation, logicInfo, pnm),
      d_state(c, u, valuation),
      d_im(*this, d_state, nullptr, "theory::$dir")
{
    // indicate we are using the default theory state object
    d_theoryState = &d_state;
    d_inferManager = &d_im;
}/* Theory$camel::Theory$camel() */

void Theory$camel::finishInit()
{
  Debug("theory") << "Theory$camel::finishInit(): processing" << std::endl;
  // Code for Init Sequence
}

void Theory$camel::check(Effort level) {
  if (done() && !fullEffort(level)) {
    return;
  }

  TimerStat::CodeTimer checkTimer(d_checkTime);

  while(!done()) {
    // Get all the assertions
    Assertion assertion = get();
    TNode fact = assertion.d_assertion;

    Debug("$dir") << "Theory$camel::check(): processing " << fact << std::endl;

    // Do the work
    switch(fact.getKind()) {

    /* cases for all the theory's kinds go here... */

    default:
      Debug("$dir") << "Theory$camel::check(): Unhandled Kind"
                    << fact.getKind() << std::endl;fact.getKind());
    }
  }

}/* Theory$camel::check() */

}/* CVC4::theory::$dir namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */
