#pragma once

#include "vulkan/vulkan_core.h"

namespace cg::engine::backend {

class Device;

struct Buffer {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    void *data = nullptr; // mapped data
    VkDeviceSize size;

    Buffer(Device &d);
    ~Buffer();

    void bind(VkDeviceSize offset = 0);
    void *map(VkDeviceSize size);
    void unmap();
    void copyFrom(const Buffer &src, VkDeviceSize size);
    void cutFrom(Buffer &src, VkDeviceSize size);

  private:
    Device &device;
};

} // namespace cg::engine::backend
