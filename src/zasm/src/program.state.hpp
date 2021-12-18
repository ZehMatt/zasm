#pragma once

#include "zasm/data.hpp"
#include "zasm/encoder.hpp"
#include "zasm/label.hpp"
#include "zasm/node.hpp"
#include "zasm/objectpool.hpp"
#include "zasm/stringpool.hpp"

#include <zydis/Zydis.h>

namespace zasm::detail
{
    constexpr size_t PoolSize = 1u << 10;

    struct ProgramState
    {
        ZydisMachineMode mode{};
        Node* head{};
        Node* tail{};
        size_t nodeCount{};
        std::vector<uint8_t> codeBuffer{};
        ObjectPool<Node, PoolSize> nodePool;

        struct LabelData
        {
            Label::Id id{ Label::Id::Invalid };

            StringPool::Id nameId{ StringPool::Id::Invalid };

            // The physical offset of the label in the buffer.
            int64_t boundOffset{ -1 };

            // Virtual address of label, the base address passed in serialize is used here.
            int64_t boundVA{ -1 };

            // The node that holds/binds the label in the list.
            const Node* labelNode{};
        };

        StringPool labelNames;
        std::vector<LabelData> labels;

        ProgramState(ZydisMachineMode m)
            : mode(m)
        {
        }
    };

} // namespace zasm