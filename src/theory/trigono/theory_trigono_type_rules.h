#include "cvc4_private.h"

#ifndef __CVC4__THEORY__TRIGONO__THEORY_TRIGONO_TYPE_RULES_H
#define __CVC4__THEORY__TRIGONO__THEORY_TRIGONO_TYPE_RULES_H

namespace CVC4 {
namespace theory {
namespace trigono {

class TrigonoTypeRule {
public:

  /**
   * Compute the type for (and optionally typecheck) a term belonging
   * to the theory of trigono.
   *
   * @param check if true, the node's type should be checked as well
   * as computed.
   */
  inline static TypeNode computeType(NodeManager* nodeManager, TNode n,
                                     bool check){
    // TODO: implement me!
    Trace("Trigono-TypeRule-debug")
        << "Hit computeType " << n << std::endl;
    return nodeManager->booleanType();
    //Unimplemented();

  }

};/* class TrigonoTypeRule */

}/* CVC4::theory::trigono namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* __CVC4__THEORY__TRIGONO__THEORY_TRIGONO_TYPE_RULES_H */
