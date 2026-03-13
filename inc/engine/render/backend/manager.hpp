#pragma once

#include "buffer.hpp"
#include "descriptor.hpp"
#include "graphics_pipeline.hpp"
#include "image.hpp"
#include "layout.hpp"
#include <SDL3/SDL_stdinc.h>
#include <cstring>
#include <freetype/fttypes.h>
#include <glm/glm.hpp>
#include <memory>
#include <spdlog/spdlog.h>
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

struct BaseTextureArrayDU {
    glm::vec2 offset;
};

struct FontU {
    glm::vec4 color;
};

struct MouseVertex {
    glm::vec2 pos;
    glm::vec2 coord;
};
struct CommonUniform {
    alignas(4) float mouse_status;
    alignas(8) glm::vec2 window_size;
    alignas(8) glm::vec2 mouse_scale;
    alignas(8) glm::vec2 mouse_pos;
};

} // namespace cg::engine::buffer

namespace cg::engine::backend {
struct FontSize;
class Device;

struct ManagerHashContainer {
    std::unique_ptr<Descriptor> descriptor;
    std::unique_ptr<Layout> layout;
    std::unique_ptr<GraphicsPipeline> pipeline;
    std::unique_ptr<Buffer> vbuffers;
    std::unique_ptr<Buffer> ibuffers;
    std::unique_ptr<Buffer> uniforms;
    std::unique_ptr<Buffer> duniforms;
    std::unique_ptr<Texture> texture;
    ManagerHashContainer();
    ~ManagerHashContainer();
};

enum class PipelineType {
    Base,
    BaseTexture,
    BaseTextureArray,
    Mouse,
    Font,
};

static inline std::string dumpPipelineName(const PipelineType &p) {
    switch (p) {
    case PipelineType::Base:
        return "Base";
    case PipelineType::BaseTexture:
        return "Base Texture";
    case PipelineType::BaseTextureArray:
        return "Base Texture Array";
    case PipelineType::Mouse:
        return "Mouse";
    default:;
        return "unknow";
    }
}

struct ManagerHashFun {
    template <typename T> size_t operator()(const T &t) const {
        return static_cast<size_t>(t);
    }
};

class Font;

class RendererManager final {
    friend class cg::engine::Renderer;

  private:
    Device &m_device;
    glm::vec2 m_window_size;
    std::unordered_map<PipelineType, std::unique_ptr<ManagerHashContainer>,
                       ManagerHashFun>
        m_container;
    std::unique_ptr<cg::engine::backend::Font> m_font;

  private:
    [[nodiscard]] bool initBasePipeline(const glm::vec2 &size);
    [[nodiscard]] bool initBaseTexturePipeline(const glm::vec2 &size);
    [[nodiscard]] bool initBaseTextureArrayPipeline(const glm::vec2 &size);
    [[nodiscard]] bool initFont(const glm::vec2 &size);
    [[nodiscard]] bool initMouse(const glm::vec2 &size);
    [[nodiscard]] bool init(const glm::vec2 &size);
    void resize(const glm::vec2 &v) { m_window_size = v; }

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
                           const VkPipelineLayout &layout,
                           std::vector<uint32_t> dynamic_offsets = {});
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
                    std::string_view texture_path, uint32_t binding = 0);
    void addTextureArray(const PipelineType &pipeline_name,
                         const std::vector<std::string_view> &texture_paths,
                         uint32_t binding = 0);
    void addTextureArray(const PipelineType &pipeline_name,
                         std::string_view texture_path, const glm::ivec2 &size,
                         uint32_t binding = 0);
    void addCharacters(const PipelineType &pipeline_name,
                       std::string_view ttf_path, const FontSize &size,
                       FT_ULong c, uint32_t binding = 0);
    void addText(const PipelineType &pipeline_name, std::string_view ttf_path,
                 std::string_view str, const glm::ivec2 &text_size = {500, 500},
                 uint32_t line_max = 10, uint32_t column_offset = 10,
                 uint32_t row_offset = 10, uint32_t column_interval = 5,
                 uint32_t row_interval = 5, uint32_t binding = 0);
    template <typename T>
    void mapUniform(const PipelineType &pipeline_name, const T &data) {
        if (auto it = m_container.find(pipeline_name);
            it != m_container.end()) {
            auto &buffer = it->second->uniforms;
            if (buffer) {
                memcpy(buffer->data, &data, sizeof(T));
                // buffer->flushMapped();
            } else {
                spdlog::warn("failed to map uniform buffer for pipeline {}",
                             dumpPipelineName(pipeline_name));
            }

        } else {
            spdlog::warn("pipeline {} not found",
                         dumpPipelineName(pipeline_name));
        }
    }

    template <typename T>
    void mapDynamicUniform(const PipelineType &pipeline_name,
                           const std::vector<T> &datas) {
        if (auto it = m_container.find(pipeline_name);
            it != m_container.end()) {
            auto &buffer = it->second->duniforms;
            if (buffer) {
                for (size_t i = 0; i < datas.size(); ++i) {
                    memcpy((uint8_t *)buffer->data + i * buffer->ranges,
                           &datas[i], sizeof(T));
                }
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

    void drawBase();
    void drawBaseTexture();
    void drawBaseTextureArray();
    void drawFont();
    void drawMouse();

    RendererManager(RendererManager &) = delete;
    RendererManager(RendererManager &&) = delete;
    RendererManager &operator=(RendererManager &) = delete;
    RendererManager &operator=(RendererManager &&) = delete;
};

} // namespace cg::engine::backend
