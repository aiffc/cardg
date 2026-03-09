#include "../../../../inc/engine/render/backend/font.hpp"
#include <algorithm>
#include <freetype/freetype.h>
#include <hb.h>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

namespace cg::engine::backend {

FontFace::FontFace(FT_Face &f, const FontSize &s) : face(f), size(s) {}
FontFace::~FontFace() {
    if (buffer) {
        hb_buffer_destroy(buffer);
    }
    if (font) {
        hb_font_destroy(font);
    }

    if (face) {
        FT_Done_Face(face);
    }
}

bool FontFace::init() {
    font = hb_ft_font_create(face, NULL);
    if (!font) {
        return false;
    }
    buffer = hb_buffer_create();
    if (!buffer) {
        return false;
    }

    return true;
}

void FontFace::addStr(std::string_view str) {
    if (buffer) {
        hb_buffer_destroy(buffer);
    }
    hb_buffer_add_utf8(buffer, str.data(), -1, 0, -1);
    hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
    hb_buffer_set_script(buffer, HB_SCRIPT_HAN);
    hb_buffer_set_language(buffer, hb_language_from_string("zh", -1));
    std::vector<hb_feature_t> features;
    hb_shape(font, buffer, features.data(), features.size());

    uint32_t count;
    hb_glyph_info_t *glyph_infos = hb_buffer_get_glyph_infos(buffer, &count);
    hb_glyph_position_t *glyph_positions =
        hb_buffer_get_glyph_positions(buffer, &count);
    glm::ivec2 pos = {0, 0};
    for (uint32_t i = 0; i < count; ++i) {
        unsigned int cluster = glyph_infos[i].cluster;
        FT_Load_Char(face, cluster, FT_LOAD_RENDER);
        pos.x += glyph_positions[i].x_offset;
        pos.y += glyph_positions[i].y_offset;
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
        auto ptr = std::make_unique<FontFace>(face, size);
        ptr->init();
        m_faces.emplace(std::string(path), std::move(ptr));
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
