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

    /**
     * @brief
     * init descriptor set layout poll and set
     */
    bool init();
    /**
     * @brief
     * add descriptor binding call it before init!!!
     *
     * @param [binding] binding index corresponding binding in shader
     * @param [type] descriptor type uniform buffer or sampler or ...
     * @param [flag] work on whitch shader
     * @param [count] how manay set you want alloc
     * @param [sampler] static sampler
     */
    void
    addDescriptorBinding(uint32_t binding, VkDescriptorType type,
                         VkShaderStageFlags flags = VK_SHADER_STAGE_VERTEX_BIT,
                         uint32_t count = 1,
                         const VkSampler *sampler = nullptr);
    /**
     * @brief
     * write buffer to descriptor
     *
     * @param [buffer] buffer you want write
     * @param [dst_binding] binding index corresponding binding in shader
     * @param [dst_array_element] dst array element
     * @param [type] work on whitch shader
     * @param [offset] offsett
     */
    void updateBuffer(const Buffer &buffer, uint32_t dst_binding,
                      uint32_t dst_array_element, VkDescriptorType type,
                      size_t offset = 0);
    /**
     * @brief
     * write texture to descriptor
     *
     * @param [texture] texture you want write
     * @param [dst_binding] binding index corresponding binding in shader
     * @param [dst_array_element] dst array element
     */
    void updateTexture(Texture &texture, uint32_t dst_binding,
                       uint32_t dst_array_element);

    /**
     * @brief
     * get descriptor layout
     *
     * @return VkDescriptorSetLayout
     *
     */
    VkDescriptorSetLayout &operator*() { return m_descriptor_layout; }
    /**
     * @brief
     * get descriptor set
     *
     * @return VkDescriptorSet
     *
     */
    VkDescriptorSet &set() { return m_descriptor_set; }

    void maxSet(uint32_t v) { m_max_set = v; }
    uint32_t maxSet() const { return m_max_set; }

    Descriptor(Descriptor &) = delete;
    Descriptor(Descriptor &&) = delete;
    Descriptor &operator=(Descriptor &) = delete;
    Descriptor &operator=(Descriptor &&) = delete;
};
} // namespace cg::engine::backend
