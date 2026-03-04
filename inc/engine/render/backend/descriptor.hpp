#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace cg::engine::backend {

class Device;
struct Buffer;
struct Texture;

class Descriptor {
  private:
    Device &m_device;
    VkDescriptorSetLayout m_descriptor_layout = VK_NULL_HANDLE;
    VkDescriptorPool m_descriptor_pool = VK_NULL_HANDLE;
    VkDescriptorSet m_descriptor_set = VK_NULL_HANDLE;
    uint32_t m_max_set = 1;
    std::vector<VkDescriptorPoolSize> m_pool_size;
    std::vector<VkDescriptorSetLayoutBinding> m_descriptor_bindings;

  public:
    Descriptor(Device &device);
    ~Descriptor();

    bool init();
    void
    addDescriptorBinding(uint32_t binding, VkDescriptorType type,
                         VkShaderStageFlags flags = VK_SHADER_STAGE_VERTEX_BIT,
                         uint32_t count = 1,
                         const VkSampler *sampler = nullptr);
    void updateBuffer(const Buffer &buffer, uint32_t dst_binding,
                      uint32_t dst_array_element, VkDescriptorType type);
    void updateTexture(Texture &texture, uint32_t dst_binding,
                       uint32_t dst_array_element);

    VkDescriptorSetLayout &operator*() { return m_descriptor_layout; }
    VkDescriptorSet &set() { return m_descriptor_set; }

    void maxSet(uint32_t v) { m_max_set = v; }
    uint32_t maxSet() const { return m_max_set; }

    Descriptor(Descriptor &) = delete;
    Descriptor(Descriptor &&) = delete;
    Descriptor &operator=(Descriptor &) = delete;
    Descriptor &operator=(Descriptor &&) = delete;
};
} // namespace cg::engine::backend
