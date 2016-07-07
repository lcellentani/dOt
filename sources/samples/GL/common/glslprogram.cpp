#include "glslprogram.h"

#include "core/Log.h"

#include <cstdio>

namespace dot
{
namespace gl
{
namespace samples
{

GLSLProgram::GLSLProgram() : handle(0), linked(false), uniformLocations()
{
}

GLSLProgram::~GLSLProgram()
{
	if (handle != 0)
	{
		uniformLocations.clear();

		// Query the number of attached shaders
		GLint numShaders = 0;
		glGetProgramiv(handle, GL_ATTACHED_SHADERS, &numShaders);

		// Get the shader names
		GLuint * shaderNames = new GLuint[numShaders];
		glGetAttachedShaders(handle, numShaders, NULL, shaderNames);

		// Delete the shaders
		for (int i = 0; i < numShaders; i++)
		{
			glDeleteShader(shaderNames[i]);
		}

		// Delete the program
		glDeleteProgram(handle);
		handle = 0;

		delete[] shaderNames;
	}
}

bool GLSLProgram::CompileShaderFromFile(const char *fileName)
{
	if (fileName != nullptr)
	{
		// Check the file name's extension to determine the shader type
		std::string fileExt = GetFileExtension(fileName);
		GLSLShaderType type = GetTypeFromFileExtension(fileExt.c_str());
		// If we didn't find a match, throw an exception
		if (type != GLSLShaderType::UNKNOW)
		{
			// Pass the discovered shader type along
			return CompileShaderFromFile(fileName, type);
		}
	}
	return false;
}

bool GLSLProgram::CompileShaderFromFile(const char * fileName, GLSLProgram::GLSLShaderType type)
{
	if (fileName != nullptr && type != GLSLShaderType::UNKNOW)
	{
		if (FileExists(fileName))
		{
			if (handle <= 0)
			{
				handle = glCreateProgram();
				//if (handle == 0) {
				//	throw GLSLProgramException("Unable to create shader program.");
				//}
			}

			FILE *fp = fopen(fileName, "rb");

			if (fp != 0)
			{
				fseek(fp, 0, SEEK_END);
				size_t filesize = ftell(fp);
				fseek(fp, 0, SEEK_SET);

				char *data = new char[filesize + 1];

				fread(data, 1, filesize, fp);
				data[filesize] = 0;
				fclose(fp);

				return CompileShaderFromSource(data, type);
			}
		}
	}
	return false;
}

bool GLSLProgram::CompileShaderFromSource(const char *source, GLSLProgram::GLSLShaderType type)
{
	bool result = false;
	if (source != nullptr && type != GLSLShaderType::UNKNOW)
	{
		if (handle <= 0)
		{
			handle = glCreateProgram();
			//if (handle == 0) {
			//	throw GLSLProgramException("Unable to create shader program.");
			//}
		}

		GLuint shaderHandle = glCreateShader(type);

		glShaderSource(shaderHandle, 1, &source, NULL);

		// Compile the shader
		glCompileShader(shaderHandle);

		// Check for errors
		int result;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			// Compile failed, get log
			int length = 0;
			std::string logString;
			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
			if (length > 0)
			{
				char * c_log = new char[length];
				int written = 0;
				glGetShaderInfoLog(shaderHandle, length, &written, c_log);
				logString = c_log;
				delete[] c_log;
			}
			LOGE("Shader compilation failed.\n%s", logString.c_str());
		}
		else
		{
			// Compile succeeded, attach shader
			glAttachShader(handle, shaderHandle);
			result = true;
		}
	}
	return result;
}

bool GLSLProgram::Link()
{
	bool result = false;
	if (!linked && handle > 0)
	{
		glLinkProgram(handle);

		int status = 0;
		glGetProgramiv(handle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			// Store log and return false
			int length = 0;
			std::string logString;

			glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);
			if (length > 0)
			{
				char * c_log = new char[length];
				int written = 0;
				glGetProgramInfoLog(handle, length, &written, c_log);
				logString = c_log;
				delete[] c_log;
			}
			LOGE("Program link failed.\n%s", logString.c_str());
		}
		else
		{
			uniformLocations.clear();
			linked = true;
			result = true;
		}
	}
	return result;
}

bool GLSLProgram::Validate()
{
	bool result = false;
	if (IsLinked())
	{
		GLint status;
		glValidateProgram(handle);
		glGetProgramiv(handle, GL_VALIDATE_STATUS, &status);

		result = true;
		if (status == GL_FALSE)
		{
			result = false;
			// Store log and return false
			int length = 0;
			std::string logString;

			glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);
			if (length > 0)
			{
				char * c_log = new char[length];
				int written = 0;
				glGetProgramInfoLog(handle, length, &written, c_log);
				logString = c_log;
				delete[] c_log;
			}
			LOGE("Program failed to validate.\n%s", logString.c_str());
		}
	}
	return result;
}

