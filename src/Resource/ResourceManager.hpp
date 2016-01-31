#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "Core/Manager.hpp"
#include "Resource/Texture.hpp"
#include "Resource/Font.hpp"
#include "Core/ObjectPool.hpp"

#include <map>

namespace dm {

class ResourceManager : public Manager<ResourceManager> {
   public:
    ResourceManager();
    ~ResourceManager();

    Texture* get_texture(const std::string& name);
    ssize_t get_texture_id(const std::string& name);
    Texture* get_texture(const size_t& id);
    ssize_t load_texture(const std::string& filename,
                         const std::string& name = std::string());

    ssize_t load_font(const std::string& filename, size_t size);
    ssize_t get_font_id(const std::string& filename);
    Font* get_font(const std::string& filename);
    Font* get_font(const size_t& id);

    const bool initialize();

   private:
    ObjectPool<Texture> m_textures;
    std::map<std::string, size_t> m_texture_names;

    ObjectPool<Font> m_fonts;
    std::map<std::string, size_t> m_font_names;
};

} /* namespace dm */

#endif /* end of include guard: RESOURCEMANAGER_HPP */
