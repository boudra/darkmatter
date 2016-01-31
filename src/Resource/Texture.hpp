#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include "Math/Vector2.hpp"

namespace dm {

class Texture {
   public:
    enum Wrap { Clamp, Repeat };

    enum Filter { Linear, Nearest, NearestMipMap };

    enum Format { RGB, RGBA, Alpha };

    Texture();
    virtual ~Texture();

    bool load(const char* filename, Filter filter_min, Filter filter_mag,
              Wrap wrap_x, Wrap wrap_y);

    bool load(const char* filename, Filter filter = Filter::Nearest,
              Wrap wrap = Wrap::Clamp);

    unsigned int upload();

    void bind();
    void release();

    void setup();

    void destroy();

    const size_t id() const;
    const unsigned int opengl_id() const;

    const bool is_bound() const { return m_bound; }
    const bool is_registered() const { return m_registered; }

    void set_data(unsigned char* data);
    void sub_data(unsigned char* data, Vec2i position, Vec2i size) const;

    const Vec2i& size() const { return m_size; }
    const int height() const { return m_size.y; }

    const Vec2f sizef() const { return Vec2f(m_size.x, m_size.y); }

    void set_size(const Vec2i& size);
    void set_format(const Format format);

    void set_filter(const Filter min, const Filter mag);
    void set_wrap_mode(const Wrap x, const Wrap y);

    const std::string& filename() const;

    const Texture::Format format() const { return m_format; }

    const bool transparent() const { return m_transparent; }

    void set_transparent(const bool transparent) {
        m_transparent = transparent;
    }

    void set_id(const size_t id) { m_id = id; }

   private:
    std::string m_name;

    unsigned int m_opengl_id;
    size_t m_id;
    bool m_registered;
    bool m_bound;

    Vec2i m_size;
    int m_bpp;

    Filter m_filter_min, m_filter_mag;
    Wrap m_wrap_x, m_wrap_y;
    Format m_format;

    bool m_transparent;
};

inline const std::string& Texture::filename() const { return m_name; }

inline void Texture::set_filter(const Filter min, const Filter mag) {
    m_filter_min = min;
    m_filter_mag = mag;
}

inline void Texture::set_wrap_mode(const Wrap x, const Wrap y) {
    m_wrap_x = x;
    m_wrap_y = y;
}

inline const Texture::Format to_texture_format(int format);
inline const unsigned int to_opengl_format(Texture::Format format);

inline const size_t Texture::id() const { return m_id; }
inline const unsigned int Texture::opengl_id() const { return m_opengl_id; }

inline void Texture::set_size(const Vec2i& size) { m_size = size; }
inline void Texture::set_format(const Format format) { m_format = format; }

} /* game */

#endif /* end of include guard: m_TEXTURE_HPP_ */