bool GLSLProgram::Use()
{
	bool result = false;
	if (handle > 0 && IsLinked())
	{
		glUseProgram(handle);
		result = true;
	}
	return result;
}

int GLSLProgram::GetHandle()
{
	return handle;
}

bool GLSLProgram::IsLinked()
{
	return linked;
}

void GLSLProgram::BindAttribLocation(GLuint location, const char * name)
{
	if (name != nullptr)
	{
		glBindAttribLocation(handle, location, name);
	}
}

void GLSLProgram::BindFragDataLocation(GLuint location, const char * name)
{
	if (name != nullptr)
	{
		glBindFragDataLocation(handle, location, name);
	}
}

void GLSLProgram::SetUniform(const char *name, float x, float y, float z)
{
	GLint loc = GetUniformLocation(name);
	glUniform3f(loc, x, y, z);
}

void GLSLProgram::SetUniform(const char *name, const glm::vec2 &v)
{
	GLint loc = GetUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void GLSLProgram::SetUniform(const char *name, const glm::vec3 &v)
{
	GLint loc = GetUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z);
}

void GLSLProgram::SetUniform(const char *name, const glm::vec4 &v)
{
	GLint loc = GetUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void GLSLProgram::SetUniform(const char *name, const glm::mat4 &m)
{
	GLint loc = GetUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void GLSLProgram::SetUniform(const char *name, const glm::mat3 &m)
{
	GLint loc = GetUniformLocation(name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void GLSLProgram::SetUniform(const char *name, float val)
{
	GLint loc = GetUniformLocation(name);
	glUniform1f(loc, val);
}

void GLSLProgram::SetUniform(const char *name, int val)
{
	GLint loc = GetUniformLocation(name);
	glUniform1i(loc, val);
}

void GLSLProgram::SetUniform(const char *name, bool val)
{
	int loc = GetUniformLocation(name);
	glUniform1i(loc, val);
}

void GLSLProgram::SetUniform(const char *name, GLuint val)
{
	GLint loc = GetUniformLocation(name);
	glUniform1ui(loc, val);
}

void GLSLProgram::PrintActiveUniforms(const char * name)
{
#ifdef __APPLE__
	// For OpenGL 4.1, use glGetActiveUniform
	GLint nUniforms, size, location, maxLen;
	GLchar *name;
	GLsizei written;
	GLenum type;

	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
	glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &nUniforms);

	name = new GLchar[maxLen];

	if (numUniforms > 0)
	{
		LOGI("Active uniforms:");
		LOGI("------------------------------------------------");
		for (int i = 0; i < nUniforms; ++i)
		{
			glGetActiveUniform(handle, i, maxLen, &written, &size, &type, name);
			location = glGetUniformLocation(handle, name);
			LOGI(" %-5d %s (%s)", location, name, getTypeString(type));
		}
		LOGI("------------------------------------------------");
	}
	else
	{
		LOGI("Active uniforms: NONE");
	}

	delete[] name;
#else
	// For OpenGL 4.3 and above, use glGetProgramResource
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

	if (numUniforms > 0)
	{
		LOGI("[%s] Active uniforms:", name);
		LOGI("------------------------------------------------");
		for (int i = 0; i < numUniforms; ++i)
		{
			GLint results[4];
			glGetProgramResourceiv(handle, GL_UNIFORM, i, 4, properties, 4, NULL, results);

			if (results[3] != -1) continue;  // Skip uniforms in blocks
			GLint nameBufSize = results[0] + 1;
			char * name = new char[nameBufSize];
			glGetProgramResourceName(handle, GL_UNIFORM, i, nameBufSize, NULL, name);
			LOGI("%-5d %s (%s)", results[2], name, GetTypeString(results[1]));
			delete[] name;
		}
		LOGI("------------------------------------------------");
	}
	else
	{
		LOGI("[%s] Active uniforms: NONE", name);
	}
#endif
}

void GLSLProgram::PrintActiveUniformBlocks(const char * name)
{
#ifdef __APPLE__
	// For OpenGL 4.1, use glGetActiveUniformBlockiv
	GLint written, maxLength, maxUniLen, nBlocks, binding;
	GLchar *name;

	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxLength);
	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_BLOCKS, &nBlocks);
	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniLen);
	GLchar * uniName = new GLchar[maxUniLen];
	name = new GLchar[maxLength];

	if (numBlocks > 0)
	{
		LOGI("Active Uniform blocks:");
		LOGI("------------------------------------------------");
		for (int i = 0; i < nBlocks; i++)
		{
			glGetActiveUniformBlockName(handle, i, maxLength, &written, name);
			glGetActiveUniformBlockiv(handle, i, GL_UNIFORM_BLOCK_BINDING, &binding);
			LOGI("Uniform block \"%s\" (%d):", name, binding);

			GLint nUnis;
			glGetActiveUniformBlockiv(handle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &nUnis);
			GLint *unifIndexes = new GLint[nUnis];
			glGetActiveUniformBlockiv(handle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, unifIndexes);

			for (int unif = 0; unif < nUnis; ++unif)
			{
				GLint uniIndex = unifIndexes[unif];
				GLint size;
				GLenum type;

				glGetActiveUniform(handle, uniIndex, maxUniLen, &written, &size, &type, uniName);
				LOGI("    %s (%s)", name, getTypeString(type));
			}

			delete[] unifIndexes;
		}
		LOGI("------------------------------------------------");
	}
	else
	{
		LOGI("Active Uniform blocks: NONE");
	}

	delete[] name;
	delete[] uniName;
#else
	GLint numBlocks = 0;

	glGetProgramInterfaceiv(handle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);
	GLenum blockProps[] = { GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH };
	GLenum blockIndex[] = { GL_ACTIVE_VARIABLES };
	GLenum props[] = { GL_NAME_LENGTH, GL_TYPE, GL_BLOCK_INDEX };

	if (numBlocks > 0)
	{
		LOGI("[%s] Active Uniform blocks:", name);
		LOGI("------------------------------------------------");
		for (int block = 0; block < numBlocks; ++block)
		{
			GLint blockInfo[2];
			glGetProgramResourceiv(handle, GL_UNIFORM_BLOCK, block, 2, blockProps, 2, NULL, blockInfo);
			GLint numUnis = blockInfo[0];

			char * blockName = new char[blockInfo[1] + 1];
			glGetProgramResourceName(handle, GL_UNIFORM_BLOCK, block, blockInfo[1] + 1, NULL, blockName);
			LOGI("Uniform block \"%s\":", blockName);
			delete[] blockName;

			GLint * unifIndexes = new GLint[numUnis];
			glGetProgramResourceiv(handle, GL_UNIFORM_BLOCK, block, 1, blockIndex, numUnis, NULL, unifIndexes);

			for (int unif = 0; unif < numUnis; ++unif)
			{
				GLint uniIndex = unifIndexes[unif];
				GLint results[3];
				glGetProgramResourceiv(handle, GL_UNIFORM, uniIndex, 3, props, 3, NULL, results);

				GLint nameBufSize = results[0] + 1;
				char * name = new char[nameBufSize];
				glGetProgramResourceName(handle, GL_UNIFORM, uniIndex, nameBufSize, NULL, name);
				LOGI("    %s (%s)", name, GetTypeString(results[1]));
				delete[] name;
			}

			delete[] unifIndexes;
		}
		LOGI("------------------------------------------------");
	}
	else
	{
		LOGI("[%s] Active Uniform blocks: NONE", name);
	}
#endif
}

