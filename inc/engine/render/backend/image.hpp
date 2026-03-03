#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace cg::engine::backend {
class Device;

struct Image {
    VkImage image = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;

    Image(const VkDevice &device, VkImage from, bool is_swapchain = false);
    Image(const VkDevice &device);
    ~Image();

    bool init(VkFormat fomrat);
    void destroy();

  private:
    const VkDevice &main_device;
    bool is_swapchain_image;
};

struct Texture {
    VkImage image = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkSampler sampler = VK_NULL_HANDLE;

    Texture(Device &device);
    ~Texture();

    bool init(VkFormat format = VK_FORMAT_R8G8B8A8_SRGB);

    void copyFrom(VkBuffer &buffer, glm::ivec2 size);

  private:
    Device &main_device;
};

} // namespace cg::engine::backend
