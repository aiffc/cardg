#include "../../../../inc/engine/render/backend/buffer.hpp"
#include "../../../../inc/engine/render/backend/device.hpp"

namespace cg::engine::backend {

Buffer::Buffer(Device &d) : device(d) {}
Buffer::~Buffer() {
    if (*device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(*device);
    }

    if (*device != VK_NULL_HANDLE && buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(*device, buffer, nullptr);
        buffer = VK_NULL_HANDLE;
    }
    if (*device != VK_NULL_HANDLE && memory != VK_NULL_HANDLE) {
        vkFreeMemory(*device, memory, nullptr);
        memory = VK_NULL_HANDLE;
    }
}

void Buffer::bind(VkDeviceSize offset) {
    vkBindBufferMemory(*device, buffer, memory, offset);
}

void *Buffer::map(VkDeviceSize size, VkDeviceSize offset) {
    if (*device != VK_NULL_HANDLE) {
        vkMapMemory(*device, memory, offset, size, 0, &data);
    } else {
        spdlog::error("invalid memory");
    }
    return data;
}

void Buffer::unmap() {
    if (*device != VK_NULL_HANDLE && data != nullptr) {
        vkUnmapMemory(*device, memory);
        data = nullptr;
    } else {
        spdlog::error("invalid memory");
    }
}

void Buffer::flushMapped() {
    if (memory != VK_NULL_HANDLE) {
        VkMappedMemoryRange range{
            .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
            .pNext = nullptr,
            .memory = memory,
            .offset = 0,
            .size = VK_WHOLE_SIZE,
        };
        vkFlushMappedMemoryRanges(*device, 1, &range);
    }
}

void Buffer::copyFrom(const Buffer &src, VkDeviceSize size) {
    if (src.buffer == VK_NULL_HANDLE || src.memory == VK_NULL_HANDLE) {
        spdlog::warn("copy invalid buffer");
        return;
    }
    auto cmd = device.beginTemporaryCommand();
    if (cmd != VK_NULL_HANDLE) {
        VkBufferCopy info{
            .srcOffset = 0,
            .dstOffset = 0,
            .size = size,
        };

        vkCmdCopyBuffer(cmd, src.buffer, buffer, 1, &info);
        device.endTemporaryCommand(cmd);
    }
}

void Buffer::cutFrom(Buffer &src, VkDeviceSize size) {
    if (src.buffer == VK_NULL_HANDLE || src.memory == VK_NULL_HANDLE) {
        spdlog::warn("copy invalid buffer");
        return;
    }
    auto cmd = device.beginTemporaryCommand();
    if (cmd != VK_NULL_HANDLE) {
        VkBufferCopy info{
            .srcOffset = 0,
            .dstOffset = 0,
            .size = size,
        };
        vkCmdCopyBuffer(cmd, src.buffer, buffer, 1, &info);
        device.endTemporaryCommand(cmd);
        vkFreeMemory(*device, src.memory, nullptr);
        vkDestroyBuffer(*device, src.buffer, nullptr);
        src.memory = VK_NULL_HANDLE;
        src.buffer = VK_NULL_HANDLE;
    }
}

} // namespace cg::engine::backend
