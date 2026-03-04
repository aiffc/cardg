#include "../../../../inc/engine/render/backend/image.hpp"
#include "../../../../inc/engine/render/backend/device.hpp"

namespace cg::engine::backend {

Image::Image(const VkDevice &device, VkImage from, bool is_swapchain)
    : image(from), main_device(device), is_swapchain_image(is_swapchain) {}

Image::Image(const VkDevice &device) : main_device(device) {}

Image::~Image() { destroy(); }

bool Image::init(VkFormat format) {
    if (main_device != VK_NULL_HANDLE && image != VK_NULL_HANDLE) {
        VkImageViewCreateInfo info{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = format,
            .components =
                {
                    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
                },
            .subresourceRange =
                {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
                },
        };
        if (VK_SUCCESS ==
            vkCreateImageView(main_device, &info, nullptr, &view)) {
            return true;
        }
        return false;
    }
    return false;
}

void Image::destroy() {
    if (view != VK_NULL_HANDLE && main_device != VK_NULL_HANDLE) {
        vkDestroyImageView(main_device, view, nullptr);
    }
    if (image != VK_NULL_HANDLE && main_device != VK_NULL_HANDLE &&
        !is_swapchain_image) {
        vkDestroyImage(main_device, image, nullptr);
    }
}

Texture::Texture(Device &device) : main_device(device) {}
Texture::~Texture() {
    if (*main_device != VK_NULL_HANDLE) {
        main_device.waitIdle();
        if (memory != VK_NULL_HANDLE) {
            vkFreeMemory(*main_device, memory, nullptr);
            memory = VK_NULL_HANDLE;
        }
        if (image != VK_NULL_HANDLE) {
            vkDestroyImage(*main_device, image, nullptr);
            image = VK_NULL_HANDLE;
        }
        if (view != VK_NULL_HANDLE) {
            vkDestroyImageView(*main_device, view, nullptr);
            view = VK_NULL_HANDLE;
        }
        if (sampler != VK_NULL_HANDLE) {
            vkDestroySampler(*main_device, sampler, nullptr);
        }
    }
}

bool Texture::init(VkFormat format) {
    if (*main_device != VK_NULL_HANDLE && image != VK_NULL_HANDLE) {
        VkImageViewCreateInfo info{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = format,
            .components =
                {
                    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
                },
            .subresourceRange =
                {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
                },
        };
        if (VK_SUCCESS !=
            vkCreateImageView(*main_device, &info, nullptr, &view)) {
            return false;
        }
        VkSamplerCreateInfo sampler_info{
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .magFilter = VK_FILTER_LINEAR,
            .minFilter = VK_FILTER_LINEAR,
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .mipLodBias = 0.0f,
            .anisotropyEnable = VK_TRUE,
            .maxAnisotropy =
                main_device.propreties().limits.maxSamplerAnisotropy,
            .compareEnable = VK_FALSE,
            .compareOp = VK_COMPARE_OP_ALWAYS,
            .minLod = 0.0f,
            .maxLod = 0.0f,
            .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
            .unnormalizedCoordinates = VK_FALSE,

        };
        if (VK_SUCCESS !=
            vkCreateSampler(*main_device, &sampler_info, nullptr, &sampler)) {
            spdlog::error("can not init sampler, create samper failed");
            return false;
        }
        return true;
    }
    return false;
}

bool Texture::initArray(uint32_t layer_count, VkFormat format) {
    if (*main_device != VK_NULL_HANDLE && image != VK_NULL_HANDLE) {
        VkImageViewCreateInfo info{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY,
            .format = format,
            .components =
                {
                    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
                },
            .subresourceRange =
                {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = layer_count,
                },
        };
        if (VK_SUCCESS !=
            vkCreateImageView(*main_device, &info, nullptr, &view)) {
            return false;
        }
        VkSamplerCreateInfo sampler_info{
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .magFilter = VK_FILTER_LINEAR,
            .minFilter = VK_FILTER_LINEAR,
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .mipLodBias = 0.0f,
            .anisotropyEnable = VK_TRUE,
            .maxAnisotropy =
                main_device.propreties().limits.maxSamplerAnisotropy,
            .compareEnable = VK_FALSE,
            .compareOp = VK_COMPARE_OP_ALWAYS,
            .minLod = 0.0f,
            .maxLod = 0.0f,
            .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
            .unnormalizedCoordinates = VK_FALSE,

        };
        if (VK_SUCCESS !=
            vkCreateSampler(*main_device, &sampler_info, nullptr, &sampler)) {
            spdlog::error("can not init sampler, create samper failed");
            return false;
        }
        return true;
    }
    return false;
}

void Texture::copyFrom(VkBuffer &buffer, glm::ivec2 size) {
    auto cmd = main_device.beginTemporaryCommand();
    VkBufferImageCopy copy_info{
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource =
            {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = 0,
                .baseArrayLayer = 0,
                .layerCount = 1,

            },
        .imageOffset =
            {
                .x = 0,
                .y = 0,
                .z = 0,

            },
        .imageExtent =
            {
                .width = static_cast<uint32_t>(size.x),
                .height = static_cast<uint32_t>(size.y),
                .depth = 1,
            },
    };
    vkCmdCopyBufferToImage(cmd, buffer, image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_info);
    main_device.endTemporaryCommand(cmd);
}

void Texture::copyFrom(VkBuffer &buffer, glm::ivec2 size,
                       uint32_t layer_count) {
    auto cmd = main_device.beginTemporaryCommand();
    std::vector<VkBufferImageCopy> copy_info;

    for (uint32_t i = 0; i < layer_count; ++i) {
        VkBufferImageCopy info{
            .bufferOffset = static_cast<VkDeviceSize>(i * size.x * size.y * 4),
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource =
                {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .mipLevel = 0,
                    .baseArrayLayer = i,
                    .layerCount = 1,

                },
            .imageOffset =
                {
                    .x = 0,
                    .y = 0,
                    .z = 0,

                },
            .imageExtent =
                {
                    .width = static_cast<uint32_t>(size.x),
                    .height = static_cast<uint32_t>(size.y),
                    .depth = 1,
                },
        };
        copy_info.push_back(info);
    }

    vkCmdCopyBufferToImage(
        cmd, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        static_cast<uint32_t>(copy_info.size()), copy_info.data());
    main_device.endTemporaryCommand(cmd);
}

} // namespace cg::engine::backend
