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

struct FontStr {
    uint32_t w;
    uint32_t h;
    uint8_t *buff;
};

struct FontFace {
    FT_Face face;
    FontSize size;
    hb_font_t *font = nullptr;
    hb_buffer_t *buffer = nullptr;

    FontFace(FT_Face &f, const FontSize &s);
    ~FontFace();
    bool init();
    void addStr(std::string_view str);
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

    Font(Font &) = delete;
    Font(Font &&) = delete;
    Font &operator=(Font &) = delete;
    Font &operator=(Font &&) = delete;
};

} // namespace cg::engine::backend
