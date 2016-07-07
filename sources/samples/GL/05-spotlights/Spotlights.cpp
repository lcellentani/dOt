#include "dot.h"

#include "rendering/RendererGLConfig.h"

#include "common/glslprogram.h"
#include "common/teapot.h"
#include "common/plane.h"
#include "common/torus.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec4;

class SpotlightsAppContext : public dot::core::AppContext
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

class Spotlights : public dot::core::Application
{
public:
	Spotlights() : Application()
	, mProgram(nullptr)
	, mTeapot(nullptr)
	, mPlane(nullptr)
	, mTorus(nullptr)
	, mAngle(0.0f)
	{}

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

		mAngle += 10.0f * GetDeltaTime();
		if (mAngle > 360.0f)
		{
			mAngle -= 360.0f;
		}
	}

	virtual void Render() OVERRIDE
	{
		static const GLfloat black[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		static const GLfloat one = 1.0f;

		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);

		vec4 lightPos = vec4(cos(mAngle), 10.0f, sin(mAngle), 1.0f);
		mProgram->SetUniform("Spot.position", view * lightPos);
		mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
		mProgram->SetUniform("Spot.direction", normalMatrix * vec3(-lightPos));

		mProgram->SetUniform("Kd", 0.9f, 0.5f, 0.3f);
		mProgram->SetUniform("Ks", 0.95f, 0.95f, 0.95f);
		mProgram->SetUniform("Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);
		mProgram->SetUniform("Shininess", 100.0f);
		model = mat4(1.0f);
		model *= glm::translate(vec3(0.0f, 0.0f, -2.0f));
		model *= glm::rotate(glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
		model *= glm::rotate(glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
		UpdateMatricesUniforms();
		mTeapot->Render();

		mProgram->SetUniform("Kd", 0.9f, 0.5f, 0.3f);
		mProgram->SetUniform("Ks", 0.95f, 0.95f, 0.95f);
		mProgram->SetUniform("Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f);
		mProgram->SetUniform("Shininess", 100.0f);
		model = mat4(1.0f);
		model *= glm::translate(vec3(-1.0f, 0.75f, 3.0f));
		model *= glm::rotate(glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
		UpdateMatricesUniforms();
		mTorus->Render();

		mProgram->SetUniform("Kd", 0.7f, 0.7f, 0.7f);
		mProgram->SetUniform("Ks", 0.9f, 0.9f, 0.9f);
		mProgram->SetUniform("Ka", 0.2f, 0.2f, 0.2f);
		mProgram->SetUniform("Shininess", 180.0f);
		model = mat4(1.0f);
		UpdateMatricesUniforms();
		mPlane->Render();
	}

	virtual int Shutdown() OVERRIDE
	{
		if (mProgram != nullptr)
		{
			delete mProgram;
			mProgram = nullptr;
		}
		if (mTeapot != nullptr)
		{
			delete mTeapot;
			mTeapot = nullptr;
		}
		if (mPlane != nullptr)
		{
			delete mPlane;
			mPlane = nullptr;
		}
		if (mTorus != nullptr)
		{
			delete mTorus;
			mTorus = nullptr;
		}
		return 0;
	}

	virtual void Reshape(uint32_t width, uint32_t height) OVERRIDE
	{
		glViewport(0, 0, width, height);

		projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.3f, 100.0f);
	}

private:
	void LoadShaders()
	{
		mProgram = new dot::gl::samples::GLSLProgram();
		mProgram->CompileShaderFromFile("shaders/GL/e05-spotlights/spotlights.vert");
		mProgram->CompileShaderFromFile("shaders/GL/e05-spotlights/spotlights.frag");
		mProgram->Link();
		mProgram->PrintActiveUniforms("spotlights");
		mProgram->PrintActiveUniformBlocks("spotlights");

		mProgram->Use();

		mProgram->SetUniform("Spot.intensity", vec3(0.9f, 0.9f, 0.9f));
		mProgram->SetUniform("Spot.exponent", 30.0f);
		mProgram->SetUniform("Spot.cutoff", 15.0f);
	}

	void InitScene()
	{
		mat4 transform = mat4(1.0f);
		mTeapot = new dot::gl::samples::Teapot(14, transform);
		mPlane = new dot::gl::samples::Plane(50.0f, 50.0f, 1, 1);
		mTorus = new dot::gl::samples::Torus(1.75f * 0.75f, 0.75f * 0.75f, 50, 50);

		model = mat4(1.0f);
		view = glm::lookAt(vec3(5.0f, 5.0f, 7.5f), vec3(0.0f, 0.75f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		projection = mat4(1.0f);
	}

	void UpdateMatricesUniforms()
	{
		mat4 mv = view * model;
		mProgram->SetUniform("ModelViewMatrix", mv);
		mProgram->SetUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
		mProgram->SetUniform("MVP", projection * mv);
	}

private:
	dot::gl::samples::GLSLProgram *mProgram;

	dot::gl::samples::Teapot *mTeapot;
	dot::gl::samples::Plane *mPlane;
	dot::gl::samples::Torus *mTorus;

	mat4 model;
	mat4 view;
	mat4 projection;

	float mAngle;
};

//=====================================================================================================================
//=====================================================================================================================

extern "C" dot::core::AppContext * CreateAppContext()
{
	return new SpotlightsAppContext();
}

extern "C" dot::core::Application * CreateApplication()
{
	return new Spotlights();
}
