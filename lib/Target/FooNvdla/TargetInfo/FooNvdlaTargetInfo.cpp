//===- FooNvdlaTargetInfo.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "FooNvdlaTargetInfo.h"
#include <onnc/IR/Quadruple.h>
#include <onnc/Target/TargetRegistry.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//

  
static unsigned int FooNvdlaMatchFn(const Quadruple& pQuadruple)
{
  unsigned int score = 0;
  if (Quadruple::foonvdla == pQuadruple.getArch()) {
    score += 10;
  }

  return score;
}


Target& onnc::getTheFooNvdlaTarget()
{
  static Target foonvdla_target;
  return foonvdla_target;
}


extern "C" void InitializeFooNvdlaONNCPlatform() {
  onnc::TargetRegistry::RegisterTarget(onnc::getTheFooNvdlaTarget(), "foonvdla",
                                       "FooNvdla DLA", FooNvdlaMatchFn);
}
