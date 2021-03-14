#include "theory/trigono/theory_trigono.h"

using namespace std;

namespace CVC4 {
namespace theory {
namespace trigono {

/** Constructs a new instance of TheoryTrigono w.r.t. the provided contexts. */
TheoryTrigono::TheoryTrigono(context::Context* c,
                           context::UserContext* u,
                           OutputChannel& out,
                           Valuation valuation,
                           const LogicInfo& logicInfo) :
    Theory(THEORY_TRIGONO, c, u, out, valuation, logicInfo) {
}/* TheoryTrigono::TheoryTrigono() */

void TheoryTrigono::check(Effort level) {
  if (done() && !fullEffort(level)) {
    return;
  }

  TimerStat::CodeTimer checkTimer(d_checkTime);

  while(!done()) {
    // Get all the assertions
    Assertion assertion = get();
    TNode fact = assertion.assertion;

    Debug("trigono") << "TheoryTrigono::check(): processing " << fact << std::endl;

    // Do the work
    switch(fact.getKind()) {

    /* cases for all the theory's kinds go here... */

    default:
      Unhandled(fact.getKind());
    }
  }

}/* TheoryTrigono::check() */

}/* CVC4::theory::trigono namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */
