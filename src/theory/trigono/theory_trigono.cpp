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
#include "theory/theory_model.h"
#include "theory/trigono/theory_trigono_rewriter.h"
#include "theory/valuation.h"
#include "math.h"

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
      //    d_notify(*this),
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

bool TheoryTrigono::needsEqualityEngine(EeSetupInfo& esi)
{
  // esi.d_notify = &d_inferManager;
  esi.d_name = "theory::trigono::ee";
  return true;
}

void TheoryTrigono::finishInit()
{
  Debug("theory") << "TheoryTrigono::finishInit(): processing" << std::endl;
  if (getLogicInfo().isTheoryEnabled(THEORY_ARITH)
      && getLogicInfo().areTranscendentalsUsed())
  {
    d_valuation.setUnevaluatedKind(TRIG_SINE);
    d_valuation.setUnevaluatedKind(TRIG_COSINE);
    d_valuation.setUnevaluatedKind(TRIG_TANGENT);
    d_valuation.setUnevaluatedKind(TRIG_COSECANT);
    d_valuation.setUnevaluatedKind(TRIG_COTANGENT);
    d_valuation.setUnevaluatedKind(TRIG_SECANT);

    d_valuation.setUnevaluatedKind(TRIG_PI);


    d_equalityEngine->addFunctionKind(TRIG_SINE);
    d_equalityEngine->addFunctionKind(TRIG_COSINE);

  }
}

/*
void TheoryTrigono::check(Effort level)
{
  Debug("theory") << "TheoryTrigono::check(): processing " << level
                  << std::endl;
  if (done() && !fullEffort(level))
  {
    return;
  }

  TimerStat::CodeTimer checkTimer(d_checkTime);

  while (!done())
  {
    // Get all the assertions
    Assertion assertion = get();
    TNode fact = assertion.d_assertion;

    Debug("trigono") << "TheoryTrigono::check(): processing " << fact
                     << std::endl;

    // Do the work
    switch (fact.getKind())
    {
        // cases for all the theory's kinds go here...

      default:
        Trace("Trigono-debug")
            << "Unhandled Kind" << fact.getKind() << std::endl;
    }
  }
}
*/

bool TheoryTrigono::needsCheckLastEffort()
{
  //  if (d_nonlinearExtension != nullptr)
  //  {
  //    return d_nonlinearExtension->needsCheckLastEffort();
  //  }
  return true;
}

bool TheoryTrigono::preNotifyFact(
    TNode atom, bool polarity, TNode fact, bool isPrereg, bool isInternal)
{
  Trace("theory-check") << "Inside preNotifyFact" <<std::endl;
  if (!isInternal && !isPrereg)
  {
    if (atom.getKind() == kind::EQUAL)
    {
      if (!d_equalityEngine->hasTerm(atom[0]))
      {
        Assert(atom[0].isConst());
        d_equalityEngine->addTerm(atom[0]);
      }
      if (!d_equalityEngine->hasTerm(atom[1]))
      {
        Assert(atom[1].isConst());
        d_equalityEngine->addTerm(atom[1]);
      }
    }
  }
  return false;
}

void TheoryTrigono::notifyFact(TNode atom, bool pol, TNode fact, bool isInternal)
{
  if (!d_state.isInConflict() && atom.getKind() == kind::EQUAL)
  {
    Trace("theory-check") << "Inside NotifyFact " <<std::endl;
    NodeManager* nm = NodeManager::currentNM();

    switch(atom[1].getKind()){
      case kind::TRIG_SINE:
        Trace("theory-check") << "Tnode atom = " << atom.getKind() << atom << atom[0].getKind() << atom[0].getConst<Rational>() << atom[1].getKind() << atom[1] <<std::endl;
        if(atom[0].getConst<Rational>().getDouble() <= 1 && atom[0].getConst<Rational>().getDouble() >= -1)
        {
         // TNode v = nm->mkNode(EQUAL, atom[1], atom[0]);
         // d_inferManager->assertInternalFact(v,pol,InferenceId::TRIGONO_EQ,v);
          Trace("theory-check")
              << asin(atom[0].getConst<Rational>().getDouble()) << std::endl;
        }
        else
        {
          Trace("theory-check") << "Raising a conflict " << theoryOf(atom) << std::endl;
          d_inferManager->conflict(atom,
                                   InferenceId::TRIGONO_OUT_OF_BOUND);
        }
        break;
      case kind::TRIG_COSINE:
        Trace("theory-check") << "Tnode atom = " << atom.getKind() << atom << atom[0].getKind() << atom[0].getConst<Rational>() << atom[1].getKind() << atom[1] <<std::endl;
        if(atom[0].getConst<Rational>().getDouble() <= 1 && atom[0].getConst<Rational>().getDouble() >= -1)
        {
         // TNode v = nm->mkNode(EQUAL, atom[1], atom[0]);
         // d_inferManager->assertInternalFact(v,pol,InferenceId::TRIGONO_EQ,v);
          Trace("theory-check")
             << acos(atom[0].getConst<Rational>().getDouble()) << std::endl;
        }
        else
        {
          Trace("theory-check") << "Raising a conflict " << theoryOf(atom) << std::endl;
          d_inferManager->conflict(atom,
                                   InferenceId::TRIGONO_OUT_OF_BOUND);
        }
        break;
      default:
        Trace("theory-check") << "Tnode atom = " << atom.getKind() << atom << atom[0].getKind() << atom[0] << atom[1].getKind() << atom[1] <<std::endl;;
    }
  }
}



/* TheoryTrigono::check() */

}  // namespace trigono
}  // namespace theory
}  // namespace CVC4
