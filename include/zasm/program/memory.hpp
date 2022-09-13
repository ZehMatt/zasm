 #pragma once
 
 #include "immediate.hpp"
 #include "label.hpp"
 #include "register.hpp"
 
 #include <zasm/base/mode.hpp>
 #include <zasm/core/bitsize.hpp>
 #include <zasm/core/packed.hpp>
 
 namespace zasm
 {
     class Mem
     {
         using RegisterPack = Packed<uint32_t, Reg::Id, 10>;
 
         RegisterPack _segBaseIndex{};
         BitSize _bitSize{};
         uint8_t _scale{};
         int64_t _disp{};
         Label::Id _label{ Label::Id::Invalid };
 
     public:
         constexpr explicit Mem(
             BitSize bitSize, const Reg& seg, const Reg& base, const Reg& index, int32_t scale, int64_t disp) noexcept
             : _bitSize{ bitSize }
             , _segBaseIndex{ seg.getId(), base.getId(), index.getId() }
             , _scale{ static_cast<uint8_t>(scale) }
             , _disp{ disp }
             , _label{ Label::Id::Invalid }
         {
         }
 
         constexpr explicit Mem(
             BitSize bitSize, const Reg& seg, const Label& label, const Reg& base, const Reg& index, int32_t scale,
             int64_t disp) noexcept
             : _bitSize{ bitSize }
             , _segBaseIndex{ seg.getId(), base.getId(), index.getId() }
             , _scale{ static_cast<uint8_t>(scale) }
             , _disp{ disp }
             , _label{ label.getId() }
         {
         }
 
         constexpr Reg getSegment() const noexcept
         {
             return Reg{ _segBaseIndex.get<0>() };
         }
 
         Mem& setSegment(const Reg& reg)
         {
             _segBaseIndex.set<0>(reg.getId());
             return *this;
         }
 
         constexpr Reg getBase() const noexcept
         {
             return Reg{ _segBaseIndex.get<1>() };
         }
 
         Mem& setBase(const Reg& reg)
         {
             _segBaseIndex.set<1>(reg.getId());
             return *this;
         }
 
         constexpr Reg getIndex() const noexcept
         {
             return Reg{ _segBaseIndex.get<2>() };
         }
 
         Mem& setIndex(const Reg& reg)
         {
             _segBaseIndex.set<2>(reg.getId());
             return *this;
         }
 
         constexpr uint8_t getScale() const noexcept
         {
             // In case no index is assigned scale has to be zero.
             if (_segBaseIndex.get<1>() == Reg::Id::None)
                 return 0;
 
             return _scale;
         }
 
         Mem& setScale(uint8_t scale)
         {
             _scale = scale;
             return *this;
         }
 
         constexpr int64_t getDisplacement() const noexcept
         {
             return _disp;
         }
 
         Mem& setDisplacement(int64_t disp)
         {
             _disp = disp;
             return *this;
         }
 
         BitSize getBitSize() const noexcept
         {
             return _bitSize;
         }
 
         Mem& setBitSize(BitSize bitSize)
         {
             _bitSize = bitSize;
             return *this;
         }
 
         BitSize getBitSize(MachineMode) const noexcept
         {
             return getBitSize();
         }
 
         int32_t getByteSize() const noexcept
         {
             return ::zasm::getBitSize(_bitSize) / 8;
         }
 
         constexpr Label getLabel() const noexcept
         {
             return Label{ _label };
         }
 
         Mem& setLabel(const Label& label)
         {
             _label = label.getId();
             return *this;
         }
 
         constexpr Label::Id getLabelId() const noexcept
         {
             return _label;
         }
 
         constexpr bool hasLabel() const noexcept
         {
             return _label != Label::Id::Invalid;
         }
     };
 
 } // namespace zasm
