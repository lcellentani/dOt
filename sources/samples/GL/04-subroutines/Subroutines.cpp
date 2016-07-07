#include "dot.h"

#include "rendering/RendererGLConfig.h"

#include "common/glslprogram.h"
#include "common/teapot.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec4;

class SubroutinesAppContext : public dot::core::AppContext
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

class Subroutines : public dot::core::Application
{
public:
	Subroutines() : Application()
	, mProgram(nullptr)
	, mTeapot(nullptr)
	, mDiffuseFuncIndex(0)
	, mPhongFuncIndex(0)
	, mAngleY(0.0f)
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

		mProgram->Use();

		glEnable(GL_DEPTH_TEST);
	}

	virtual void Update() OVERRIDE
	{
		Application::Update();

		mAngleY += 30.0f * GetDeltaTime();
		if (mAngleY > 360.0f)
		{
			mAngleY -= 360.0f;
		}

		UpdateMaterialsUniforms();
	}

	virtual void Render() OVERRIDE
	{
		static const GLfloat black[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		static const GLfloat one = 1.0f;

		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);

		glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &mPhongFuncIndex);
		model = mat4(1.0f);
		model *= glm::translate(vec3(-4.0, -1.5, 0.0));
		model *= glm::rotate(glm::radians(mAngleY), vec3(0.0f, 1.0f, 0.0f));
		model *= glm::rotate(glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
		UpdateMatricesUniforms();
		mTeapot->Render();

		glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &mDiffuseFuncIndex);
		model = mat4(1.0f);
		model *= glm::translate(vec3(4.0f, -1.5f, 0.0f));
		model *= glm::rotate(glm::radians(mAngleY), vec3(0.0f, 1.0f, 0.0f));
		model *= glm::rotate(glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
		UpdateMatricesUniforms();
		mTeapot->Render();
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
		mProgram->CompileShaderFromFile("shaders/GL/e04-subroutines/subroutines.vert");
		mProgram->CompileShaderFromFile("shaders/GL/e04-subroutines/subroutines.frag");
		mProgram->Link();
		mProgram->PrintActiveUniforms("subroutines");
		mProgram->PrintActiveUniformBlocks("subroutines");

		mPhongFuncIndex = glGetSubroutineIndex(mProgram->GetHandle(), GL_VERTEX_SHADER, "phongModel");
		mDiffuseFuncIndex = glGetSubroutineIndex(mProgram->GetHandle(), GL_VERTEX_SHADER, "lambertModel");
	}

	void InitScene()
	{
		mat4 transform = mat4(1.0f);
		mTeapot = new dot::gl::samples::Teapot(13, transform);

		model = mat4(1.0f);
		view = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		projection = mat4(1.0f);
	}

	void UpdateMaterialsUniforms()
	{
		vec4 lightPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		mProgram->SetUniform("Light.Position", lightPos);

		mProgram->SetUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
		mProgram->SetUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
		mProgram->SetUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
		mProgram->SetUniform("Light.La", 0.4f, 0.4f, 0.4f);
		mProgram->SetUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
		mProgram->SetUniform("Light.Ls", 1.0f, 1.0f, 1.0f);
		mProgram->SetUniform("Material.Shininess", 100.0f);
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

	uint32_t mDiffuseFuncIndex;
	uint32_t mPhongFuncIndex;

	mat4 model;
	mat4 view;
	mat4 projection;

	float mAngleY;
};

//=====================================================================================================================
//=====================================================================================================================

extern "C" dot::core::AppContext * CreateAppContext()
{
	return new SubroutinesAppContext();
}

extern "C" dot::core::Application * CreateApplication()
{
	return new Subroutines();
}
