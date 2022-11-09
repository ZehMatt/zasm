#pragma once

#include "immediate.hpp"
#include "label.hpp"
#include "register.hpp"

#include <cstdint>
#include <limits>
#include <zasm/base/mode.hpp>
#include <zasm/core/bitsize.hpp>
#include <zasm/core/packed.hpp>

namespace zasm
{
    class Mem final
    {
        using RegisterPack = Packed<std::uint32_t, Reg::Id, 10>;

        BitSize _bitSize{};
        RegisterPack _segBaseIndex{};
        std::uint8_t _scale{};
        std::int64_t _disp{};
        Label::Id _label{ Label::Id::Invalid };

    public:
        constexpr explicit Mem(
            BitSize bitSize, const Reg& seg, const Reg& base, const Reg& index, std::int32_t scale, std::int64_t disp) noexcept
            : _bitSize{ bitSize }
            , _segBaseIndex{ seg.getId(), base.getId(), index.getId() }
            , _scale{ static_cast<std::uint8_t>(scale) }
            , _disp{ disp }
        {
        }

        constexpr explicit Mem(
            BitSize bitSize, const Reg& seg, const Label& label, const Reg& base, const Reg& index, std::int32_t scale,
            std::int64_t disp) noexcept
            : _bitSize{ bitSize }
            , _segBaseIndex{ seg.getId(), base.getId(), index.getId() }
            , _scale{ static_cast<std::uint8_t>(scale) }
            , _disp{ disp }
            , _label{ label.getId() }
        {
        }

        constexpr Reg getSegment() const noexcept
        {
            return Reg{ _segBaseIndex.get<0>() };
        }

        Mem& setSegment(const Reg& reg) noexcept
        {
            _segBaseIndex.set<0>(reg.getId());
            return *this;
        }

        constexpr Reg getBase() const noexcept
        {
            return Reg{ _segBaseIndex.get<1>() };
        }

        Mem& setBase(const Reg& reg) noexcept
        {
            _segBaseIndex.set<1>(reg.getId());
            return *this;
        }

        constexpr Reg getIndex() const noexcept
        {
            return Reg{ _segBaseIndex.get<2>() };
        }

        Mem& setIndex(const Reg& reg) noexcept
        {
            _segBaseIndex.set<2>(reg.getId());
            return *this;
        }

        constexpr uint8_t getScale() const noexcept
        {
            // In case no index is assigned scale has to be zero.
            if (_segBaseIndex.get<1>() == Reg::Id::None)
            {
                return 0;
            }

            return _scale;
        }

        Mem& setScale(std::uint8_t scale) noexcept
        {
            _scale = scale;
            return *this;
        }

        constexpr std::int64_t getDisplacement() const noexcept
        {
            return _disp;
        }

        Mem& setDisplacement(std::int64_t disp) noexcept
        {
            _disp = disp;
            return *this;
        }

        BitSize getBitSize() const noexcept
        {
            return _bitSize;
        }

        Mem& setBitSize(BitSize bitSize) noexcept
        {
            _bitSize = bitSize;
            return *this;
        }

        BitSize getBitSize([[maybe_unused]] MachineMode mode) const noexcept
        {
            return getBitSize();
        }

        int32_t getByteSize() const noexcept
        {
            return ::zasm::getBitSize(_bitSize) / std::numeric_limits<uint8_t>::digits;
        }

        constexpr Label getLabel() const noexcept
        {
            return Label{ _label };
        }

        Mem& setLabel(const Label& label) noexcept
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
