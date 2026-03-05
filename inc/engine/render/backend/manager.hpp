#pragma once

#include "buffer.hpp"
#include "descriptor.hpp"
#include "graphics_pipeline.hpp"
#include "image.hpp"
#include "layout.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cg::engine::buffer {

struct Base {
    glm::vec2 pos;
    glm::vec3 color;
};

struct BaseTexture {
    glm::vec2 pos;
    glm::vec2 coord;
};

struct BaseTextureArrayU {
    float index;
};
} // namespace cg::engine::buffer

namespace cg::engine::backend {

class Device;

struct ManagerHashContainer {
    std::unique_ptr<Descriptor> descriptor;
    std::unique_ptr<Layout> layout;
    std::unique_ptr<GraphicsPipeline> pipeline;
    std::unique_ptr<Buffer> vbuffers;
    std::unique_ptr<Buffer> ibuffers;
    std::unique_ptr<Buffer> uniforms;
    std::unique_ptr<Texture> texture;
    ManagerHashContainer();
    ~ManagerHashContainer();
};

enum class PipelineType {
    Base,
    BaseTexture,
    BaseTextureArray,
};

static inline std::string dumpPipelineName(const PipelineType &p) {
    switch (p) {
    case PipelineType::Base:
        return "Base";
    case PipelineType::BaseTexture:
        return "Base Texture";
    case PipelineType::BaseTextureArray:
        return "Base Texture Array";
    default:;
        return "unknow";
    }
}

struct ManagerHashFun {
    template <typename T> size_t operator()(const T &t) const {
        return static_cast<size_t>(t);
    }
};

class RendererManager final {
    friend class cg::engine::Renderer;

  private:
    Device &m_device;
    glm::vec2 m_window_size;
    std::unordered_map<PipelineType, std::unique_ptr<ManagerHashContainer>,
                       ManagerHashFun>
        m_container;

  private:
    [[nodiscard]] bool initBasePipeline(const glm::vec2 &size);
    [[nodiscard]] bool initBaseTexturePipeline(const glm::vec2 &size);
    [[nodiscard]] bool initBaseTextureArrayPipeline(const glm::vec2 &size);
    [[nodiscard]] bool init(const glm::vec2 &size);
    void resize(const glm::vec2 &v) { m_window_size = v; }

    void drawBase();
    void drawBaseTexture();
    void drawBaseTextureArray();

  private:
    void setViewport(float w = 0.0f, float h = 0.0f, float x = 0.0f,
                     float y = 0.0f, float min = 0.0f, float max = 1.0f);
    void setScissor(uint32_t w = 0, uint32_t h = 0, int32_t x = 0,
                    int32_t y = 0);
    void bindPipeline(cg::engine::backend::GraphicsPipeline &pipeline);
    void bindVertex(cg::engine::backend::Buffer &buffer);
    void draw(uint32_t count);
    void bindIndex(cg::engine::backend::Buffer &buffer);
    void drawIndex(uint32_t count);
    void bindDescriptorSet(const VkDescriptorSet &set,
                           const VkPipelineLayout &layout);
    void pushConstant(VkPipelineLayout &layout, VkShaderStageFlags stage,
                      uint32_t offset, uint32_t size, void *data);

  public:
    RendererManager(Device &device);
    ~RendererManager();

    template <typename T>
    void addVertexBuffer(const PipelineType &pipeline_name,
                         const std::vector<T> &data) {
        auto it = m_container.find(pipeline_name);
        if (it != m_container.end()) {
            auto buff = m_device.createUsageBuffer<T>(
                data, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            if (buff) {
                it->second->vbuffers = std::move(buff);
            } else {
                spdlog::warn("failed to create vertex buffer for {}",
                             dumpPipelineName(pipeline_name));
            }

        } else {
            spdlog::warn("pipeline {} not found",
                         dumpPipelineName(pipeline_name));
        }
    }
    void addIndexBuffer(const PipelineType &pipeline_name,
                        const std::vector<uint32_t> &data);
    void addTexture(const PipelineType &pipeline_name,
                    std::string_view texture_path);
    void addTextureArray(const PipelineType &pipeline_name,
                         const std::vector<std::string_view> &texture_paths);
    template <typename T>
    void mapUniform(const PipelineType &pipeline_name, const T &data) {
        auto it = m_container.find(pipeline_name);
        if (it != m_container.end()) {
            auto &buffer = it->second->uniforms;
            if (buffer) {
                memcpy(buffer->data, &data,
                       sizeof(cg::engine::buffer::BaseTextureArrayU));
                buffer->flushMapped();
            } else {
                spdlog::warn("failed to map uniform buffer for pipeline {}",
                             dumpPipelineName(pipeline_name));
            }

        } else {
            spdlog::warn("pipeline {} not found",
                         dumpPipelineName(pipeline_name));
        }
    }

    RendererManager(RendererManager &) = delete;
    RendererManager(RendererManager &&) = delete;
    RendererManager &operator=(RendererManager &) = delete;
    RendererManager &operator=(RendererManager &&) = delete;
};

} // namespace cg::engine::backend
