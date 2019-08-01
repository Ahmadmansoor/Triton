#include <triton/aarch64Cpu.hpp>
#include <triton/api.hpp>
#include <triton/architecture.hpp>
#include <triton/exceptions.hpp>
#include <triton/snapshotEngine.hpp>
#include <triton/x8664Cpu.hpp>
#include <triton/x86Cpu.hpp>

namespace triton {
namespace engines {
namespace snapshot {

SnapshotEngine::SnapshotEngine(triton::API& api) {
  // Get the triton::arch::Architecture reference
  triton::arch::Architecture& architecture = api.snapshotArchitecture();
  // Get the architecture type (copy by 'value', it's an enumeration)
  this->arch = architecture.getArchitecture();
  // Saving the inner CPU state (memory map and registers)
  triton::arch::CpuInterface* cpui = architecture.getCpuInstance();
  switch (this->arch) {
    case triton::arch::architecture_e::ARCH_X86_64: {
      triton::arch::x86::x8664Cpu* cpup =
          reinterpret_cast<triton::arch::x86::x8664Cpu*>(cpui);
      this->cpu = std::make_shared<triton::arch::x86::x8664Cpu>(*cpup);
      break;
    }
    case triton::arch::architecture_e::ARCH_X86: {
      triton::arch::x86::x86Cpu* cpup =
          reinterpret_cast<triton::arch::x86::x86Cpu*>(cpui);
      this->cpu = std::make_shared<triton::arch::x86::x86Cpu>(*cpup);
      break;
    }
    case triton::arch::architecture_e::ARCH_AARCH64: {
      triton::arch::aarch64::AArch64Cpu* cpup =
          reinterpret_cast<triton::arch::aarch64::AArch64Cpu*>(cpui);
      this->cpu = std::make_shared<triton::arch::aarch64::AArch64Cpu>(*cpup);
      break;
    }
    default:
      break;
  }
  // Get the Modes object (copy by 'copy constructor', new object with the same
  // values)
  this->modes = triton::modes::Modes(api.snapshotModes());
  // Get the AstContext object (copy by 'copy constructor', new object with the
  // same values)
  this->astContext =
      std::make_shared<triton::ast::AstContext>(api.getAstContext());
  // Get the SymbolicEngine object (copy by 'copy constructor', new object with
  // the same values)
  this->symbolic = std::make_shared<triton::engines::symbolic::SymbolicEngine>(
      *(api.snapshotSymbolicEngine()));
  // Get the TaintEngine object (copy by 'copy constructor', new object with the
  // same values)
  this->taint = std::make_shared<triton::engines::taint::TaintEngine>(
      *(api.snapshotTaintEngine()));
}

SnapshotEngine::SnapshotEngine(const SnapshotEngine& other) {
  this->astContext = other.astContext;
  this->symbolic = other.symbolic;
  this->taint = other.taint;
  this->modes = other.modes;
  this->arch = other.arch;
  this->cpu = other.cpu;
}

std::shared_ptr<triton::arch::CpuInterface> SnapshotEngine::getCpuInstance()
    const {
  if (!this->cpu)
    throw triton::exceptions::Architecture(
        "Snapshot::getCpuInstance(): this->cpu is nullptr.");
  return this->cpu;
}

triton::arch::architecture_e SnapshotEngine::getArchEnum() const {
  return this->arch;
}

triton::ast::AstContext SnapshotEngine::getAstContext() const {
  return *(this->astContext);
}

triton::engines::symbolic::SymbolicEngine& SnapshotEngine::getSymbolicEngine()
    const {
  return *(this->symbolic);
}

triton::engines::taint::TaintEngine& SnapshotEngine::getTaintEngine() const {
  return *(this->taint);
}

triton::modes::Modes SnapshotEngine::getModes() const { return this->modes; }

};  // namespace snapshot
};  // namespace engines
};  // namespace triton
