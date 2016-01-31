#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "Resource/Font.hpp"
#include "Math/Math.hpp"
#include "Core/Logger.hpp"

#include <iostream>
#include <GL/glew.h>

namespace dm {

FT_Library ft;
bool m_ftinitialised = false;

Font::Font() {
    if (!m_ftinitialised) {
        if (FT_Init_FreeType(&ft)) {
            Log::error("Error initializing FreeType2");
        }
    }
}

Font::~Font() {}

bool Font::load(Texture& texture, const char* name, const int size) {
    FT_Face face;

    int atlaswidth = 0, atlasheight = 0;

    m_glyphs.resize(130);

    if (FT_New_Face(ft, name, 0, &face)) {
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, size);

    FT_GlyphSlot g = face->glyph;

    for (uint32_t i = 32; i < 128; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            Log::error("Failed to load character %c", i);
            continue;
        }

        atlaswidth += g->bitmap.width;
        atlasheight = dm::max(atlasheight, (int)g->bitmap.rows);
    }

    atlaswidth += (128 - 32);

    texture.set_size(Vec2i(atlaswidth, atlasheight));
    texture.set_format(Texture::Format::Alpha);
    texture.set_filter(Texture::Filter::Nearest, Texture::Filter::Nearest);
    texture.set_transparent(true);
    texture.upload();

    texture.bind();

    texture.setup();

    unsigned char* data = new unsigned char[atlaswidth * atlasheight];
    memset(data, 0, atlaswidth * atlasheight);
    texture.set_data(data);

    delete data;

    int x = 0;

    for (uint32_t i = 32; i < 128; ++i) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) continue;

        texture.sub_data(g->bitmap.buffer, Vec2i(x, 0),
                         Vec2i(g->bitmap.width, g->bitmap.rows));

        Glyph info;

        /* Characters are inverted in the texture */
        info.crop.position =
            Vec2f(x / (float)atlaswidth,
                  ((float)(g->bitmap.rows + 1) / (float)atlasheight));
        info.crop.size =
            Vec2f((float)g->bitmap.width / (float)atlaswidth,
                  -(((float)g->bitmap.rows + 1) / (float)atlasheight));

        info.vertices.position =
            Vec2f((float)g->bitmap_left,
                  (float)g->bitmap_top - (float)g->bitmap.rows);

        info.vertices.size =
            Vec2f((float)g->bitmap.width, (float)g->bitmap.rows);
        info.kerning.x = (float)(g->advance.x >> 6);
        info.kerning.y = (float)(g->advance.y >> 6);

        m_glyphs[i] = info;

        /* 1px padding to remove artifacts between characters */
        x += g->bitmap.width + 1;
    }

    m_size = size;

    m_texture = texture.id();

    texture.release();

    return true;
}

} /* game */
