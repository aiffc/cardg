#include "../../../../inc/engine/render/backend/font.hpp"
#include <algorithm>
#include <cstring>
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
        buffer = nullptr;
    }
    if (font) {
        hb_font_destroy(font);
        font = nullptr;
    }
    if (face) {
        FT_Done_Face(face);
        face = nullptr;
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

uint8_t *FontFace::createStr(std::string_view str, const glm::ivec2 &size) {
    // TODO calc scale for font with texture
    if (buffer) {
        hb_buffer_destroy(buffer);
        buffer = nullptr;
    }

    buffer = hb_buffer_create();
    hb_buffer_add_utf8(buffer, str.data(), -1, 0, -1);
    hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
    hb_buffer_set_script(buffer, HB_SCRIPT_HAN);
    hb_buffer_set_language(buffer, hb_language_from_string("zh", -1));
    // std::vector<hb_feature_t> features;
    hb_shape(font, buffer, nullptr, 0);

    uint32_t count;
    hb_glyph_info_t *glyph_infos = hb_buffer_get_glyph_infos(buffer, &count);
    hb_glyph_position_t *glyph_positions =
        hb_buffer_get_glyph_positions(buffer, &count);

    uint32_t base_line = face->size->metrics.ascender >> 6;
    uint8_t *ret = new uint8_t[size.x * size.y];
    memset(ret, 0, size.x * size.y);
    glm::ivec2 cursor = {0, 0};
    glm::ivec2 offset = {0, 0};
    glm::ivec2 advance = {0, 0};
    for (uint32_t i = 0; i < count; ++i) {
        hb_codepoint_t id = glyph_infos[i].codepoint;
        offset.x = glyph_positions[i].x_offset >> 6;
        offset.y = glyph_positions[i].y_offset >> 6;
        advance.x = glyph_positions[i].x_advance >> 6;
        advance.y = glyph_positions[i].y_advance >> 6;
        FT_Error error = FT_Load_Glyph(face, id, FT_LOAD_DEFAULT);
        if (error) {
            spdlog::error("failed to load code point {}", id);
            delete[] ret;
            if (buffer) {
                hb_buffer_destroy(buffer);
                buffer = nullptr;
            }
            return nullptr;
        }
        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        if (error) {
            spdlog::error("failed to render code point {}", id);
            delete[] ret;
            if (buffer) {
                hb_buffer_destroy(buffer);
                buffer = nullptr;
            }
            return nullptr;
        }

        for (uint32_t j = 0; j < face->glyph->bitmap.rows; ++j) {
            memcpy(ret +
                       (cursor.y + j + offset.y +
                        (base_line - face->glyph->bitmap_top)) *
                           size.x +
                       cursor.x + offset.x,
                   (uint8_t *)face->glyph->bitmap.buffer +
                       j * face->glyph->bitmap.width,
                   face->glyph->bitmap.width);
        }

        cursor += advance;
    }
    if (buffer) {
        hb_buffer_destroy(buffer);
        buffer = nullptr;
    }
    return ret;
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

uint8_t *Font::loadStr(std::string_view key, std::string_view str,
                       const glm::ivec2 &size) {
    if (auto it = m_faces.find(std::string(key)); it != m_faces.end()) {
        uint8_t *ret = it->second->createStr(str, size);
        return ret;
    } else {
        return nullptr;
    }
}
} // namespace cg::engine::backend
