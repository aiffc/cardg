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

class RendererManager final {
    friend class cg::engine::Renderer;

  private:
    Device &m_device;
    glm::vec2 m_window_size;
    std::unordered_map<std::string, std::unique_ptr<ManagerHashContainer>>
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

    void addBasePipelineVertexBuffer(
        const std::vector<cg::engine::buffer::Base> &data);
    void addBasePipelineIndexBuffer(const std::vector<uint32_t> &data);

    void addBaseTexturePipelineVertexBuffer(
        const std::vector<cg::engine::buffer::BaseTexture> &data);
    void addBaseTexturePipelineIndexBuffer(const std::vector<uint32_t> &data);
    void addBaseTextureTexture(std::string_view path);

    void addBaseTextureArrayPipelineVertexBuffer(
        const std::vector<cg::engine::buffer::BaseTexture> &data);
    void
    addBaseTextureArrayPipelineIndexBuffer(const std::vector<uint32_t> &data);
    void addBaseTextureArrayTexture(const std::vector<std::string_view> &path);
    void
    mapBaseTextureArrayUniform(cg::engine::buffer::BaseTextureArrayU &index);

    RendererManager(RendererManager &) = delete;
    RendererManager(RendererManager &&) = delete;
    RendererManager &operator=(RendererManager &) = delete;
    RendererManager &operator=(RendererManager &&) = delete;
};

} // namespace cg::engine::backend
