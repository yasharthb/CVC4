#include "cvc4_private.h"

#ifndef __CVC4__THEORY__TRIGONO__THEORY_TRIGONO_H
#define __CVC4__THEORY__TRIGONO__THEORY_TRIGONO_H

#include "theory/theory.h"

namespace CVC4 {
namespace theory {
namespace trigono {

class TheoryTrigono : public Theory {
public:

  /** Constructs a new instance of TheoryTrigono w.r.t. the provided contexts. */
  TheoryTrigono(context::Context* c,
               context::UserContext* u,
               OutputChannel& out,
               Valuation valuation,
               const LogicInfo& logicInfo);

  void check(Effort);

  std::string identify() const {
    return "THEORY_TRIGONO";
  }

};/* class TheoryTrigono */

}/* CVC4::theory::trigono namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* __CVC4__THEORY__TRIGONO__THEORY_TRIGONO_H */
