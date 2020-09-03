//===- UpsampleAsConvTransposePass.h --------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_FOONVDLA_UPSAMPLE_AS_CONVTRANSPOSE_H
#define ONNC_FOONVDLA_UPSAMPLE_AS_CONVTRANSPOSE_H
#include <onnc/Core/CustomPass.h>

namespace onnc {
namespace foonvdla {

class UpsampleAsConvTransposePass : 
  public CustomPass<UpsampleAsConvTransposePass>
{
public:
  UpsampleAsConvTransposePass() = default;

  ReturnType runOnModule(Module& pModule) override;
  ReturnType runOnComputeGraph(ComputeGraph& pCG) override;

private:
  // recognize Upsample with mode='nearest', scale = 2x2
  bool matchPattern(ComputeOperator* up);  
};

} // namespace foonvdla
} // namespace onnc

#endif