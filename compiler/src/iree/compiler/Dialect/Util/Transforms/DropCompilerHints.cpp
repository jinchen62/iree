// Copyright 2019 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <utility>

#include "iree/compiler/Dialect/Util/IR/UtilOps.h"
#include "iree/compiler/Dialect/Util/Transforms/Passes.h"
#include "mlir/Pass/Pass.h"

namespace mlir::iree_compiler::IREE::Util {

#define GEN_PASS_DEF_DROPCOMPILERHINTSPASS
#include "iree/compiler/Dialect/Util/Transforms/Passes.h.inc"

namespace {

class DropCompilerHintsPass
    : public impl::DropCompilerHintsPassBase<DropCompilerHintsPass> {
public:
  void runOnOperation() override {
    // We can't use patterns and applyPatternsGreedily because that
    // automatically does canonicalization.
    getOperation()->walk([&](Operation *genericOp) {
      if (auto op = dyn_cast<IREE::Util::OptimizationBarrierOp>(genericOp)) {
        op.replaceAllUsesWith(op.getOperands());
        op.erase();
      } else if (auto op = dyn_cast<IREE::Util::AssumeIntOp>(genericOp)) {
        op.replaceAllUsesWith(op.getOperands());
        op.erase();
      }
    });
  }
};

} // namespace

} // namespace mlir::iree_compiler::IREE::Util
