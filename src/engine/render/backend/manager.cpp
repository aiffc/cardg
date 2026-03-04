#include "../../../../inc/engine/render/backend/manager.hpp"
#include "../../../../inc/engine/render/backend/device.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <memory>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan_core.h>

namespace cg::engine::backend {

ManagerHashContainer::ManagerHashContainer() = default;
ManagerHashContainer::~ManagerHashContainer() = default;

RendererManager::RendererManager(Device &device) : m_device(device) {}

RendererManager::~RendererManager() = default;

bool RendererManager::initBasePipeline(const glm::vec2 &size) {
    auto base = std::make_unique<ManagerHashContainer>();
    base->layout = std::make_unique<Layout>(m_device);
    if (!base->layout->init()) {
        spdlog::error("failed to create pipeline layout");
        return false;
    }
    base->pipeline = std::make_unique<GraphicsPipeline>(m_device);
    base->pipeline->addShader(VK_SHADER_STAGE_VERTEX_BIT,
                              "../shaders/base/vert.spv");
    base->pipeline->addShader(VK_SHADER_STAGE_FRAGMENT_BIT,
                              "../shaders/base/frag.spv");
    base->pipeline->addViewport(size.x, size.y);
    base->pipeline->addScissor((uint32_t)size.x, (uint32_t)size.y);
    base->pipeline->addColorBlendAttachemt();
    base->pipeline->addBinding(0, sizeof(cg::engine::buffer::Base));
    base->pipeline->addAttribute(0, 0, VK_FORMAT_R32G32_SFLOAT,
                                 offsetof(cg::engine::buffer::Base, pos));
    base->pipeline->addAttribute(1, 0, VK_FORMAT_R32G32B32_SFLOAT,
                                 offsetof(cg::engine::buffer::Base, color));
    if (!base->pipeline->init(**base->layout)) {
        spdlog::error("failed to create base pipeline");
        return false;
    }
    m_container.emplace("base pipeline", std::move(base));
    return true;
}

bool RendererManager::initBaseTexturePipeline(const glm::vec2 &size) {
    auto base = std::make_unique<ManagerHashContainer>();

    base->descriptor = std::make_unique<Descriptor>(m_device);
    base->descriptor->addDescriptorBinding(
        0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        VK_SHADER_STAGE_FRAGMENT_BIT);
    if (!base->descriptor->init()) {
        spdlog::error("failed to create descriptor things");
        return false;
    }
    base->layout = std::make_unique<Layout>(m_device);
    if (!base->layout->init({**base->descriptor})) {
        spdlog::error("failed to create pipeline layout");
        return false;
    }
    base->pipeline = std::make_unique<GraphicsPipeline>(m_device);
    base->pipeline->addShader(VK_SHADER_STAGE_VERTEX_BIT,
                              "../shaders/texture/vert.spv");
    base->pipeline->addShader(VK_SHADER_STAGE_FRAGMENT_BIT,
                              "../shaders/texture/frag.spv");
    base->pipeline->addViewport(size.x, size.y);
    base->pipeline->addScissor((uint32_t)size.x, (uint32_t)size.y);
    base->pipeline->addColorBlendAttachemt();
    base->pipeline->addBinding(0, sizeof(cg::engine::buffer::BaseTexture));
    base->pipeline->addAttribute(
        0, 0, VK_FORMAT_R32G32_SFLOAT,
        offsetof(cg::engine::buffer::BaseTexture, pos));
    base->pipeline->addAttribute(
        1, 0, VK_FORMAT_R32G32_SFLOAT,
        offsetof(cg::engine::buffer::BaseTexture, coord));
    if (!base->pipeline->init(**base->layout)) {
        spdlog::error("failed to create base pipeline");
        return false;
    }
    m_container.emplace("base texture pipeline", std::move(base));
    return true;
}

bool RendererManager::initBaseTextureArrayPipeline(const glm::vec2 &size) {
    auto base = std::make_unique<ManagerHashContainer>();

    base->descriptor = std::make_unique<Descriptor>(m_device);
    base->descriptor->addDescriptorBinding(
        0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        VK_SHADER_STAGE_FRAGMENT_BIT);
    base->descriptor->addDescriptorBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                           VK_SHADER_STAGE_FRAGMENT_BIT);
    if (!base->descriptor->init()) {
        spdlog::error("failed to create descriptor things");
        return false;
    }
    base->layout = std::make_unique<Layout>(m_device);
    if (!base->layout->init({**base->descriptor})) {
        spdlog::error("failed to create pipeline layout");
        return false;
    }
    base->pipeline = std::make_unique<GraphicsPipeline>(m_device);
    base->pipeline->addShader(VK_SHADER_STAGE_VERTEX_BIT,
                              "../shaders/texture_array/vert.spv");
    base->pipeline->addShader(VK_SHADER_STAGE_FRAGMENT_BIT,
                              "../shaders/texture_array/frag.spv");
    base->pipeline->addViewport(size.x, size.y);
    base->pipeline->addScissor((uint32_t)size.x, (uint32_t)size.y);
    base->pipeline->addColorBlendAttachemt();
    base->pipeline->addBinding(0, sizeof(cg::engine::buffer::BaseTexture));
    base->pipeline->addAttribute(
        0, 0, VK_FORMAT_R32G32_SFLOAT,
        offsetof(cg::engine::buffer::BaseTexture, pos));
    base->pipeline->addAttribute(
        1, 0, VK_FORMAT_R32G32_SFLOAT,
        offsetof(cg::engine::buffer::BaseTexture, coord));
    if (!base->pipeline->init(**base->layout)) {
        spdlog::error("failed to create base pipeline");
        return false;
    }

