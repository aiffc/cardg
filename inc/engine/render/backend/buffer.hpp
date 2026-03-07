#pragma once

#include "vulkan/vulkan_core.h"

namespace cg::engine::backend {

class Device;

struct Buffer {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    void *data = nullptr; // mapped data
    uint32_t size;        // for vertetx and index draw
    VkDeviceSize ranges = 0;
    // size_t aligment = 0; // for dynamic uniform buffer

    Buffer(Device &d);
    ~Buffer();

    void bind(VkDeviceSize offset = 0);
    void *map(VkDeviceSize size, VkDeviceSize offset = 0);
    void unmap();
    void flushMapped();
    void copyFrom(const Buffer &src, VkDeviceSize size);
    void cutFrom(Buffer &src, VkDeviceSize size);

  private:
    Device &device;
};

} // namespace cg::engine::backend
