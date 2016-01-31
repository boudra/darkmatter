#ifndef SHADER_HPP
#define SHADER_HPP

#include <cstdint>
#include <vector>

#include "Math/Matrix4.hpp"

namespace dm {

struct Shader {
    enum Type { Vertex, Fragment, Geometry };

    Type type;
    uint32_t id;
    bool compiled;
};

class ProgramShader {
   public:
    ProgramShader();
    ~ProgramShader();

    bool linked() const;
    bool id() const;

    bool link();
    void unload_shaders();
    void bind();
    bool load(const char* filename, Shader::Type type);

    /* GLSL Uniform setters */
    void set_uniform(int value, const char* name);
    void set_uniform(float value, const char* name);
    void set_uniform(const Matrix4f& value, const char* name);
    void set_uniform(const Vec3f& value, const char* name);
    void set_uniform(const Vec2f& value, const char* name);

   private:
    bool m_linked;
    uint32_t m_id;

    std::vector<Shader> m_shaders;
};

inline bool ProgramShader::linked() const { return m_linked; }
inline bool ProgramShader::id() const { return m_id; }

} /* namespace dm */

#endif /* end of include guard: SHADER_HPP */
