#pragma once

#include "image.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace cg::engine::backend {

class Swapchain {
  private:
    Device &m_device;
    VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
    std::vector<std::unique_ptr<Image>> m_swapchain_images;
    uint32_t m_current_index;
    // multiple sample
    std::unique_ptr<Image> m_color_image;
    VkDeviceMemory m_color_memory = VK_NULL_HANDLE;

  public:
    Swapchain(Device &device);
    ~Swapchain();

    VkSwapchainKHR &operator*() { return m_swapchain; }
    bool init(const glm::ivec2 &window_size);

    VkResult acquireNext();
    VkImage &currentImage() const {
        return m_swapchain_images[m_current_index]->image;
    }
    VkImageView &currentView() const {
        return m_swapchain_images[m_current_index]->view;
    }
    uint32_t currentIndex() const { return m_current_index; }

    VkImage &colorImage() const { return m_color_image->image; }
    VkImageView &colorView() const { return m_color_image->view; }

    Swapchain(Swapchain &) = delete;
    Swapchain(Swapchain &&) = delete;
    Swapchain &operator=(Swapchain &) = delete;
    Swapchain &operator=(Swapchain &&) = delete;
};

} // namespace cg::engine::backend
