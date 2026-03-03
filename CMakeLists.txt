cmake_minimum_required(VERSION 3.14.0)

project(game VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

set(TARGET ${PROJECT_NAME})

find_package(SDL3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)
find_package(glm REQUIRED)

set(SOURCE_FILES
  src/engine/core/app.cpp
  src/engine/core/time.cpp
  src/engine/render/backend/util.cpp
  src/engine/render/backend/buffer.cpp
  src/engine/render/backend/device.cpp
  src/engine/render/backend/image.cpp
  src/engine/render/backend/swapchain.cpp
  src/engine/render/backend/layout.cpp
  src/engine/render/backend/descriptor.cpp
  src/engine/render/backend/graphics_pipeline.cpp
  src/engine/render/renderer.cpp
  src/engine/input/manager.cpp
  src/engine/scene/scene.cpp
  src/engine/scene/manager.cpp
  src/card.cpp
  src/character.cpp
  src/factory.cpp
  src/map.cpp
  tests/test_scene.cpp
  tests/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC
  ${SDL3_LIBRARIES}
  ${Vulkan_LIBRARIES}
  spdlog::spdlog
  glm::glm
)
