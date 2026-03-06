#pragma once

#include "buffer.hpp"
#include "util.hpp"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace cg::engine {
class Renderer;
}

namespace cg::engine::backend {

struct Texture;

struct Queues {
    VkQueue graphics;
    VkQueue present;
    VkQueue transfer;
    VkQueue compute;
};

struct SyncObjs {
    VkSemaphore image_available;
    std::vector<VkSemaphore> render_done;
    VkFence in_flight_fence = VK_NULL_HANDLE;

    void destroy(const VkDevice device);
};

class Device {
    friend class cg::engine::Renderer;
    friend class Swapchain;
    friend class RendererManager;

  private:
    VkSurfaceKHR &m_surface;
    bool m_debug;
    GPUInfo m_phy_info;
    VkPhysicalDevice m_phy_device;
    QueueFamilyIndices m_queue_indices;
    VkDevice m_device;
    Queues m_queues;
    VkCommandPool m_cmd_pool;
    VkCommandBuffer m_cmd;
    SyncObjs m_sync;
    // sample count
    VkSampleCountFlagBits m_sample_count = VK_SAMPLE_COUNT_1_BIT;

  private:
    [[nodiscard]] bool pickupPhyDevice(const VkInstance &instance);
    [[nodiscard]] bool initLogicDevice();
    [[nodiscard]] bool initCmds();
    [[nodiscard]] bool initSync();

    VkFence &inFlightFence() { return m_sync.in_flight_fence; }
    VkSemaphore &imageAvailable() { return m_sync.image_available; }
    VkSemaphore &renderDone(uint32_t i = 0) { return m_sync.render_done[i]; }
    VkCommandBuffer &cmd() { return m_cmd; }
    void updateWindowSize();

    template <typename T> size_t calcDynamicUniformAligment() {
        size_t min_aligment =
            m_phy_info.properties.limits.minUniformBufferOffsetAlignment;
        size_t aligment_size = sizeof(T);
        if (min_aligment > 0) {
            aligment_size =
                (aligment_size + min_aligment - 1) & ~(min_aligment - 1);
        }
        return min_aligment;
    }

  private:
    uint32_t findMemoryType(uint32_t type_filter,
                            VkMemoryPropertyFlags properties);
    std::unique_ptr<Buffer> createBuffer(VkDeviceSize size,
                                         VkBufferUsageFlags usage,
                                         VkMemoryPropertyFlags properties);
    bool internalCreateSampleImage(uint32_t w, uint32_t h, VkFormat format,
                                   VkImageTiling tilling,
                                   VkImageUsageFlags usage,
                                   VkMemoryPropertyFlags properties,
                                   VkImage &image, VkDeviceMemory &memory);
    bool internalCreateImage(uint32_t layer_cout, uint32_t w, uint32_t h,
                             VkFormat format, VkImageTiling tilling,
                             VkImageUsageFlags usage,
                             VkMemoryPropertyFlags properties, VkImage &image,
                             VkDeviceMemory &memory);
    void transitionImageLayout(VkImage &image, VkImageLayout old_layout,
                               VkImageLayout new_layout,
                               uint32_t layer_count = 1);

  public:
    Device(VkSurfaceKHR &surface,
           VkSampleCountFlagBits sample_count = VK_SAMPLE_COUNT_1_BIT,
           bool debug = false);
    ~Device();

    bool init(const VkInstance &instance);
    VkDevice &operator*() { return m_device; }
    VkQueue &graphicsQueue() { return m_queues.graphics; }
    VkQueue &presentQueue() { return m_queues.present; }
    VkQueue &transferQueue() { return m_queues.transfer; }
    VkQueue &computeQueue() { return m_queues.compute; }
    const VkPhysicalDeviceProperties &propreties() const {
        return m_phy_info.properties;
    }
    VkSampleCountFlagBits sampleCount() const { return m_sample_count; }
    void sampleCount(VkSampleCountFlagBits flag) {
        VkSampleCountFlags max_counts =
            m_phy_info.properties.limits.framebufferColorSampleCounts;
        if (max_counts & flag) {
            m_sample_count = flag;
            spdlog::info("set sample bit {}", static_cast<uint32_t>(flag));
            return;
        }
        spdlog::warn("failed to set bit {}", static_cast<uint32_t>(flag));
        m_sample_count = VK_SAMPLE_COUNT_1_BIT;
    }

    VkCommandBuffer beginTemporaryCommand();
    void endTemporaryCommand(VkCommandBuffer &cmd);

    VkFormat format() const { return m_phy_info.surface_format.format; }

    // for vertex & index buffer
    template <typename T>
    std::unique_ptr<Buffer> createUsageBuffer(const std::vector<T> &datas,
                                              VkBufferUsageFlagBits usage) {
        VkDeviceSize total_size = sizeof(T) * datas.size();
        auto stage = createBuffer(total_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        if (!stage) {
            // invalid stage buffer
            return stage;
        }
        void *data = stage->map(total_size);

        memcpy(data, datas.data(), (size_t)total_size);
        stage->unmap();

        auto ret =
            createBuffer(total_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        if (ret) {
            ret->cutFrom(*stage, total_size);
            stage.reset();
            ret->size = static_cast<uint32_t>(datas.size());
        }
        return ret;
    }
    // for uniform buffer
    template <typename T> std::unique_ptr<Buffer> createUniformBuffer() {
        VkDeviceSize size = sizeof(T);
        auto ret = createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (ret) {
            ret->map(size);
            ret->ranges = size;
        }
        return ret;
    }

    template <typename T>
    std::unique_ptr<Buffer> createDynamicUniformBuffer(uint32_t count = 1) {
        size_t aligment = calcDynamicUniformAligment<T>();
        VkDeviceSize size = aligment * count;
        auto ret = createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (ret) {
            ret->map(size);
            ret->ranges = size;
            if (count > 1) {
                ret->aligment = aligment;
            }
        }
        return ret;
    }

    // template <typename DynamicT, typename NormalT>
    // std::unique_ptr<Buffer> createMixUniformBuffer(uint32_t count = 1) {
    //     size_t dynamic_aligment = calcDynamicUniformAligment<DynamicT>();
    //     VkDeviceSize size = dynamic_aligment * count;

    //     size_t normal_aligment = calcDynamicUniformAligment<NormalT>();
    //     size += normal_aligment;

    //     auto ret = createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    //                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
    //                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    //     if (ret) {
    //         ret->map(size);
    //         ret->size = size;
    //         if (count > 1) {
    //             ret->aligment = aligment;
    //         }
    //     }
    //     return ret;
    // }

    std::unique_ptr<Texture> createTexture(std::string_view path);
    std::unique_ptr<Texture>
    createTextureArray(const std::vector<std::string_view> &paths);
    std::unique_ptr<Texture> createTextureArray(std::string_view path,
                                                const glm::ivec2 &size);

    void waitIdle() { vkDeviceWaitIdle(m_device); }

    Device(Device &) = delete;
    Device(Device &&) = delete;
    Device &operator=(Device &) = delete;
    Device &operator=(Device &&) = delete;
};

} // namespace cg::engine::backend
