#ifndef __DOT_SAMPLES_GLSLPROGRAM_H_HEADER_GUARD__
#define __DOT_SAMPLES_GLSLPROGRAM_H_HEADER_GUARD__

#include "rendering/RendererGLConfig.h"

#include <string>
#include <map>
#include <glm.hpp>

namespace dot
{
namespace gl
{
namespace samples
{

class GLSLProgram
{
public:
	enum GLSLShaderType
	{
		UNKNOW = -1,
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER,
		TESS_CONTROL = GL_TESS_CONTROL_SHADER,
		TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
		COMPUTE = GL_COMPUTE_SHADER
	};

public:
	GLSLProgram();
	virtual ~GLSLProgram();

	bool CompileShaderFromFile(const char *fileName);
	bool CompileShaderFromFile(const char *fileName, GLSLShaderType type);
	bool CompileShaderFromSource(const char *source, GLSLShaderType type);

	bool Link();
	bool Validate();
	bool Use();

	int GetHandle();
	bool IsLinked();

	void BindAttribLocation(GLuint location, const char * name);
	void BindFragDataLocation(GLuint location, const char * name);

	void SetUniform(const char *name, float x, float y, float z);
	void SetUniform(const char *name, const glm::vec2 & v);
	void SetUniform(const char *name, const glm::vec3 & v);
	void SetUniform(const char *name, const glm::vec4 & v);
	void SetUniform(const char *name, const glm::mat4 & m);
	void SetUniform(const char *name, const glm::mat3 & m);
	void SetUniform(const char *name, float val);
	void SetUniform(const char *name, int val);
	void SetUniform(const char *name, bool val);
	void SetUniform(const char *name, GLuint val);

	void PrintActiveUniforms(const char * name = "unnamed");
	void PrintActiveUniformBlocks(const char * name = "unnamed");
	void PrintActiveAttribs(const char * name = "unnamed");

	const char * GetTypeString(GLenum type);

private:
	GLint GetUniformLocation(const char * name);
	bool FileExists(const char *fileName);
	std::string GetFileExtension(const char * fileName);

	GLSLShaderType GetTypeFromFileExtension(const char *fileExt);

	// Make these private in order to make the object non-copyable
	GLSLProgram(const GLSLProgram & other) { }
	GLSLProgram & operator=(const GLSLProgram &other) { return *this; }

private:
	int handle;
	bool linked;
	std::map<std::string, int> uniformLocations;
};

} // samples
} // gl
} // dot

#endif // __DOT_SAMPLES_GLSLPROGRAM_H_HEADER_GUARD__