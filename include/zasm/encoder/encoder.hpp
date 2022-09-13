 #pragma once
 
 #include <array>
 #include <cstdint>
 #include <zasm/base/mode.hpp>
 #include <zasm/core/errors.hpp>
 #include <zasm/core/expected.hpp>
 #include <zasm/program/instruction.hpp>
 
 namespace zasm
 {
     // Encoder context used for serialization by the Program.
     struct EncoderContext;
 
     enum class RelocationType : uint8_t
     {
         None = 0,
         Abs,
         Rel32,
     };
 
     enum class RelocationData : uint8_t
     {
         None = 0,
         Immediate,
         Memory,
         Data,
     };
 
     // A small buffer which holds the bytes of a single encoded instruction and the length.
     struct EncoderResult
     {
         std::array<uint8_t, 15> data{};
         uint8_t length{};
         RelocationType relocKind{};
         RelocationData relocData{};
         Label::Id relocLabel{ Label::Id::Invalid };
     };
 
     using EncoderOperands = std::array<Operand, 5 /* ZYDIS_ENCODER_MAX_OPERANDS */>;
 
     // Encodes with the requested instruction without a context and will use temporary
     // values for operands like labels and rip-rel addressing.
     Expected<EncoderResult, Error> encode(
         MachineMode mode, Instruction::Attribs attribs, Instruction::Mnemonic id, size_t numOps,
         const EncoderOperands& operands) noexcept;
 
     // Encodes with full context. This function still allows labels to be unbound and will not error
     // instead a temporary value be usually encoded. It is expected for the serialization to handle this
     // with multiple passes.
     Expected<EncoderResult, Error> encode(EncoderContext& ctx, MachineMode mode, const Instruction& instr) noexcept;
 
 } // namespace zasm
