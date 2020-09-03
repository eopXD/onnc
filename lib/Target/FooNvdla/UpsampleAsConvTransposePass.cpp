//===- UpsampleAsConvTransposePass.cpp ------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "UpsampleAsConvTransposePass.h"

#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/Upsample.h>
#include <onnc/IR/Compute/ConvTranspose.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/Transforms/Optimizations/OptimizationsUtils.h>


namespace onnc {
namespace foonvdla {

//unsigned UpsampleAsConvTransposePass::tensorIdx = 0;

Pass::ReturnType UpsampleAsConvTransposePass::runOnModule(Module& pModule)
{
  Pass::ReturnType ret = kModuleNoChanged;

  ret = BaseType::runOnModule(pModule);

  if (ret != kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

Pass::ReturnType UpsampleAsConvTransposePass::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;
  
  // -----
  // search for Upsample ComputeOperator
  // -----
  std::vector<ComputeOperator*> upsampleList;
  for (ComputeOperator& node : pCG) {
  	if (isa<Upsample>(node)) {
      std::cout << "== found Upsample node ===\n";
      node.printAttributes(std::cout);
      std::cout << "\n";
  	}
    if (isa<Upsample>(node) and matchPattern(&node)) {
      upsampleList.emplace_back(&node);
      ret |= Pass::kModuleChanged;
    }
  }
  // -----
  // perform replacement, upsample -> convtrans
  // -----
  for (ComputeOperator* node: upsampleList) {
    std::cout << "== envoke replacement ===\n";
    Upsample* up = dyn_cast<Upsample>(node);
    //int rank = up->getScales().size();

    // (input, output) = ('x', 'y')
    FloatTensor* x = dynamic_cast<FloatTensor*>(up->getX());
    FloatTensor* y = dynamic_cast<FloatTensor*>(up->getY());
    up->removeAllInputs();
    up->removeAllOutputs();
    pCG.erase(*up);

    ConvTranspose* ct = pCG.addOperator<ConvTranspose>();
    //ct->setAutoPad("NOTSET");
    //ct->setDilations();
    //ct->setOutputPadding();
    //ct->setOutputShape();
    ct->setKernelShape({2, 2});
    ct->setPads({0, 0});
    ct->setStrides({2, 2});
    ct->addInput(*x);
    ct->addOutput(*y);
    ct->printAttributes(std::cout);
    std::cout << "\n";
  }

  pCG.topologicalSort();
  return ret;
}

bool UpsampleAsConvTransposePass::matchPattern (ComputeOperator* node)
{
  Upsample* up = dyn_cast<Upsample>(node);
  const FloatsAttr& scale = up->getScales();
  
  if (up->getMode().value() != StringAttr::ValueType("nearest")) {
    return 0;
  }
  
  int i, rank = scale.vector().size();
  //std::cout << "rank: " << rank << "\n";
  if ( rank <= 2 ) {
    return 0;
  }
  //for (i=0; i<rank; ++i ) { std::cout << i << ": " << scale.at(i) << "\n"; }
  for (i=0; i<rank-2; ++i ) {
    if (scale.at(i) != 1.0) {
      return 0;
    }
  }
  for (; i<rank; ++i ) {
    if (scale.at(i) != 2.0) {
      return 0;
    }
  }
  return 1;
}

} // namespace foonvdla
} // namespace onnc