    base->uniforms =
        m_device.createUniformBuffer<cg::engine::buffer::BaseTextureArrayU>();
    base->descriptor->updateBuffer(*base->uniforms, 1, 0,
                                   VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    m_container.emplace("base texture array pipeline", std::move(base));
    return true;
}
bool RendererManager::init(const glm::vec2 &size) {
    m_window_size = size;
    if (!initBasePipeline(size)) {
        return false;
    }
    if (!initBaseTexturePipeline(size)) {
        return false;
    }
    if (!initBaseTextureArrayPipeline(size)) {
        return false;
    }
    return true;
}

void RendererManager::addBasePipelineVertexBuffer(
    const std::vector<cg::engine::buffer::Base> &data) {
    auto it = m_container.find("base pipeline");
    if (it != m_container.end()) {
        auto buff = m_device.createUsageBuffer<cg::engine::buffer::Base>(
            data, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        if (buff) {
            it->second->vbuffers = std::move(buff);
        } else {
            spdlog::warn("failed to create vertex buffer for base pipeline");
        }

    } else {
        spdlog::warn("base pipeline not found");
    }
}
void RendererManager::addBasePipelineIndexBuffer(
    const std::vector<uint32_t> &data) {
    auto it = m_container.find("base pipeline");
    if (it != m_container.end()) {
        auto buff = m_device.createUsageBuffer<uint32_t>(
            data, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        if (buff) {
            it->second->ibuffers = std::move(buff);
        } else {
            spdlog::warn("failed to create index buffer for base pipeline");
        }

    } else {
        spdlog::warn("base pipeline not found");
    }
}

void RendererManager::addBaseTexturePipelineVertexBuffer(
    const std::vector<cg::engine::buffer::BaseTexture> &data) {
    auto it = m_container.find("base texture pipeline");
    if (it != m_container.end()) {
        auto buff = m_device.createUsageBuffer<cg::engine::buffer::BaseTexture>(
            data, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        if (buff) {
            it->second->vbuffers = std::move(buff);
        } else {
            spdlog::warn("failed to create vertex buffer for texture pipeline");
        }

    } else {
        spdlog::warn("base pipeline not found");
    }
}

void RendererManager::addBaseTexturePipelineIndexBuffer(
    const std::vector<uint32_t> &data) {
    auto it = m_container.find("base texture pipeline");
    if (it != m_container.end()) {
        auto buff = m_device.createUsageBuffer<uint32_t>(
            data, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        if (buff) {
            it->second->ibuffers = std::move(buff);
        } else {
            spdlog::warn("failed to create index buffer for texture pipeline");
        }

    } else {
        spdlog::warn("base pipeline not found");
    }
}

void RendererManager::addBaseTextureTexture(std::string_view path) {
    auto it = m_container.find("base texture pipeline");
    if (it != m_container.end()) {
        auto texture = m_device.createTexture(path);
        if (texture) {
            if (it->second->descriptor) {
                it->second->descriptor->updateTexture(*texture, 0, 0);
                it->second->texture = std::move(texture);
            } else {
                spdlog::warn("no descriptor in texture pipeline");
            }
        } else {
            spdlog::warn("failed to create texture for texture pipeline");
        }

    } else {
        spdlog::warn("base pipeline not found");
    }
}

void RendererManager::addBaseTextureArrayPipelineVertexBuffer(
    const std::vector<cg::engine::buffer::BaseTexture> &data) {
    auto it = m_container.find("base texture array pipeline");
    if (it != m_container.end()) {
        auto buff = m_device.createUsageBuffer<cg::engine::buffer::BaseTexture>(
            data, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        if (buff) {
            it->second->vbuffers = std::move(buff);
        } else {
            spdlog::warn("failed to create vertex buffer for base pipeline");
        }

    } else {
        spdlog::warn("base pipeline not found");
    }
}

void RendererManager::addBaseTextureArrayPipelineIndexBuffer(
    const std::vector<uint32_t> &data) {
    auto it = m_container.find("base texture array pipeline");
    if (it != m_container.end()) {
        auto buff = m_device.createUsageBuffer<uint32_t>(
            data, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        if (buff) {
            it->second->ibuffers = std::move(buff);
        } else {
            spdlog::warn("failed to create index buffer for base pipeline");
        }

    } else {
        spdlog::warn("base pipeline not found");
    }
}

void RendererManager::addBaseTextureArrayTexture(
    const std::vector<std::string_view> &path) {
    auto it = m_container.find("base texture array pipeline");
    if (it != m_container.end()) {
        auto texture = m_device.createTextureArray(path);
        if (texture) {
            it->second->descriptor->updateTexture(*texture, 0, 0);
            it->second->texture = std::move(texture);
        } else {
            spdlog::warn("failed to create index buffer for base pipeline");
        }

    } else {
        spdlog::warn("base pipeline not found");
    }
}

void RendererManager::mapBaseTextureArrayUniform(
    cg::engine::buffer::BaseTextureArrayU &index) {
    auto it = m_container.find("base texture array pipeline");
    if (it != m_container.end()) {
        auto &buffer = it->second->uniforms;
        if (buffer) {
            memcpy(buffer->data, &index,
                   sizeof(cg::engine::buffer::BaseTextureArrayU));
        } else {
            spdlog::warn("failed to create index buffer for base pipeline");
        }

    } else {
        spdlog::warn("base pipeline not found");
    }
}

void RendererManager::setViewport(float w, float h, float x, float y, float min,
                                  float max) {
    VkViewport v{
        .x = x,
        .y = y,
        .width = w,
        .height = h,
        .minDepth = min,
        .maxDepth = max,
    };
    if (w == 0.0f) {
        v.width = static_cast<float>(m_window_size.x);
    }
    if (h == 0.0f) {
        v.height = static_cast<float>(m_window_size.y);
    }
    vkCmdSetViewport(m_device.cmd(), 0, 1, &v);
}

void RendererManager::setScissor(uint32_t w, uint32_t h, int32_t x, int32_t y) {
    VkRect2D v{
        .offset =
            {
                .x = x,
                .y = y,
            },
        .extent =
            {
                .width = w,
                .height = h,

            },

    };
    if (w == 0) {
        v.extent.width = m_window_size.x;
    }
    if (h == 0) {
        v.extent.height = m_window_size.y;
    }
    vkCmdSetScissor(m_device.cmd(), 0, 1, &v);
}

void RendererManager::bindPipeline(
    cg::engine::backend::GraphicsPipeline &pipeline) {
    if (*pipeline != VK_NULL_HANDLE) {
        vkCmdBindPipeline(m_device.cmd(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                          *pipeline);
    }
}

void RendererManager::bindVertex(cg::engine::backend::Buffer &buffer) {
    VkBuffer buffers = buffer.buffer;
    VkDeviceSize offsets = 0;
    vkCmdBindVertexBuffers(m_device.cmd(), 0, 1, &buffers, &offsets);
}

void RendererManager::draw(uint32_t count) {
    vkCmdDraw(m_device.cmd(), count, 1, 0, 0); // TODO test for now
}

void RendererManager::bindIndex(cg::engine::backend::Buffer &buffer) {
    // use 32
    vkCmdBindIndexBuffer(m_device.cmd(), buffer.buffer, 0,
                         VK_INDEX_TYPE_UINT32);
}

void RendererManager::drawIndex(uint32_t count) {
    vkCmdDrawIndexed(m_device.cmd(), count, 1, 0, 0, 0);
}

void RendererManager::bindDescriptorSet(const VkDescriptorSet &set,
                                        const VkPipelineLayout &layout) {
    vkCmdBindDescriptorSets(m_device.cmd(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                            layout, 0, 1, &set, 0, nullptr);
}

void RendererManager::pushConstant(VkPipelineLayout &layout,
                                   VkShaderStageFlags stage, uint32_t offset,
                                   uint32_t size, void *data) {
    vkCmdPushConstants(m_device.cmd(), layout, stage, offset, size, data);
}

void RendererManager::drawBase() {
    auto it = m_container.find("base pipeline");
    if (it != m_container.end()) {
        auto &pipeline = it->second->pipeline;
        if (**pipeline != VK_NULL_HANDLE) {
            vkCmdBindPipeline(m_device.cmd(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                              **pipeline);
        }
        setViewport();
        setScissor();
        auto &vertex_buffer = it->second->vbuffers;
        auto &index_buffer = it->second->ibuffers;
        if (vertex_buffer) {
            bindVertex(*vertex_buffer);
            if (index_buffer) {
                bindIndex(*index_buffer);
                drawIndex(index_buffer->size);
            } else {
                draw(vertex_buffer->size);
            }
        }
    } else {
        spdlog::warn("base pipeline not found");
    }
}

void RendererManager::drawBaseTexture() {
    auto it = m_container.find("base texture pipeline");
    if (it != m_container.end()) {
        auto &pipeline = it->second->pipeline;
        if (**pipeline != VK_NULL_HANDLE) {
            vkCmdBindPipeline(m_device.cmd(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                              **pipeline);
        }
        setViewport();
        setScissor();
        if (it->second->descriptor) {
            bindDescriptorSet(it->second->descriptor->set(),
                              **it->second->layout);
        }
        auto &vertex_buffer = it->second->vbuffers;
        auto &index_buffer = it->second->ibuffers;
        if (vertex_buffer) {
            bindVertex(*vertex_buffer);
            if (index_buffer) {
                bindIndex(*index_buffer);
                drawIndex(index_buffer->size);
            } else {
                draw(vertex_buffer->size);
            }
        }
    } else {
        spdlog::warn("base texture pipeline not found");
    }
}

void RendererManager::drawBaseTextureArray() {
    auto it = m_container.find("base texture array pipeline");
    if (it != m_container.end()) {
        auto &pipeline = it->second->pipeline;
        if (**pipeline != VK_NULL_HANDLE) {
            vkCmdBindPipeline(m_device.cmd(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                              **pipeline);
        }
        setViewport();
        setScissor();
        if (it->second->descriptor) {
            bindDescriptorSet(it->second->descriptor->set(),
                              **it->second->layout);
        }
        auto &vertex_buffer = it->second->vbuffers;
        auto &index_buffer = it->second->ibuffers;
        if (vertex_buffer) {
            bindVertex(*vertex_buffer);
            if (index_buffer) {
                bindIndex(*index_buffer);
                drawIndex(index_buffer->size);
            } else {
                draw(vertex_buffer->size);
            }
        }
    } else {
        spdlog::warn("base texture pipeline not found");
    }
}
} // namespace cg::engine::backend
