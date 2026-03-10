#pragma once

#include <freetype/fttypes.h>
#include <ft2build.h>
#include <glm/fwd.hpp>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <glm/glm.hpp>
#include <hb-ft.h>
#include <hb.h>

namespace cg::engine::backend {

class RendererManager;

struct FontSize {
    FT_UInt w;
    FT_UInt h;
};

struct FontFormat {
    FontSize size;
    uint32_t left_offset;
    uint32_t right_offset;
    uint32_t top_offset;
    uint32_t botton_offset;
    uint32_t column_interval;
    uint32_t row_interval;
    uint32_t line_max;
};

struct FontFace {
    FT_Face face;
    FontFormat format;
    hb_font_t *font = nullptr;
    hb_buffer_t *buffer = nullptr;

    FontFace(FT_Face &f, const FontFormat &s);
    ~FontFace();
    bool init();
    uint8_t *createStr(std::string_view str, const glm::ivec2 &size);
};

class Font final {
    friend class cg::engine::backend::RendererManager;

  private:
    FT_Library m_ft;
    std::unordered_map<std::string, std::unique_ptr<FontFace>> m_faces;

  private:
    [[nodiscard]] bool init();

  public:
    Font();
    ~Font();

    void addFace(std::string_view path, const FontSize &size);

    void size(std::string_view key, FT_UInt h);
    void size(std::string_view key, const FontSize &size);
    FontSize size(std::string_view key) const;

    std::optional<const FT_GlyphSlot> loadChar(std::string_view key,
                                               FT_ULong char_code);
    uint8_t *loadStr(std::string_view key, std::string_view str,
                     const glm::ivec2 &size);

    Font(Font &) = delete;
    Font(Font &&) = delete;
    Font &operator=(Font &) = delete;
    Font &operator=(Font &&) = delete;
};

} // namespace cg::engine::backend
