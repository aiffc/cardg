#include "../../../inc/engine/render/renderer.hpp"
#include "../../../inc/engine/render/backend/buffer.hpp"
#include "../../../inc/engine/render/backend/device.hpp"
#include "../../../inc/engine/render/backend/graphics_pipeline.hpp"
#include "../../../inc/engine/render/backend/manager.hpp"
#include "../../../inc/engine/render/backend/swapchain.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>
#include <algorithm>
#include <memory>
#include <spdlog/spdlog.h>

namespace cg::engine {
const std::vector<char const *> validation_layers = {
    "VK_LAYER_KHRONOS_validation",
};

Renderer::Renderer(const glm::ivec2 &size) : m_window_size(size) {
    if (m_debug) {
        spdlog::set_level(spdlog::level::info);
    }
}
Renderer::~Renderer() { quit(); }

void Renderer::updateWindowSize() {
    m_device->updateWindowSize();
    SDL_GetWindowSize(m_window.get(), &m_window_size.x, &m_window_size.y);
    m_window_size.x =
        std::clamp((uint32_t)m_window_size.x,
                   m_device->m_phy_info.capabilities.maxImageExtent.width,
                   m_device->m_phy_info.capabilities.minImageExtent.width);
    m_window_size.y =
        std::clamp((uint32_t)m_window_size.y,
                   m_device->m_phy_info.capabilities.maxImageExtent.height,
                   m_device->m_phy_info.capabilities.minImageExtent.height);
    m_manager->resize({m_window_size.x, m_window_size.y});
}

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType [[maybe_unused]],
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData [[maybe_unused]]) {
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        spdlog::trace("vk dbg {}", pCallbackData->pMessage);
    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        spdlog::info("vk dbg {}", pCallbackData->pMessage);
    } else if (messageSeverity &
               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        spdlog::warn("vk dbg {}", pCallbackData->pMessage);
    } else if (messageSeverity &
               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        spdlog::error("vk dbg {}", pCallbackData->pMessage);
    }
    // spdlog::error("vk dbg {}", pCallbackData->pMessage);
    return VK_FALSE;
}

bool Renderer::initInstance() {
    // check layer
    uint32_t support_layer_count = 0;
    std::vector<VkLayerProperties> support_layers;
    if (VK_SUCCESS ==
        vkEnumerateInstanceLayerProperties(&support_layer_count, nullptr)) {
        support_layers.resize(support_layer_count);
        if (VK_SUCCESS != vkEnumerateInstanceLayerProperties(
                              &support_layer_count, support_layers.data())) {
            spdlog::error("failed to enumerate instance layer");
            return false;
        }
    } else {
        spdlog::error("failed to enumerate instance layer");
        return false;
    }
    // dump all supported layer
    // for (auto &support_layer : support_layers) {
    //     spdlog::info("{}", support_layer.layerName);
    // }
    std::vector<char const *> required_layers;
    if (m_debug) {
        required_layers.assign(validation_layers.begin(),
                               validation_layers.end());
        for (const auto &required_layer : required_layers) {
            if (std::ranges::none_of(
                    support_layers,
                    [required_layer](const auto &support_layer) {
                        return !strncmp(support_layer.layerName, required_layer,
                                        strlen(required_layer));
                    })) {
                spdlog::error("{} layer not support", required_layer);
                return false;
            }
        }
    }

    // check extension
    uint32_t sdl_extension_count = 0;
    auto sdl_extensions =
        SDL_Vulkan_GetInstanceExtensions(&sdl_extension_count);
    // required extensions
    std::vector<char const *> required_extensions(
        sdl_extensions, sdl_extensions + sdl_extension_count);
    if (m_debug) {
        required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    // enabled extensions
    uint32_t support_extension_count = 0;
    std::vector<VkExtensionProperties> support_extensions;
    if (VK_SUCCESS == vkEnumerateInstanceExtensionProperties(
                          nullptr, &support_extension_count, nullptr)) {
        support_extensions.resize(support_extension_count);
        if (VK_SUCCESS !=
            vkEnumerateInstanceExtensionProperties(
                nullptr, &support_extension_count, support_extensions.data())) {
            spdlog::error("failed to enumerate instance extensions");
            return false;
        }
    } else {
        spdlog::error("failed to enumerate instance extensions");
        return false;
    }
    // dump all supported extension
    // for (auto &support_extension : support_extensions) {
    //     spdlog::info("{}", support_extension.extensionName);
    // }
    // check all required extension support
    for (const auto &extension : required_extensions) {
        if (std::ranges::none_of(
                support_extensions, [extension](const auto &support_extension) {
                    return !strncmp(support_extension.extensionName, extension,
                                    strlen(extension));
                })) {
            spdlog::error("{} extension not support", extension);
            return false;
        }
    }

    VkApplicationInfo app_info{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "vbr",
        .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
        .pEngineName = "",
        .engineVersion = VK_MAKE_VERSION(0, 0, 0),
        .apiVersion = VK_API_VERSION_1_3,
    };

    VkInstanceCreateInfo info{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = static_cast<uint32_t>(required_layers.size()),
        .ppEnabledLayerNames = required_layers.data(),
        .enabledExtensionCount =
            static_cast<uint32_t>(required_extensions.size()),
        .ppEnabledExtensionNames = required_extensions.data(),
    };

    VkDebugUtilsMessengerCreateInfoEXT dbg_message_info{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debugCallback,
        .pUserData = nullptr,
    };

    if (m_debug) {
        info.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&dbg_message_info;
    }

    if (VK_SUCCESS != vkCreateInstance(&info, nullptr, &m_instance)) {
        spdlog::error("failed to create vulkan instance");
        return false;
    }

    if (m_debug) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            m_instance, "vkCreateDebugUtilsMessengerEXT");
        if (func) {
            if (VK_SUCCESS !=
                func(m_instance, &dbg_message_info, nullptr, &m_dbg_messager)) {
                spdlog::error("failed to create debug messager");
                return false;
            }
        }
    }

    return true;
}

