#include "../../../../inc/engine/render/backend/layout.hpp"
#include "../../../../inc/engine/render/backend/device.hpp"

namespace cg::engine::backend {

Layout::Layout(Device &device) : m_device(device) {}
Layout::~Layout() {
    if (*m_device != VK_NULL_HANDLE && m_pipeline_layout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(*m_device, m_pipeline_layout, nullptr);
        m_pipeline_layout = VK_NULL_HANDLE;
    }
}

void Layout::addConstnat(VkShaderStageFlags stage, uint32_t offset,
                         uint32_t size) {
    VkPushConstantRange range{
        .stageFlags = stage,
        .offset = offset,
        .size = size,

    };
    m_constant.push_back(range);
}

bool Layout::init(const std::vector<VkDescriptorSetLayout> &dls) {
    VkPipelineLayoutCreateInfo layout_info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = static_cast<uint32_t>(dls.size()),
        .pSetLayouts = dls.data(),
        .pushConstantRangeCount = static_cast<uint32_t>(m_constant.size()),
        .pPushConstantRanges = m_constant.empty() ? nullptr : m_constant.data(),
    };

    if (VK_SUCCESS != vkCreatePipelineLayout(*m_device, &layout_info, nullptr,
                                             &m_pipeline_layout)) {
        spdlog::error("failed to create pipeline layout");
        return false;
    }

    return true;
}

} // namespace cg::engine::backend
