#include <iostream>
#include <triton/api.hpp>
#include <triton/ast.hpp>
#include <triton/snapshotEngine.hpp>
#include <triton/x86Specifications.hpp>

using namespace std;
using namespace triton;
using namespace triton::arch;
using namespace triton::arch::x86;
using namespace triton::engines::snapshot;

struct op {
  triton::uint64 addr;
  std::string inst;
};

std::vector<struct op> trace{
    {0x00, "\x48\x83\xFB\x01"},              // cmp    rbx,0x1
    {0x04, "\x74\x0A"},                      // je     7 <label_true>
    {0x06, "\x90"},                          // nop
    {0x07, "\x48\xC7\xC0\x01\x01\x01\x01"},  // mov    rax,0x1
    {0x0E, "\xEB\x07"},                      // jmp    16 <exit>
    {0x10, "\x48\xC7\xC0\x02\x02\x02\x02"}   // mov    rax,0x2
};

int main() {
  /* Create the Triton API */
  triton::API api;

  /* Set the arch */
  api.setArchitecture(ARCH_X86_64);

  /* Define RAX and RBX as symbolic variables */
  api.convertRegisterToSymbolicVariable(
      triton::arch::Register(api.getRegister(ID_REG_X86_RAX)));
  api.convertRegisterToSymbolicVariable(
      triton::arch::Register(api.getRegister(ID_REG_X86_RBX)));

  /* Keep a dictionary of snapshots */
  std::map<triton::uint64, std::shared_ptr<SnapshotEngine>> snapshots;

  /* Explore the assembly CFG */

  for (struct op& op : trace) {
    /* Determine if we should restore a snapshot */
    if (snapshots.find(op.addr) != snapshots.end()) {
      std::cout << "Restoring snapshot @ " << std::hex << op.addr << std::endl;
      api.restoreSnapshot(*snapshots[op.addr]);
      std::cout << "RAX = "
                << api.getSymbolicRegister(api.getRegister(ID_REG_X86_RAX))
                << endl;
    }
    /* If we reach the address 0x07 symbolize RCX */
    if (op.addr == 0x07) {
      api.convertRegisterToSymbolicVariable(
          triton::arch::Register(api.getRegister(ID_REG_X86_RCX)));
    }
    /* Show the symbolic value of RCX */
    auto rcx = api.getSymbolicRegister(api.getRegister(ID_REG_X86_RCX));
    if (rcx != nullptr) {
      std::cout << "RCX = " << rcx << endl;
    }
    /* Build an instruction */
    Instruction inst;
    /* Setup address */
    inst.setAddress(op.addr);
    /* Setup opcode */
    inst.setOpcode((const triton::uint8*)op.inst.c_str(), op.inst.size());
    /* Process the instruction */
    api.processing(inst);
    /* Print the processed instruction */
    std::cout << std::hex << op.addr << ": " << inst.getDisassembly()
              << std::endl;
    /* Check if it's a JCC */
    if (inst.isControlFlow() && inst.getType() != ID_INS_JMP) {
      /* Get the 2 addresses */
      triton::uint64 jmp1 = inst.operands[0].getImmediate().getValue();
      triton::uint64 jmp2 = inst.getAddress() + inst.getSize();
      /* Snapshot the engine */
      std::shared_ptr<SnapshotEngine> snapshot = api.createSnapshot();
      /* Save the snapshot */
      std::cout << "Taking snapshot @ " << std::hex << jmp1 << std::endl;
      snapshots.insert(
          std::pair<triton::uint64, std::shared_ptr<SnapshotEngine>>(jmp1,
                                                                     snapshot));
      std::cout << "Taking snapshot @ " << std::hex << jmp2 << std::endl;
      snapshots.insert(
          std::pair<triton::uint64, std::shared_ptr<SnapshotEngine>>(jmp2,
                                                                     snapshot));
    }
  }

  return 0;
}