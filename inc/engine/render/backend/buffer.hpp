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

    /**
     * @brief
     * buffer bind memory
     *
     * @param [offset] offset
     */
    void bind(VkDeviceSize offset = 0);
    /**
     * @brief
     * map buffer memory
     *
     * @param [size] offset
     * @param [offset] offset
     */
    void *map(VkDeviceSize size, VkDeviceSize offset = 0);
    /**
     * @brief
     * unmap buffer memory
     */
    void unmap();
    /**
     * @brief
     * flush buffer memory
     */
    void flushMapped();
    /**
     * @brief
     * copy data from other
     *
     * @param [src] copy data from src to this
     * @param [size] the size you want copy
     */
    void copyFrom(const Buffer &src, VkDeviceSize size);
    /**
     * @brief
     * cut data from other, destroy src
     *
     * @param [src] cut data from src to this
     * @param [size] the size you want copy
     */
    void cutFrom(Buffer &src, VkDeviceSize size);

  private:
    Device &device;
};

} // namespace cg::engine::backend
