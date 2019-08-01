//! \file
/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the BSD License.
*/

#include <triton/cpuSize.hpp>
#include <triton/exceptions.hpp>
#include <triton/memoryAccess.hpp>



namespace triton {
  namespace arch {

    MemoryAccess::MemoryAccess() {
      this->address    = 0;
      this->leaAst     = nullptr;
      this->pcRelative = 0;
    }


    MemoryAccess::MemoryAccess(triton::uint64 address, triton::uint32 size /* bytes */)
      : MemoryAccess() {
      this->address = address;

      if (size == 0)
        throw triton::exceptions::MemoryAccess("MemoryAccess::MemoryAccess(): size cannot be zero.");

      if (size != BYTE_SIZE     &&
          size != WORD_SIZE     &&
          size != DWORD_SIZE    &&
          size != QWORD_SIZE    &&
          size != DQWORD_SIZE   &&
          size != QQWORD_SIZE   &&
          size != DQQWORD_SIZE)
        throw triton::exceptions::MemoryAccess("MemoryAccess::MemoryAccess(): size must be aligned.");

      this->setPair(std::make_pair(((size * BYTE_SIZE_BIT) - 1), 0));
    }


    MemoryAccess::MemoryAccess(const MemoryAccess& other)
      : BitsVector(other) {
      this->copy(other);
    }


    triton::uint64 MemoryAccess::getAddress(void) const {
      return this->address;
    }


    triton::ast::SharedAbstractNode MemoryAccess::getLeaAst(void) const {
      return this->leaAst;
    }


    triton::uint64 MemoryAccess::getPcRelative(void) const {
      return this->pcRelative;
    }


    triton::uint32 MemoryAccess::getBitSize(void) const {
      return this->getVectorSize();
    }


    triton::uint32 MemoryAccess::getSize(void) const {
      return this->getVectorSize() / BYTE_SIZE_BIT;
    }


    triton::arch::operand_e MemoryAccess::getType(void) const {
      return triton::arch::OP_MEM;
    }


    triton::arch::Register& MemoryAccess::getSegmentRegister(void) {
      return this->segmentReg;
    }


    triton::arch::Register& MemoryAccess::getBaseRegister(void) {
      return this->baseReg;
    }


    triton::arch::Register& MemoryAccess::getIndexRegister(void) {
      return this->indexReg;
    }


    triton::arch::Immediate& MemoryAccess::getDisplacement(void) {
      return this->displacement;
    }


    triton::arch::Immediate& MemoryAccess::getScale(void) {
      return this->scale;
    }


    const triton::arch::Register& MemoryAccess::getConstSegmentRegister(void) const {
      return this->segmentReg;
    }


    const triton::arch::Register& MemoryAccess::getConstBaseRegister(void) const {
      return this->baseReg;
    }


    const triton::arch::Register& MemoryAccess::getConstIndexRegister(void) const {
      return this->indexReg;
    }


    const triton::arch::Immediate& MemoryAccess::getConstDisplacement(void) const {
      return this->displacement;
    }


    const triton::arch::Immediate& MemoryAccess::getConstScale(void) const {
      return this->scale;
    }


    bool MemoryAccess::isOverlapWith(const MemoryAccess& other) const {
      if (this->getAddress() <= other.getAddress() && other.getAddress() < (this->getAddress() + this->getSize())) return true;
      if (other.getAddress() <= this->getAddress() && this->getAddress() < (other.getAddress() + other.getSize())) return true;
      return false;
    }


    void MemoryAccess::setAddress(triton::uint64 addr) {
      this->address = addr;
    }


    void MemoryAccess::setPcRelative(triton::uint64 addr) {
      this->pcRelative = addr;
    }


    void MemoryAccess::setSegmentRegister(const triton::arch::Register& segment) {
      this->segmentReg = segment;
    }


    void MemoryAccess::setBaseRegister(const triton::arch::Register& base) {
      this->baseReg = base;
    }


    void MemoryAccess::setIndexRegister(const triton::arch::Register& index) {
      this->indexReg = index;
    }


    void MemoryAccess::setDisplacement(const triton::arch::Immediate& displacement) {
      this->displacement = displacement;
    }


    void MemoryAccess::setScale(const triton::arch::Immediate& scale) {
      this->scale = scale;
    }


    void MemoryAccess::setLeaAst(const triton::ast::SharedAbstractNode& ast) {
      this->leaAst = ast;
    }


    MemoryAccess& MemoryAccess::operator=(const MemoryAccess& other) {
      BitsVector::operator=(other);
      this->copy(other);
      return *this;
    }


    void MemoryAccess::copy(const MemoryAccess& other) {
      this->address      = other.address;
      this->baseReg      = other.baseReg;
      this->displacement = other.displacement;
      this->indexReg     = other.indexReg;
      this->leaAst       = other.leaAst;
      this->pcRelative   = other.pcRelative;
      this->scale        = other.scale;
      this->segmentReg   = other.segmentReg;
    }


    std::ostream& operator<<(std::ostream& stream, const MemoryAccess& mem) {
      stream << "[@0x"
             << std::hex << mem.getAddress()
             << "]:"
             << std::dec << mem.getBitSize()
             << " bv["
             << mem.getHigh()
             << ".."
             << mem.getLow()
             << "]";
      return stream;
    }


    std::ostream& operator<<(std::ostream& stream, const MemoryAccess* mem) {
      stream << *mem;
      return stream;
    }


    bool operator==(const MemoryAccess& mem1, const MemoryAccess& mem2) {
      if (mem1.getAddress() != mem2.getAddress())
        return false;
      if (mem1.getSize() != mem2.getSize())
        return false;
      if (mem1.getConstBaseRegister() != mem2.getConstBaseRegister())
        return false;
      if (mem1.getConstIndexRegister() != mem2.getConstIndexRegister())
        return false;
      if (mem1.getConstScale() != mem2.getConstScale())
        return false;
      if (mem1.getConstDisplacement() != mem2.getConstDisplacement())
        return false;
      if (mem1.getConstSegmentRegister() != mem2.getConstSegmentRegister())
        return false;
      if (mem1.getPcRelative() != mem2.getPcRelative())
        return false;
      return true;
    }


    bool operator!=(const MemoryAccess& mem1, const MemoryAccess& mem2) {
      return !(mem1 == mem2);
    }


    bool operator<(const MemoryAccess& mem1, const MemoryAccess& mem2) {
      triton::uint64 seed1 = 0;
      triton::uint64 seed2 = 0;

      /*
       * Golden ratio 32-bits -> 0x9e3779b9
       * Golden ratio 64-bits -> 0x9e3779b97f4a7c13
       */
      seed1 ^= mem1.getAddress() + 0x9e3779b97f4a7c13 + (seed1 << 6) + (seed1 >> 2);
      seed1 ^= mem1.getSize() + 0x9e3779b97f4a7c13 + (seed1 << 6) + (seed1 >> 2);

      seed2 ^= mem2.getAddress() + 0x9e3779b97f4a7c13 + (seed2 << 6) + (seed2 >> 2);
      seed2 ^= mem2.getSize() + 0x9e3779b97f4a7c13 + (seed2 << 6) + (seed2 >> 2);

      return (seed1 < seed2);
    }

  }; /* arch namespace */
}; /* triton namespace */
