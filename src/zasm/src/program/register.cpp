 #include <Zydis/Zydis.h>
 #include <zasm/program/register.hpp>
 
 namespace zasm
 {
     static_assert(ZYDIS_REGISTER_AL < ZYDIS_REGISTER_AH);
     static constexpr int8_t kGp8HiStartIndex = ZYDIS_REGISTER_AH - ZYDIS_REGISTER_AL;
     static_assert(kGp8HiStartIndex == 4, "This should be 4, if this triggers the definition probably changed");
 
     static ZydisMachineMode getMode(MachineMode mode)
     {
         switch (mode)
         {
             case MachineMode::I386:
                 return ZYDIS_MACHINE_MODE_LONG_COMPAT_32;
             case MachineMode::AMD64:
                 return ZYDIS_MACHINE_MODE_LONG_64;
             default:
                 break;
         }
         assert(false);
         return ZYDIS_MACHINE_MODE_MAX_VALUE;
     }
 
     BitSize Reg::getBitSize(MachineMode mode) const noexcept
     {
         const auto id = static_cast<ZydisRegister>(getId());
         return toBitSize(ZydisRegisterGetWidth(getMode(mode), id));
     }
 
     Reg::Class Reg::getClass() const noexcept
     {
         const auto id = static_cast<ZydisRegister>(getId());
         return static_cast<Reg::Class>(ZydisRegisterGetClass(id));
     }
 
     int8_t Reg::getIndex() const noexcept
     {
         const auto id = static_cast<ZydisRegister>(getId());
         return ZydisRegisterGetId(id);
     }
 
     int8_t Reg::getPhysicalIndex() const noexcept
     {
         const auto id = static_cast<ZydisRegister>(getId());
         const auto regIndex = ZydisRegisterGetId(id);
         if (regIndex == -1)
             return -1;
         if (isGp8() && regIndex >= kGp8HiStartIndex)
         {
             return regIndex - kGp8HiStartIndex;
         }
         return regIndex;
     }
 
     Reg Reg::getRoot(MachineMode mode) const noexcept
     {
         const auto id = static_cast<ZydisRegister>(getId());
         const auto enclosingId = ZydisRegisterGetLargestEnclosing(getMode(mode), id);
         return Reg{ static_cast<Reg::Id>(enclosingId) };
     }
 
     int8_t Reg::getOffset() const noexcept
     {
         const auto id = static_cast<ZydisRegister>(getId());
         switch (id)
         {
             case ZYDIS_REGISTER_AH:
             case ZYDIS_REGISTER_CH:
             case ZYDIS_REGISTER_BH:
             case ZYDIS_REGISTER_DH:
                 return 1;
             default:
                 return 0;
         }
         return 0;
     }
 
     bool Reg::isGp8() const noexcept
     {
         return static_cast<ZydisRegisterClass>(getClass()) == ZydisRegisterClass::ZYDIS_REGCLASS_GPR8;
     }
 
     bool Reg::isGp8Lo() const noexcept
     {
         if (!isGp8())
             return false;
         const auto id = static_cast<ZydisRegister>(getId());
         switch (id)
         {
             case ZYDIS_REGISTER_AH:
             case ZYDIS_REGISTER_CH:
             case ZYDIS_REGISTER_BH:
             case ZYDIS_REGISTER_DH:
                 return false;
             default:
                 return true;
         }
         return true;
     }
 
     bool Reg::isGp8Hi() const noexcept
     {
         if (!isGp8())
             return false;
         const auto id = static_cast<ZydisRegister>(getId());
         switch (id)
         {
             case ZYDIS_REGISTER_AH:
             case ZYDIS_REGISTER_CH:
             case ZYDIS_REGISTER_BH:
             case ZYDIS_REGISTER_DH:
                 return true;
             default:
                 return false;
         }
         return false;
     }
 
     bool Reg::isGp16() const noexcept
     {
         return static_cast<ZydisRegisterClass>(getClass()) == ZydisRegisterClass::ZYDIS_REGCLASS_GPR16;
     }
 
     bool Reg::isGp32() const noexcept
     {
         return static_cast<ZydisRegisterClass>(getClass()) == ZydisRegisterClass::ZYDIS_REGCLASS_GPR32;
     }
 
     bool Reg::isGp64() const noexcept
     {
         return static_cast<ZydisRegisterClass>(getClass()) == ZydisRegisterClass::ZYDIS_REGCLASS_GPR64;
     }
 
     bool Reg::isGp() const noexcept
     {
         return isGp8() || isGp16() || isGp32() || isGp64();
     }
 
     bool Reg::isXmm() const noexcept
     {
         return static_cast<ZydisRegisterClass>(getClass()) == ZydisRegisterClass::ZYDIS_REGCLASS_XMM;
     }
 
     bool Reg::isYmm() const noexcept
     {
         return static_cast<ZydisRegisterClass>(getClass()) == ZydisRegisterClass::ZYDIS_REGCLASS_YMM;
     }
 
     bool Reg::isZmm() const noexcept
     {
         return static_cast<ZydisRegisterClass>(getClass()) == ZydisRegisterClass::ZYDIS_REGCLASS_ZMM;
     }
 
 } // namespace zasm
