//! \file
/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the BSD License.
*/

#include <triton/cpuInterface.hpp>
#include <triton/register.hpp>



namespace triton {
  namespace arch {

    Register::Register()
      : Register(triton::arch::ID_REG_INVALID, "unknown", triton::arch::ID_REG_INVALID, 0, 0, true) {
    }


    Register::Register(triton::arch::register_e regId, std::string name, triton::arch::register_e parent, triton::uint32 high, triton::uint32 low, bool vmutable)
      : BitsVector(high, low),
        name(name),
        id(regId),
        parent(parent),
        vmutable(vmutable) {
    }


    Register::Register(const triton::arch::CpuInterface& cpu, triton::arch::register_e regId)
      : Register(
          (regId == triton::arch::ID_REG_INVALID) ?
          triton::arch::Register(triton::arch::ID_REG_INVALID, "unknown", triton::arch::ID_REG_INVALID, 0, 0, true) : cpu.getRegister(regId)
        ) {
    }


    Register::Register(const Register& other)
      : BitsVector(other),
        AArch64OperandProperties(other) {
      this->copy(other);
    }


    void Register::copy(const Register& other) {
      this->id       = other.id;
      this->name     = other.name;
      this->parent   = other.parent;
      this->vmutable = other.vmutable;
    }


    triton::arch::register_e Register::getId(void) const {
      return this->id;
    }


    triton::arch::register_e Register::getParent(void) const {
      return this->parent;
    }


    triton::uint32 Register::getBitSize(void) const {
      return this->getVectorSize();
    }


    triton::uint32 Register::getSize(void) const {
      return this->getVectorSize() / BYTE_SIZE_BIT;
    }


    std::string Register::getName(void) const {
      return this->name;
    }


    triton::arch::operand_e Register::getType(void) const {
      return triton::arch::OP_REG;
    }


    bool Register::isOverlapWith(const Register& other) const {
      if (this->parent == other.parent) {
        if (this->getLow() <= other.getLow() && other.getLow() <= this->getHigh()) return true;
        if (other.getLow() <= this->getLow() && this->getLow() <= other.getHigh()) return true;
      }
      return false;
    }


    bool Register::isMutable(void) const {
      return this->vmutable;
    }


    bool Register::operator==(const Register& other) const {
      return getId() == other.getId();
    }


    bool Register::operator!=(const Register& other) const {
      return !(*this == other);
    }


    Register& Register::operator=(const Register& other) {
      AArch64OperandProperties::operator=(other);
      BitsVector::operator=(other);
      this->copy(other);
      return *this;
    }


    std::ostream& operator<<(std::ostream& stream, const Register& reg) {
      stream << reg.getName()
             << ":"
             << std::dec << reg.getBitSize()
             << " bv["
             << reg.getHigh()
             << ".."
             << reg.getLow()
             << "]";
      return stream;
    }


    std::ostream& operator<<(std::ostream& stream, const Register* reg) {
      stream << *reg;
      return stream;
    }


    bool operator<(const Register& reg1, const Register& reg2) {
        return (reg1.getId() < reg2.getId());
    }

  }; /* arch namespace */
}; /* triton namespace */
