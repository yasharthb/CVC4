#include "cvc4_private.h"

#ifndef __CVC4__THEORY__$id__THEORY_$id_H
#define __CVC4__THEORY__$id__THEORY_$id_H

#include "theory/$dir/theory_$dir_rewriter.h"

#include "theory/theory.h"
#include "theory/theory_state.h"
#include "theory/decision_strategy.h"
#include "theory/inference_manager_buffered.h"

class TheoryModel;

namespace CVC4 {
namespace theory {
namespace $dir {

class Theory$camel : public Theory {
 private:

  /** A theory rewriter object */
  Theory$camelRewriter d_rewriter;

  /** A (default) theory state object */
  TheoryState d_state;

  /** A buffered inference manager */
  InferenceManagerBuffered d_im;
 public:

    /** Constructs a new instance of Theory$camel w.r.t. the provided contexts. */
    Theory$camel(context::Context* c,
                 context::UserContext* u,
                 OutputChannel& out,
                 Valuation valuation,
                 const LogicInfo& logicInfo,
                 ProofNodeManager* pnm = nullptr);

    void check(Effort);

    /** get the official theory rewriter of this theory */
    TheoryRewriter* getTheoryRewriter() override;

    /** finish initialization */
    void finishInit() override;

    std::string identify() const {
      return std::string ("THEORY_$id");
  }

};/* class Theory$camel */

}/* CVC4::theory::$dir namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* __CVC4__THEORY__$id__THEORY_$id_H */
