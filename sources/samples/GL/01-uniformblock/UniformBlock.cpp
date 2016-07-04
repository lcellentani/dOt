#include "dot.h"

#include "rendering/RendererGLConfig.h"
#include "common/glslprogram.h"

class UniformBlockAppContext : public dot::core::AppContext
{
public:
	virtual const int GetWindowWidth() const OVERRIDE
	{
		return 512;
	}
	virtual const int GetWindowHeight() const OVERRIDE
	{
		return 512;
	}

	virtual const bool IsDebugModeEnabled() const OVERRIDE
	{
		return true;
	}
};

//=====================================================================================================================

class UniformBlock : public dot::core::Application
{
public:
	UniformBlock() : Application(), vao(0), prog() {}

	virtual void Initialize(dot::core::PlatformContext * const platformContext) OVERRIDE
	{
		Application::Initialize(platformContext);

		LoadShaders();

		InitUniformBlock();

		InitGeometry();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	virtual void Update() OVERRIDE
	{
		Application::Update();
	}

	virtual void Render() OVERRIDE
	{
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	virtual int Shutdown() OVERRIDE
	{
		if (vao != 0)
		{
			glDeleteVertexArrays(1, &vao);
			vao = 0;
		}
		return 0;
	}

	virtual void Reshape(uint32_t width, uint32_t height) OVERRIDE
	{
		glViewport(0, 0, width, height);
	}

private:
	void LoadShaders()
	{
		prog.CompileShaderFromFile("shaders/GL/uniformblock/basic_uniformblock.vert");
		prog.CompileShaderFromFile("shaders/GL/uniformblock/basic_uniformblock.frag");
		prog.Link();
		prog.Use();

		prog.PrintActiveUniforms();
		prog.PrintActiveUniformBlocks();
	}

	void InitUniformBlock()
	{
		GLuint programHandle = prog.GetHandle();

		// Get the index of the uniform block
		GLuint blockIndex = glGetUniformBlockIndex(programHandle, "BlobSettings");

		// Allocate space for the buffer
		GLint blockSize;
		glGetActiveUniformBlockiv(programHandle, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
		GLubyte * blockBuffer;
		blockBuffer = (GLubyte *)malloc(blockSize);

		// Query for the offsets of each block variable
		const GLchar *names[] = { "BlobSettings.InnerColor", "BlobSettings.OuterColor", "BlobSettings.RadiusInner", "BlobSettings.RadiusOuter" };

		GLuint indices[4];
		glGetUniformIndices(programHandle, 4, names, indices);

		GLint offset[4];
		glGetActiveUniformsiv(programHandle, 4, indices, GL_UNIFORM_OFFSET, offset);

		// Store data within the buffer at the appropriate offsets
		GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };
		GLfloat innerRadius = 0.25f;
		GLfloat outerRadius = 0.45f;

		memcpy(blockBuffer + offset[0], innerColor, 4 * sizeof(GLfloat));
		memcpy(blockBuffer + offset[1], outerColor, 4 * sizeof(GLfloat));
		memcpy(blockBuffer + offset[2], &innerRadius, sizeof(GLfloat));
		memcpy(blockBuffer + offset[3], &outerRadius, sizeof(GLfloat));

		// Create the buffer object and copy the data
		GLuint uboHandle;
		glGenBuffers(1, &uboHandle);
		glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
		glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);

		// Bind the buffer object to the uniform block
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboHandle);

#ifdef __APPLE__
		// We don't need this if we specify the binding within the shader (OpenGL 4.2 and above)
		glUniformBlockBinding(programHandle, blockIndex, 1);
#endif
	}

	void InitGeometry()
	{
		float positionData[] =
		{
			-0.8f, -0.8f, 0.0f,
			0.8f, -0.8f, 0.0f,
			0.8f,  0.8f, 0.0f,
			-0.8f, -0.8f, 0.0f,
			0.8f, 0.8f, 0.0f,
			-0.8f, 0.8f, 0.0f
		};
		float tcData[] =
		{
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};

		// Create and populate the buffer objects
		GLuint vboHandles[2];
		glGenBuffers(2, vboHandles);
		GLuint positionBufferHandle = vboHandles[0];
		GLuint tcBufferHandle = vboHandles[1];

		glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), positionData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, tcBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), tcData, GL_STATIC_DRAW);

		// Create and set-up the vertex array object
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);  // Vertex position
		glEnableVertexAttribArray(1);  // Vertex texture coords

		glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		glBindBuffer(GL_ARRAY_BUFFER, tcBufferHandle);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	}

private:
	GLuint vao;
	dot::gl::samples::GLSLProgram prog;
};

//=====================================================================================================================
//=====================================================================================================================

extern "C" dot::core::AppContext * CreateAppContext()
{
	return new UniformBlockAppContext();
}

extern "C" dot::core::Application * CreateApplication()
{
	return new UniformBlock();
}
