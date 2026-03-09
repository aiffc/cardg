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

namespace cg::engine::backend {

class RendererManager;

struct FontSize {
    FT_UInt w;
    FT_UInt h;
};

struct FontFace {
    FT_Face face;
    FontSize size;

    FontFace(FT_Face &f, const FontSize &s);
    ~FontFace();
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

    std::optional<FT_GlyphSlot> loadChar(std::string_view key,
                                         FT_ULong char_code);

    Font(Font &) = delete;
    Font(Font &&) = delete;
    Font &operator=(Font &) = delete;
    Font &operator=(Font &&) = delete;
};

} // namespace cg::engine::backend
