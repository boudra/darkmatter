#include "Graphics/Shader.hpp"
#include "Core/Logger.hpp"

#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h>
#endif

#include <fstream>

namespace dm {

ProgramShader::ProgramShader() : m_linked(false), m_id(0) {}
ProgramShader::~ProgramShader() { this->unload_shaders(); }

bool ProgramShader::link() {

  Log::progress("debug", "Linking shader %d", m_id);

   assert(!m_shaders.empty());

   glLinkProgram(m_id);

   GLint linked;
   glGetProgramiv(m_id, GL_LINK_STATUS, &linked);

   if (linked == GL_FALSE) {

      GLint length, lengthRead = 0;
      glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);

      char* log = new char[length];
      glGetProgramInfoLog(m_id, length, &lengthRead, log);

      Log::result(Log::Result::ERROR);
      Log::debug("Error log: %s", log);

      delete[] log;
      return false;
   }

   Log::result(Log::Result::OK);

   m_linked = true;

   return true;
}

void ProgramShader::unload_shaders() {
   for (Shader& s : m_shaders) {
      if (s.compiled) {
         glDeleteShader(s.id);
         s.compiled = false;
         s.id = 0;
      }
   }
}

void ProgramShader::bind() {
   assert(m_id != 0);
   glUseProgram(m_id);
}

bool ProgramShader::load(const char* filename, Shader::Type type) {
   assert(m_linked == false);

   /* Create the program */
   if (m_id == 0) {
      m_id = glCreateProgram();
   }

   int size = 0;
   char* data = nullptr;
   std::ifstream file(filename, std::ios::in);

   if (!file.is_open()) {
      return false;
   }

   /* Calculate the size needed to store the file */
   file.seekg(0, std::ios::end);
   size = file.tellg();
   file.seekg(0, std::ios::beg);

   data = new char[size];

   file.read(data, size);

   /* Check if the whole file was loaded */
   if (file.tellg() != size) {
      delete[] data;
      return false;
   }

   file.close();

   if (m_shaders.size() <= type) {
      m_shaders.resize(type + 1);
   }

   Shader& shader = m_shaders[type];

   if (shader.compiled) {
      glDeleteShader(shader.id);
   }

   shader = Shader{type, 0, false};

   /* TODO: Other shader types */
   shader.id = glCreateShader(
       type == Shader::Type::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

   if (shader.id == 0) {
      delete[] data;
      return false;
   }

   Log::progress("debug", "Compiling shader %s", filename);

   const GLchar* source = static_cast<const GLchar*>(data);
   const int sizei = static_cast<const int>(size);

   glShaderSource(shader.id, 1, static_cast<const GLchar**>(&source),
                  static_cast<const GLint*>(&sizei));
   glCompileShader(shader.id);

   delete[] data;

   GLint compiled = 0;
   glGetShaderiv(shader.id, GL_COMPILE_STATUS, &compiled);

   /* Failed to compile, show the error log and exit */
   if (compiled == GL_FALSE) {

      GLint length, lengthRead = 0;
      glGetShaderiv(shader.id, GL_INFO_LOG_LENGTH, &length);
      char* log = new char[length];
      glGetShaderInfoLog(shader.id, length, &lengthRead, log);

      Log::result(Log::Result::ERROR);
      Log::debug("Error log: %s", log);

      glDeleteShader(shader.id);

      delete[] log;
      return false;
   }

   Log::result(Log::Result::OK);

   shader.compiled = true;

   glAttachShader(m_id, shader.id);

   return true;
}

/* GLSL Uniform setters */
void ProgramShader::set_uniform(int value, const char* name) {
   assert(m_id != 0);
   glUniform1i(glGetUniformLocation(m_id, name), value);
}

void ProgramShader::set_uniform(float value, const char* name) {
   assert(m_id != 0);
   glUniform1f(glGetUniformLocation(m_id, name), value);
}

void ProgramShader::set_uniform(const Matrix4f& value, const char* name) {
   assert(m_id != 0);
   Matrix4f transposed = value.transpose();
   glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE,
                      &transposed.m[0]);
}

void ProgramShader::set_uniform(const Vec3f& value, const char* name) {
   assert(m_id != 0);
   glUniform3f(glGetUniformLocation(m_id, name), value.x, value.y, value.z);
}

void ProgramShader::set_uniform(const Vec2f& value, const char* name) {
   assert(m_id != 0);
   glUniform2f(glGetUniformLocation(m_id, name), value.x, value.y);
}

} /* namespace dm */
