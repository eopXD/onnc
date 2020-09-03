//===- TemplatePass.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "TemplatePass.h"

#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/Transforms/Optimizations/OptimizationsUtils.h>

namespace onnc {
namespace foonvdla {

//===----------------------------------------------------------------------===//
// TemplatePass
//===----------------------------------------------------------------------===//

Pass::ReturnType TemplatePass::runOnModule(Module& pModule)
{
  const Pass::ReturnType ret = BaseType::runOnModule(pModule);

  if (ret != kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

Pass::ReturnType TemplatePass::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;

  return ret;
}

} // namespace foonvdla
} // namespace onnc
