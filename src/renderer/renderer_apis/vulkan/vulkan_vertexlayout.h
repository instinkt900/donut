#pragma once

#include "vulkan_capsset.h"
#include "vulkan_types.h"
#include "vulkan_utils.h"

namespace donut::vulkan {
    enum class VertexAttrib : uint32_t {
        Position = 0,
        Normal,
        Bitangent,
        Tangent,
        TexCoord0,
        Corner,
        VertexColor,
        TexCoord1,
        JointWeights,
        JointIndices,
        Unknown,
        Count
    };

    class VertexLayout
    {
    public:
        struct Attrib
        {
            VertexAttrib attrib;
            Format format;
            uint32_t offset;
        };

        VertexLayout()
        {
            Clear();
        }

        void AddAttrib(VertexAttrib attrib, Format format, uint32_t size)
        {
            attribs[(uint32_t)attrib] = { attrib, format, stride };
            stride += size;
            hash = Hash(attribs.data(), sizeof(attribs));
        }

        void Clear()
        {
            for (auto& attrib : attribs)
                attrib = { VertexAttrib::Unknown, Format::Undefined , (uint32_t)-1 };

            stride = 0;
            hash = 0;
        }

        uint32_t GetHash() const { return hash; }
        uint32_t GetStride() const { return stride; }
        uint32_t GetAttribOffset(VertexAttrib attrib) const
        {
            assert(attribs[(uint32_t)attrib].offset != -1);
            return attribs[(uint32_t)attrib].offset;
        }

        Format GetAttribFormat(VertexAttrib attrib) const
        {
            assert(attribs[(uint32_t)attrib].format != Format::Undefined);
            return attribs[(uint32_t)attrib].format;
        }

    private:
        uint32_t stride = 0;
        uint32_t hash = 0;
        std::array<Attrib, (uint32_t)VertexAttrib::Count> attribs;
    };

    typedef CapsSet<VertexAttrib> VertexAttribSet;
    typedef std::shared_ptr<VertexAttribSet> VertexAttribSetPtr;
}