void GLSLProgram::PrintActiveAttribs(const char * name)
{
#ifdef __APPLE__
	// For OpenGL 4.1, use glGetActiveAttrib
	GLint written, size, location, maxLength, nAttribs;
	GLenum type;
	GLchar *name;

	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &nAttribs);

	name = new GLchar[maxLength];
	LOGI("Active Attributes:");
	LOGI("------------------------------------------------\n");
	for (int i = 0; i < nAttribs; i++)
	{
		glGetActiveAttrib(handle, i, maxLength, &written, &size, &type, name);
		location = glGetAttribLocation(handle, name);
		LOGI(" %-5d %s (%s)", location, name, getTypeString(type));
	}
	LOGI("------------------------------------------------");
	delete[] name;
#else
	// >= OpenGL 4.3, use glGetProgramResource
	GLint numAttribs;
	glGetProgramInterfaceiv(handle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);

	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

	if (numAttribs > 0)
	{
		LOGI("[%s] Active attributes:", name);
		LOGI("------------------------------------------------\n");
		for (int i = 0; i < numAttribs; ++i)
		{
			GLint results[3];
			glGetProgramResourceiv(handle, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results);

			GLint nameBufSize = results[0] + 1;
			char * name = new char[nameBufSize];
			glGetProgramResourceName(handle, GL_PROGRAM_INPUT, i, nameBufSize, NULL, name);
			LOGI("%-5d %s (%s)", results[2], name, GetTypeString(results[1]));
			delete[] name;
		}
		LOGI("------------------------------------------------");
	}
	else
	{
		LOGI("[%s] Active attributes: NONE", name);
	}