bool Renderer::initSurface() {
    return SDL_Vulkan_CreateSurface(m_window.get(), m_instance, nullptr,
                                    &m_surface);
}

bool Renderer::init(VkSampleCountFlagBits sample_count [[maybe_unused]]) {
    if (!SDL_Vulkan_LoadLibrary(nullptr)) {
        spdlog::error("sdl load vulkan library failed {}", SDL_GetError());
    } else {
        SDL_Vulkan_UnloadLibrary();
    }
    SDL_Window *raw_window = SDL_CreateWindow(
        "vbr", m_window_size.x, m_window_size.y, SDL_WINDOW_VULKAN);

    if (raw_window == nullptr) {
        spdlog::error("sdl create window failed {}", SDL_GetError());
        return false;
    }

    m_window = std::unique_ptr<SDL_Window, WindowDeleter>(raw_window);

    if (!initInstance()) {
        return false;
    }
    if (!initSurface()) {
        spdlog::error("sdl init vulkan surface failed {}", SDL_GetError());
        return false;
    }

    m_device = std::make_unique<cg::engine::backend::Device>(
        m_surface, sample_count, m_debug);
    if (!m_device->init(m_instance)) {
        spdlog::error("unable to create logic device");
        return false;
    }

    m_swapchain = std::make_unique<cg::engine::backend::Swapchain>(*m_device);
    if (!m_swapchain->init(m_window_size)) {
        spdlog::error("unable to create swapchain");
        return false;
    }

    m_manager =
        std::make_unique<cg::engine::backend::RendererManager>(*m_device);
    if (!m_manager->init(m_window_size)) {
        spdlog::error("unable to init manager");
        return false;
    }
    spdlog::info("app init done");
    return true;
}

void Renderer::quit() {
    if (m_manager) {
        if (m_device) {
            m_device->waitIdle();
        }
        m_manager.reset();
    }
    if (m_swapchain) {
        if (m_device) {
            m_device->waitIdle();
        }
        m_swapchain.reset();
    }
    if (m_device) {
        m_device->waitIdle();
        m_device.reset();
    }
    if (m_surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        m_surface = VK_NULL_HANDLE;
    }

    if (m_debug && m_dbg_messager != VK_NULL_HANDLE) {
        auto fun = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            m_instance, "vkDestroyDebugUtilsMessengerEXT");
        if (fun) {
            fun(m_instance, m_dbg_messager, nullptr);
        }
        m_dbg_messager = VK_NULL_HANDLE;
    }

    if (m_instance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = VK_NULL_HANDLE;
    }
    if (m_window) {
        m_window.reset();
    }
    spdlog::info("app quit done");
}

