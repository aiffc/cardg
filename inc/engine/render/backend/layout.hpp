#pragma once

#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace cg::engine::backend {

class Device;

class Layout {
  private:
    Device &m_device;
    VkPipelineLayout m_pipeline_layout = VK_NULL_HANDLE;
    std::vector<VkPushConstantRange> m_constant;

  public:
    Layout(Device &device);
    ~Layout();

    void addConstnat(VkShaderStageFlags stage, uint32_t offset, uint32_t size);
    bool init(const std::vector<VkDescriptorSetLayout> &dls = {});

    VkPipelineLayout &operator*() { return m_pipeline_layout; }

    Layout(Layout &) = delete;
    Layout(Layout &&) = delete;
    Layout &operator=(Layout &) = delete;
    Layout &operator=(Layout &&) = delete;
};

} // namespace cg::engine::backend