#endif
}

const char * GLSLProgram::GetTypeString(GLenum type)
{
	// There are many more types than are covered here, but
	// these are the most common in these examples.
	switch (type)
	{
	case GL_FLOAT:
		return "float";
	case GL_FLOAT_VEC2:
		return "vec2";
	case GL_FLOAT_VEC3:
		return "vec3";
	case GL_FLOAT_VEC4:
		return "vec4";
	case GL_DOUBLE:
		return "double";
	case GL_INT:
		return "int";
	case GL_UNSIGNED_INT:
		return "unsigned int";
	case GL_BOOL:
		return "bool";
	case GL_FLOAT_MAT2:
		return "mat2";
	case GL_FLOAT_MAT3:
		return "mat3";
	case GL_FLOAT_MAT4:
		return "mat4";
	default:
		return "?";
	}
	return "?";
}

GLint GLSLProgram::GetUniformLocation(const char * name)
{
	std::map<std::string, int>::iterator pos;
	pos = uniformLocations.find(name);
	if (pos == uniformLocations.end())
	{
		uniformLocations[name] = glGetUniformLocation(handle, name);
	}

	return uniformLocations[name];
}

bool GLSLProgram::FileExists(const char *fileName)
{
	struct stat info;
	int ret = -1;

	ret = stat(fileName, &info);
	return 0 == ret;
}

std::string GLSLProgram::GetFileExtension(const char * filename)
{
	if (filename != nullptr)
	{
		std::string name(filename);
		if (!name.empty())
		{
			size_t loc = name.find_last_of('.');
			if (loc != std::string::npos)
			{
				return name.substr(loc, std::string::npos);
			}
		}
	}
	return std::string();
}

GLSLProgram::GLSLShaderType GLSLProgram::GetTypeFromFileExtension(const char *fileExt)
{
	GLSLShaderType type = GLSLShaderType::UNKNOW;
	
	if (_strcmpi(fileExt, ".vs") == 0 || _strcmpi(fileExt, ".vert") == 0)
	{
		type = GLSLShaderType::VERTEX;
	}
	else if (_strcmpi(fileExt, ".gs") == 0 || _strcmpi(fileExt, ".geom") == 0)
	{
		type = GLSLShaderType::GEOMETRY;
	}
	else if (_strcmpi(fileExt, ".tcs") == 0)
	{
		type = GLSLShaderType::TESS_CONTROL;
	}
	else if (_strcmpi(fileExt, ".tes") == 0)
	{
		type = GLSLShaderType::TESS_EVALUATION;
	}
	else if (_strcmpi(fileExt, ".fs") == 0 || _strcmpi(fileExt, ".frag") == 0)
	{
		type = GLSLShaderType::FRAGMENT;
	}
	else if (_strcmpi(fileExt, ".cs") == 0)
	{
		type = GLSLShaderType::COMPUTE;
	}

	return type;
}

} // samples
} // gl
} // dot
