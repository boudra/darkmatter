#ifndef FONT_HPP
#define FONT_HPP

#include <vector>
#include <string>

#include "Resource/Texture.hpp"
#include "Math/Rectangle.hpp"

namespace dm {

struct Glyph {
   Rectangle crop;
   Rectangle vertices;
   Vec2f kerning;
};

class Font {
  public:
   Font();
   ~Font();

   bool load(Texture& texture, const char* name, const int size);
   const size_t& texture() const;
   const Glyph& glyph(const uint32_t caracter) const;

   const int size() const {
      return m_size;
   }

  private:
   size_t m_texture;

   int m_size;

   std::vector<Glyph> m_glyphs;
};

inline const size_t& Font::texture() const { return m_texture; }

inline const Glyph& Font::glyph(const uint32_t character) const {
   return m_glyphs[character];
}

} /* game */

#endif /* end of include guard: m_FONT_HPP_ */

