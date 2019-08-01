#ifndef TRITON_SNAPSHOTENGINE_H
#define TRITON_SNAPSHOTENGINE_H

#include <triton/architecture.hpp>
#include <triton/astContext.hpp>
#include <triton/callbacks.hpp>
#include <triton/modes.hpp>
#include <triton/solverEngine.hpp>
#include <triton/symbolicEngine.hpp>
#include <triton/taintEngine.hpp>

namespace triton {
namespace engines {
namespace snapshot {

class SnapshotEngine {
 private:
  // Handling triton::arch::Architecture snapshot
  triton::arch::architecture_e arch =
      triton::arch::architecture_e::ARCH_INVALID;
  std::shared_ptr<triton::arch::CpuInterface> cpu = nullptr;
  // Handling triton::engines::symbolic::SymbolicEngine snapshot
  std::shared_ptr<triton::engines::symbolic::SymbolicEngine> symbolic = nullptr;
  // Handling triton::engines::taint::TaintEngine snapshot
  std::shared_ptr<triton::engines::taint::TaintEngine> taint = nullptr;
  // Handling triton::ast::AstContext snapshot
  std::shared_ptr<triton::ast::AstContext> astContext = nullptr;
  // Handling triton::modes::Modes snapshot
  triton::modes::Modes modes;

 public:
  // Standard constructor
  SnapshotEngine(triton::API& api);
  // Copy constructor
  SnapshotEngine(const SnapshotEngine& other);
  // Restore triton::arch::Architecture
  std::shared_ptr<triton::arch::CpuInterface> getCpuInstance() const;
  triton::arch::architecture_e getArchEnum() const;
  // Restore triton::engines::symbolic::SymbolicEngine
  triton::engines::symbolic::SymbolicEngine& getSymbolicEngine() const;
  // Restore triton::engines::taint::TaintEngine
  triton::engines::taint::TaintEngine& getTaintEngine() const;
  // Restore triton::ast::AstContext
  triton::ast::AstContext getAstContext() const;
  // Restore triton::modes::Modes
  triton::modes::Modes getModes() const;
};

};  // namespace snapshot
};  // namespace engines
};  // namespace triton

#endif