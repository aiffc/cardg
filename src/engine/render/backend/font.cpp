#include "../../../../inc/engine/render/backend/font.hpp"
#include <freetype/freetype.h>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>

namespace cg::engine::backend {

FontFace::FontFace(FT_Face &f, const FontSize &s) : face(f), size(s) {}
FontFace::~FontFace() {
    if (face) {
        FT_Done_Face(face);
    }
}

Font::Font() = default;
Font::~Font() {
    if (!m_faces.empty()) {
        m_faces.clear();
    }
    FT_Done_FreeType(m_ft);
    spdlog::info("font quit done");
};

bool Font::init() {
    FT_Error error;
    error = FT_Init_FreeType(&m_ft);

    if (error) {
        spdlog::error("init freetype failed");
        return false;
    }
    return true;
}

void Font::addFace(std::string_view path, const FontSize &size) {
    if (auto it = m_faces.find(std::string(path)); it != m_faces.end()) {
        it->second->size = size;
    } else {
        FT_Face face;
        FT_Error error;
        error = FT_New_Face(m_ft, path.data(), 0, &face);
        if (error) {
            spdlog::warn("failed to load ttf file {}", path);
            return;
        }
        error = FT_Set_Pixel_Sizes(face, size.w, size.h);
        if (error) {
            spdlog::warn("failed to set font size {} ", path);
            FT_Done_Face(face);
            return;
        }
        m_faces.emplace(std::string(path),
                        std::make_unique<FontFace>(face, size));
    }
}

void Font::size(std::string_view key, FT_UInt h) {
    FontSize size{0, h};
    if (auto it = m_faces.find(std::string(key)); it != m_faces.end()) {
        it->second->size = size;
        FT_Error error;
        error = FT_Set_Pixel_Sizes(it->second->face, size.w, size.h);
        if (error) {
            spdlog::warn("failed to set font size {} ", key);
        }
    } else {
        addFace(key, size);
    }
}
void Font::size(std::string_view key, const FontSize &size) {
    if (auto it = m_faces.find(std::string(key)); it != m_faces.end()) {
        it->second->size = size;
        FT_Error error;
        error = FT_Set_Pixel_Sizes(it->second->face, size.w, size.h);
        if (error) {
            spdlog::warn("failed to set font size {} ", key);
        }
    } else {
        addFace(key, size);
    }
}
FontSize Font::size(std::string_view key) const {
    if (auto it = m_faces.find(std::string(key)); it != m_faces.end()) {
        return it->second->size;
    } else {
        FontSize size = {0, 0};
        return size;
    }
}

std::optional<const FT_GlyphSlot> Font::loadChar(std::string_view key,
                                                 FT_ULong char_code) {
    if (auto it = m_faces.find(std::string(key)); it != m_faces.end()) {
        FT_Error error =
            FT_Load_Char(it->second->face, char_code, FT_LOAD_RENDER);
        if (error) {
            spdlog::error("failed to load char code {}", char_code);
            return std::nullopt;
        }
        return it->second->face->glyph;
    } else {
        return std::nullopt;
    }
}

} // namespace cg::engine::backend
