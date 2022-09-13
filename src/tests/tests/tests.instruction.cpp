 #include "../testutils.hpp"
 
 #include <gtest/gtest.h>
 #include <zasm/zasm.hpp>
 
 namespace zasm::tests
 {
     TEST(InstructionTests, BuildInstruction)
     {
         Program program(MachineMode::AMD64);
 
         x86::Assembler assembler(program);
 
         const auto instr = zasm::Instruction()                  //
                                .setMnemonic(x86::Mnemonic::Mov) //
                                .addOperand(x86::eax)            //
                                .addOperand(zasm::Imm(1));       //
 
         ASSERT_EQ(instr.isMetaDataValid(), false);
         ASSERT_EQ(assembler.emit(instr), Error::None);
 
         const auto* instrNode = assembler.getCursor();
         ASSERT_NE(instrNode, nullptr);
         ASSERT_EQ(instrNode->get<Instruction>().isMetaDataValid(), true);
 
         Serializer serializer;
         ASSERT_EQ(serializer.serialize(program, 0x00400000), Error::None);
 
         const std::array<uint8_t, 5> expected = {
             0xB8, 0x01, 0x00, 0x00, 0x00,
         };
         ASSERT_EQ(serializer.getCodeSize(), expected.size());
 
         const auto* data = serializer.getCode();
         ASSERT_NE(data, nullptr);
         for (size_t i = 0; i < expected.size(); i++)
         {
             ASSERT_EQ(data[i], expected[i]);
         }
     }
 
     TEST(InstructionTests, ModifyInstruction)
     {
         Program program(MachineMode::AMD64);
 
         x86::Assembler assembler(program);
 
         assembler.mov(x86::eax, zasm::Imm(2));
 
         const auto* instrNode = assembler.getCursor();
 
         // Make copy.
         Instruction instr = assembler.getCursor()->get<Instruction>();
         ASSERT_EQ(instr.isMetaDataValid(), true);
 
         // Modify operand.
         instr.setOperand(1, zasm::Imm(1));
         ASSERT_EQ(instr.isMetaDataValid(), false);
 
         // Emit modified
         ASSERT_EQ(assembler.emit(instr), Error::None);
 
         // Remove original
         program.destroy(instrNode);
 
         instrNode = assembler.getCursor();
         ASSERT_NE(instrNode, nullptr);
         ASSERT_EQ(instrNode->get<Instruction>().isMetaDataValid(), true);
 
         Serializer serializer;
         ASSERT_EQ(serializer.serialize(program, 0x00400000), Error::None);
 
         const std::array<uint8_t, 5> expected = {
             0xB8, 0x01, 0x00, 0x00, 0x00,
         };
         ASSERT_EQ(serializer.getCodeSize(), expected.size());
 
         const auto* data = serializer.getCode();
         ASSERT_NE(data, nullptr);
         for (size_t i = 0; i < expected.size(); i++)
         {
             ASSERT_EQ(data[i], expected[i]);
         }
     }
 
 } // namespace zasm::tests
