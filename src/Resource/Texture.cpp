#include "Texture.hpp"
#include "Core/Logger.hpp"

#include <iostream>
#include <cassert>

#include <GL/glew.h>
#ifdef WIN32
#include <GL/wglew.h>
#endif
#include <IL/il.h>
#include <IL/ilu.h>

namespace dm {
bool is_devil_initialised = false;

const Texture::Format to_texture_format(int format) {
    Texture::Format texture_format;

    switch (format) {
        case GL_RED:
            texture_format = Texture::Format::Alpha;
            break;

        case GL_RGB:
            texture_format = Texture::Format::RGB;
            break;

        case GL_RGBA:
            texture_format = Texture::Format::RGBA;
            break;

        default:
            assert(false);
            break;
    };

    return texture_format;
}

const unsigned int to_opengl_format(Texture::Format format) {
    unsigned int opengl_format;

    switch (format) {
        case Texture::Format::Alpha:
            opengl_format = GL_RED;
            break;

        case Texture::Format::RGB:
            opengl_format = GL_RGB;
            break;

        case Texture::Format::RGBA:
            opengl_format = GL_RGBA;
            break;

        default:
            assert(false);
            break;
    };

    return opengl_format;
}

const unsigned int to_opengl_filter(Texture::Filter filter) {
    unsigned int opengl_filter;

    switch (filter) {
        case Texture::Filter::Linear:
            opengl_filter = GL_LINEAR;
            break;

        case Texture::Filter::Nearest:
            opengl_filter = GL_NEAREST;
            break;

        case Texture::Filter::NearestMipMap:
            opengl_filter = GL_NEAREST_MIPMAP_NEAREST;
            break;

        default:
            assert(false);
            break;
    }

    return opengl_filter;
}

Texture::Texture()
    : m_name(),
      m_opengl_id(0),
      m_registered(false),
      m_bound(false),
      m_size(0, 0),
      m_bpp(0),
      m_filter_min(Filter::Linear),
      m_filter_mag(Filter::Linear),
      m_wrap_x(Wrap::Clamp),
      m_wrap_y(Wrap::Clamp),
      m_transparent(false) {}

Texture::~Texture() {}

bool Texture::load(const char *filename, Filter filter, Wrap wrap) {
    return this->load(filename, filter, filter, wrap, wrap);
}

bool Texture::load(const char *filename, Filter filter_min, Filter filter_mag,
                   Wrap wrap_x, Wrap wrap_y) {
    m_filter_min = filter_min;
    m_filter_mag = filter_mag;
    m_wrap_x = wrap_x;
    m_wrap_y = wrap_y;

    unsigned char *data = nullptr;
    unsigned int ilid;

    if (!is_devil_initialised) {
        ilInit();
        is_devil_initialised = true;
    }

    ilGenImages(1, &ilid);

    if (ilLoadImage(filename) != IL_TRUE) {
        return false;
    }

    data = ilGetData();
    m_size = Vec2i(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));

    m_bpp = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);

    m_name = std::string(filename);

    m_format = to_texture_format(ilGetInteger(IL_IMAGE_FORMAT));

    ILinfo image_info;

    iluGetImageInfo(&image_info);

    if (image_info.Origin == IL_ORIGIN_UPPER_LEFT) {
        iluFlipImage();
    }

    this->set_wrap_mode(Texture::Wrap::Repeat, Texture::Wrap::Repeat);

    this->upload();

    this->bind();

    this->setup();
    this->set_data(data);

    this->release();

    ilDeleteImages(1, &ilid);

    return true;
}

void Texture::set_data(unsigned char *data) {
    unsigned int format = to_opengl_format(m_format);
    size_t bytes = m_bpp / 8;

    assert(m_bound);

    if (m_format == Format::Alpha) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, to_opengl_format(m_format), m_size.x,
                 m_size.y, 0, format, GL_UNSIGNED_BYTE, data);

    if (m_format == Format::RGBA) {
        for (size_t i = bytes - 1; i < m_size.x * m_size.y * bytes;
             i += bytes) {
            if (data[i] < 255) {
                m_transparent = true;
                break;
            }
        }
    }
}

void Texture::sub_data(unsigned char *data, Vec2i position, Vec2i size) const {
    unsigned int format = to_opengl_format(m_format);

    assert(position + size < m_size && m_bound);

    if (m_format == Format::Alpha) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, position.x, position.y, size.x, size.y,
                    format, GL_UNSIGNED_BYTE, data);
}

void Texture::setup() {
    assert(m_registered && m_bound);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    // GL_LINEAR);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
    // GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    to_opengl_filter(m_filter_min));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    to_opengl_filter(m_filter_mag));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    m_wrap_x == Wrap::Repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    m_wrap_y == Wrap::Repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
}

unsigned int Texture::upload() {
    assert(m_registered == false);

    glGenTextures(1, &m_opengl_id);
    glBindTexture(GL_TEXTURE_2D, m_opengl_id);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_registered = true;

    return m_opengl_id;
}

void Texture::bind() {
    assert(m_opengl_id != 0 && m_registered && !m_bound);
    glBindTexture(GL_TEXTURE_2D, m_opengl_id);
    m_bound = true;
}

void Texture::release() {
    assert(m_bound);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_bound = false;
}

void Texture::destroy() {
    if (m_registered) glDeleteTextures(1, &m_opengl_id);
    m_registered = false;
}

} /* game */