bool Renderer::begin(float r, float g, float b, float a) {
    if (VK_SUCCESS != vkWaitForFences(**m_device, 1, &m_device->inFlightFence(),
                                      VK_TRUE, UINT64_MAX)) {
        spdlog::warn("fence timeout");
        return false;
    }

    VkResult acquire_ret = m_swapchain->acquireNext();
    if (acquire_ret == VK_ERROR_OUT_OF_DATE_KHR) {
        spdlog::info("recreate swapchain");
        updateWindowSize();
        if (!m_swapchain->init(m_window_size)) {
            spdlog::error("failed to recreate swapchain");
            return false;
        }
        return false;
    } else if (VK_SUCCESS != acquire_ret && VK_SUBOPTIMAL_KHR != acquire_ret) {
        spdlog::warn("failed to get current image index");
        return false;
    }
    vkResetFences(**m_device, 1, &m_device->inFlightFence());
    vkResetCommandBuffer(m_device->cmd(), 0);

    VkCommandBufferBeginInfo info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pInheritanceInfo = nullptr,
    };
    if (VK_SUCCESS != vkBeginCommandBuffer(m_device->cmd(), &info)) {
        return false;
    }

    backend::imageLayoutTrans(m_device->cmd(), m_swapchain->currentImage(),
                              VK_IMAGE_LAYOUT_UNDEFINED,
                              VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    VkRenderingAttachmentInfo attachment_info{
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR,
        .pNext = nullptr,
        .imageView = m_swapchain->currentView(),
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .resolveMode = VK_RESOLVE_MODE_NONE,
        .resolveImageView = nullptr,
        .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue =
            {
                .color =
                    {
                        .float32 = {r, g, b, a},
                    },
            },
    };

    if (m_device->sampleCount() != VK_SAMPLE_COUNT_1_BIT &&
        m_swapchain->colorView() != VK_NULL_HANDLE) {
        attachment_info.imageView = m_swapchain->colorView();
        attachment_info.resolveMode = VK_RESOLVE_MODE_AVERAGE_BIT;
        attachment_info.resolveImageView = m_swapchain->currentView();
        attachment_info.resolveImageLayout =
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    }

    VkRenderingInfo rinfo{
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .pNext = nullptr,
        .flags = 0,
        .renderArea =
            {
                .offset =
                    {
                        .x = 0,
                        .y = 0,
                    },
                .extent =
                    {
                        .width = static_cast<uint32_t>(m_window_size.x),
                        .height = static_cast<uint32_t>(m_window_size.y),
                    },

            },
        .layerCount = 1,
        .viewMask = 0,
        .colorAttachmentCount = 1,
        .pColorAttachments = &attachment_info,
        .pDepthAttachment = nullptr,
        .pStencilAttachment = nullptr,
    };
    vkCmdBeginRendering(m_device->cmd(), &rinfo);

    return true;
}

bool Renderer::end() {
    vkCmdEndRendering(m_device->cmd());

    backend::imageLayoutTrans(m_device->cmd(), m_swapchain->currentImage(),
                              VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                              VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    if (VK_SUCCESS != vkEndCommandBuffer(m_device->cmd())) {
        return false;
    }

    VkPipelineStageFlags wait_stage =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submit_info{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_device->imageAvailable(),
        .pWaitDstStageMask = &wait_stage,
        .commandBufferCount = 1,
        .pCommandBuffers = &m_device->cmd(),
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &m_device->renderDone(m_swapchain->currentIndex()),
    };
    if (VK_SUCCESS != vkQueueSubmit(m_device->graphicsQueue(), 1, &submit_info,
                                    m_device->inFlightFence())) {
        spdlog::error("failed to submit queue");
        return false;
    }

    uint32_t current_index = m_swapchain->currentIndex();
    VkPresentInfoKHR present_info{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_device->renderDone(m_swapchain->currentIndex()),
        .swapchainCount = 1,
        .pSwapchains = &**m_swapchain,
        .pImageIndices = &current_index,
        .pResults = nullptr,
    };
    VkResult present_ret =
        vkQueuePresentKHR(m_device->presentQueue(), &present_info);
    if (VK_ERROR_OUT_OF_DATE_KHR == present_ret ||
        VK_SUBOPTIMAL_KHR == present_ret) {
        spdlog::info("recreate swapchain");
        updateWindowSize();
        if (!m_swapchain->init(m_window_size)) {
            spdlog::error("failed to recreate swapchain");
            return false;
        }
    } else if (VK_SUCCESS != present_ret) {
        spdlog::error("failed to submit queue");
        return false;
    }
    return true;
};

void Renderer::drawBase() { m_manager->drawBase(); }
void Renderer::drawBaseTexture() { m_manager->drawBaseTexture(); }
void Renderer::drawBaseTextureArray() { m_manager->drawBaseTextureArray(); }

} // namespace cg::engine
