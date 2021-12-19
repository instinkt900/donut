#pragma once

#include "vulkan_context.h"
#include "vulkan_reflectioninfo.h"

namespace donut::vulkan {
    
    class ReflectionInfo;
    class ShaderBindings;
    class Texture;

    class ShaderModule
    {
    public:
        enum class State : unsigned
        {
            Unloaded,
            Loading,
            Loaded,
            Error
        };

        ShaderModule(uint32_t hash = 0) : hash(hash) {};
        void Load(Context& context, void* data, size_t size);
        ShaderModule(ShaderModule&&) = default;
        ShaderModule& operator= (ShaderModule&&) = default;

        VkShaderModule GetModule() const { return shader_module; }
        const ReflectionInfo* GetReflectionInfo() const { return reflection_info.get(); }
        uint32_t GetHash() const { return hash; }
        bool TransitionState(State old_state, State new_state);
        void WaitLoaded();

    private:
        std::atomic<State> state = State::Unloaded;
        uint32_t hash;
        VkShaderModule shader_module;
        std::unique_ptr<ReflectionInfo> reflection_info;
    };


    class ShaderProgram
    {
    public:
        enum class Stage : unsigned
        {
            Vertex   = 1 << 0,
            Fragment = 1 << 1,
            Compute  = 1 << 2
        };

        enum class BindingType : unsigned
        {
            CombinedImageSampler = 0,
            SampledImage,
            Sampler,
            UniformBuffer,
            UniformBufferDynamic,
            StorageBuffer,
            Count
        };

        struct BindingAddress
        {
            unsigned int set;
            unsigned int binding;
        };

        struct BindingData
        {
            BindingType type;
            uint32_t id;
            unsigned int stage_flags;
            std::string name;
            BindingAddress address;
        };

        struct DescriptorSet
        {
            unsigned int set_index = -1;
            std::vector<VkDescriptorSetLayoutBinding> layout_bindings;
            VkDescriptorSetLayout layout;
            std::vector<BindingData> bindings;
            uint32_t layout_hash = 0;
            bool IsEmpty() const { return bindings.empty(); }
            uint32_t GetBindingIndexByName(std::string name);
        };

        struct PushConstants
        {
            uint32_t id;
            uint32_t offset;
            uint32_t range;
            std::string name;
            uint32_t stage_flags;
        };

        static const unsigned max_descriptor_sets = 4;

        static uint32_t CalculateHash(uint32_t fragment_hash, uint32_t vertex_hash, uint32_t compute_hash = 0);

        ShaderProgram();
        void AddModule(ShaderModule* shader_module, Stage stage);
        void Prepare(Context& context);

        BindingAddress GetBindingAddress(const std::string& name);
        const ShaderModule* VertexModule() const { WaitLoaded(); return vertex_module; }
        const ShaderModule* FragmentModule() const { WaitLoaded(); return fragment_module; }
        const ShaderModule* ComputeModule() const { WaitLoaded(); return compute_module; }
        
        const auto& GetDescriptorSets() const { WaitLoaded(); return descriptor_sets; }
        const DescriptorSet* GetDescriptorSet(unsigned set) const { WaitLoaded(); return &descriptor_sets[set]; }
        const BindingData* GetBinding(unsigned set, unsigned binding) const;
        const BindingData* GetBindingByName(const std::string& name) const;
        const std::vector<PushConstants>& GetPushConstants() const { WaitLoaded(); return push_constants; }
        const char* GetEntryPoint(Stage stage) const;

        uint32_t GetHash() const;
        bool Ready() const { return state == ShaderModule::State::Loaded; }
        
        void WaitLoaded() const
        {
            if (state == ShaderModule::State::Unloaded)
                return;

            while ((int)state.load() < (int)ShaderModule::State::Loaded)
                std::this_thread::yield();
        }

        bool TransitionState(ShaderModule::State old_state, ShaderModule::State new_state);

    private:
        void AppendBindings(const ShaderModule& module, ShaderProgram::Stage stage, std::map<std::pair<unsigned, unsigned>, int>& existing_bindings);
        void AppendPushConstants(const ShaderModule& module, ShaderProgram::Stage stage);

    private:
        std::atomic<ShaderModule::State> state = ShaderModule::State::Unloaded;
        ShaderModule* vertex_module = nullptr;
        ShaderModule* fragment_module = nullptr;
        ShaderModule* compute_module = nullptr;
        
        uint32_t hash;

        std::unordered_map<std::string, BindingAddress> name_binding_map;
        std::array<DescriptorSet, max_descriptor_sets> descriptor_sets;
        std::vector<PushConstants> push_constants;
    };

}