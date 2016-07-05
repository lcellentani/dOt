#include "dot.h"

#include "rendering/RendererGLConfig.h"

#include "common/glslprogram.h"
#include "common/torus.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec4;

class BasicLightingAppContext : public dot::core::AppContext
{
public:
	virtual const int GetWindowWidth() const OVERRIDE
	{
		return 1024;
	}
	virtual const int GetWindowHeight() const OVERRIDE
	{
		return 576;
	}

	virtual const bool IsDebugModeEnabled() const OVERRIDE
	{
		return true;
	}
};

//=====================================================================================================================

class BasicLighting : public dot::core::Application
{
public:
	BasicLighting() : Application(), mDiffuseProgram(), mTorus(nullptr), mAngleY(0.0f) {}

	//@note: this is to prevent compiling warning C4316
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}
	//@note: this is to prevent compiling warning C4316
	void operator delete(void* p)
	{
		_mm_free(p);
	}

	virtual void Initialize(dot::core::PlatformContext * const platformContext) OVERRIDE
	{
		Application::Initialize(platformContext);

		LoadShaders();

		InitScene();

		glEnable(GL_DEPTH_TEST);
	}

	virtual void Update() OVERRIDE
	{
		Application::Update();

		UpdateUniforms();
	}

	virtual void Render() OVERRIDE
	{
		static const GLfloat black[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		static const GLfloat one = 1.0f;

		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);

		mTorus->Render();
	}

	virtual int Shutdown() OVERRIDE
	{
		return 0;
	}

	virtual void Reshape(uint32_t width, uint32_t height) OVERRIDE
	{
		glViewport(0, 0, width, height);

		projection = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 0.3f, 100.0f);
	}

private:
	void LoadShaders()
	{
		// diffuse lighting shaders
		mDiffuseProgram.CompileShaderFromFile("shaders/GL/basiclighting/diffuse.vert");
		mDiffuseProgram.CompileShaderFromFile("shaders/GL/basiclighting/diffuse.frag");
		mDiffuseProgram.Link();
		mDiffuseProgram.Use();
		mDiffuseProgram.PrintActiveUniforms();
		mDiffuseProgram.PrintActiveUniformBlocks();
	}

	void InitScene()
	{
		mTorus = new dot::gl::samples::Torus(0.7f, 0.3f, 30, 30);

		model = mat4(1.0f);
		view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		projection = mat4(1.0f);
	}

	void UpdateUniforms()
	{
		mAngleY += 20.0f * GetDeltaTime();
		if (mAngleY > 360.0f)
		{
			mAngleY -= 360.0f;
		}

		model = mat4(1.0f);
		model *= glm::rotate(glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
		model *= glm::rotate(glm::radians(mAngleY), vec3(0.0f, 1.0f, 0.0f));

		mat4 mv = view * model;
		mDiffuseProgram.SetUniform("ModelViewMatrix", mv);
		mDiffuseProgram.SetUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
		mDiffuseProgram.SetUniform("MVP", projection * mv);
		mDiffuseProgram.SetUniform("Kd", 0.9f, 0.5f, 0.3f);
		mDiffuseProgram.SetUniform("Ld", 1.0f, 1.0f, 1.0f);
		mDiffuseProgram.SetUniform("LightPosition", view * vec4(5.0f, 5.0f, 2.0f, 1.0f));
	}

private:
	dot::gl::samples::GLSLProgram mDiffuseProgram;

	dot::gl::samples::Torus *mTorus;

	mat4 model;
	mat4 view;
	mat4 projection;

	float mAngleY;
};

//=====================================================================================================================
//=====================================================================================================================

extern "C" dot::core::AppContext * CreateAppContext()
{
	return new BasicLightingAppContext();
}

extern "C" dot::core::Application * CreateApplication()
{
	return new BasicLighting();
}
