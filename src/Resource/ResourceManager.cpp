#include "ResourceManager.hpp"
#include "Core/Logger.hpp"

namespace dm {

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

Texture* ResourceManager::get_texture(const std::string& name) {
    ssize_t id = get_texture_id(name);

    if (id == -1) return &m_textures[0];

    return &m_textures[id];
}

ssize_t ResourceManager::get_texture_id(const std::string& name) {
    auto found = m_texture_names.find(name);
    ssize_t id = -1;

    if (found == m_texture_names.end()) {
        Log::error("Texture %s not found", name.c_str());
    } else {
        id = found->second;
    }

    return id;
}

Texture* ResourceManager::get_texture(const size_t& id) {
    return &m_textures[id];
}

ssize_t ResourceManager::load_texture(const std::string& filename,
                                      const std::string& name) {
    const std::string& real_name =
        name.empty() ? filename.substr(filename.find_last_of("/") + 1) : name;
    auto found = m_texture_names.find(real_name);

    if (found != m_texture_names.end()) return found->second;

    const size_t& texture_id = m_textures.create();
    Texture& texture = m_textures[texture_id];

    Log::progress("debug", "Loading texture %s as %s", filename.c_str(),
                  real_name.c_str());

    if (!texture.load(filename.c_str(), Texture::Filter::Nearest,
                      Texture::Wrap::Clamp)) {
        Log::result(Log::Result::ERROR);
        m_textures.remove(texture_id);
        return -1;
    }

    texture.set_id(texture_id);

    m_texture_names[real_name] = texture_id;

    Log::result(Log::Result::OK);

    return texture_id;
}

ssize_t ResourceManager::load_font(const std::string& filename, size_t size) {
    const size_t name_pos = filename.find_last_of("/");
    const std::string& real_name =
        filename.substr(name_pos + 1,
                        filename.find_last_of(".") - 1 - name_pos) +
        " " + std::to_string(size);

    const size_t& font_id = m_fonts.create();
    const size_t& texture_id = m_textures.create();

    Log::progress("debug", "Loading font %s", real_name.c_str());

    Font& font = m_fonts[font_id];

    m_textures[texture_id].set_id(texture_id);

    if (!font.load(m_textures[texture_id], filename.c_str(), size)) {
        Log::result(Log::Result::ERROR);
        m_fonts.remove(font_id);
        m_textures.remove(texture_id);
        return 0;
    }

    Log::result(Log::Result::OK);

    m_font_names[real_name] = font_id;

    return font_id;
}

ssize_t ResourceManager::get_font_id(const std::string& filename) {
    return m_font_names[filename];
}

Font* ResourceManager::get_font(const std::string& filename) {
    return &m_fonts[m_font_names[filename]];
}

Font* ResourceManager::get_font(const size_t& id) { return &m_fonts[id]; }

} /* namespace dm */
