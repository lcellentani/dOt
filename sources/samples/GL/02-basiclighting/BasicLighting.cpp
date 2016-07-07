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
	BasicLighting() : Application()
	, mDiffuseProgram(nullptr)
	, mPhongProgram(nullptr)
	, mFlatProgram(nullptr)
	, mPPPhongProgram(nullptr)
	, mPPPhongHalfVecProgram(nullptr)
	, mActiveProgram(nullptr)
	, mTorus(nullptr)
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

		//mActiveProgram = mDiffuseProgram;
		//mActiveProgram = mPhongProgram;
		//mActiveProgram = mFlatProgram;
		//mActiveProgram = mPPPhongProgram;
		mActiveProgram = mPPPhongHalfVecProgram;
		mActiveProgram->Use();

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
		if (mDiffuseProgram != nullptr)
		{
			delete mDiffuseProgram;
			mDiffuseProgram = nullptr;
		}
		if (mPhongProgram != nullptr)
		{
			delete mPhongProgram;
			mPhongProgram = nullptr;
		}
		if (mFlatProgram != nullptr)
		{
			delete mFlatProgram;
			mFlatProgram = nullptr;
		}
		if (mPPPhongProgram != nullptr)
		{
			delete mPPPhongProgram;
			mPPPhongProgram = nullptr;
		}
		if (mPPPhongHalfVecProgram != nullptr)
		{
			delete mPPPhongHalfVecProgram;
			mPPPhongHalfVecProgram = nullptr;
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

		projection = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 0.3f, 100.0f);
	}

private:
	void LoadShaders()
	{
		// diffuse lighting shaders
		mDiffuseProgram = new dot::gl::samples::GLSLProgram();
		mDiffuseProgram->CompileShaderFromFile("shaders/GL/e02-basiclighting/diffuse.vert");
		mDiffuseProgram->CompileShaderFromFile("shaders/GL/e02-basiclighting/diffuse.frag");
		mDiffuseProgram->Link();
		mDiffuseProgram->PrintActiveUniforms("diffuse");
		mDiffuseProgram->PrintActiveUniformBlocks("diffuse");

		// phong lighting shaders
		mPhongProgram = new dot::gl::samples::GLSLProgram();
		mPhongProgram->CompileShaderFromFile("shaders/GL/e02-basiclighting/phong.vert");
		mPhongProgram->CompileShaderFromFile("shaders/GL/e02-basiclighting/phong.frag");
		mPhongProgram->Link();
		mPhongProgram->PrintActiveUniforms("phong");
		mPhongProgram->PrintActiveUniformBlocks("phong");

		// flat lighting shaders
		mFlatProgram = new dot::gl::samples::GLSLProgram();
		mFlatProgram->CompileShaderFromFile("shaders/GL/e02-basiclighting/flat.vert");
		mFlatProgram->CompileShaderFromFile("shaders/GL/e02-basiclighting/flat.frag");
		mFlatProgram->Link();
		mFlatProgram->PrintActiveUniforms("flat");
		mFlatProgram->PrintActiveUniformBlocks("flat");

		// phong lighting shaders
		mPPPhongProgram = new dot::gl::samples::GLSLProgram();
		mPPPhongProgram->CompileShaderFromFile("shaders/GL/e02-basiclighting/perpixelphong.vert");
		mPPPhongProgram->CompileShaderFromFile("shaders/GL/e02-basiclighting/perpixelphong.frag");
		mPPPhongProgram->Link();
		mPPPhongProgram->PrintActiveUniforms("pp-phong");
		mPPPhongProgram->PrintActiveUniformBlocks("pp-phong");

		// phong lighting shaders
		mPPPhongHalfVecProgram = new dot::gl::samples::GLSLProgram();
		mPPPhongHalfVecProgram->CompileShaderFromFile("shaders/GL/e02-basiclighting/perpixelphong.vert");
		mPPPhongHalfVecProgram->CompileShaderFromFile("shaders/GL/e02-basiclighting/perpixelphong-halfvec.frag");
		mPPPhongHalfVecProgram->Link();
		mPPPhongHalfVecProgram->PrintActiveUniforms("pp-phong-half");
		mPPPhongHalfVecProgram->PrintActiveUniformBlocks("pp-phong-half");
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
		if (mActiveProgram == nullptr)
		{
			return;
		}

		mAngleY += 20.0f * GetDeltaTime();
		if (mAngleY > 360.0f)
		{
			mAngleY -= 360.0f;
		}

		vec4 worldLight = vec4(5.0f, 5.0f, 2.0f, 1.0f);
		model = mat4(1.0f);
		model *= glm::rotate(glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
		model *= glm::rotate(glm::radians(mAngleY), vec3(0.0f, 1.0f, 0.0f));

		mat4 mv = view * model;

		mActiveProgram->SetUniform("ModelViewMatrix", mv);
		mActiveProgram->SetUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
		mActiveProgram->SetUniform("MVP", projection * mv);
		mActiveProgram->SetUniform("Light.Position", view * worldLight);
		mActiveProgram->SetUniform("Material.Kd", 0.9f, 0.5f, 0.3f);
		mActiveProgram->SetUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
		mActiveProgram->SetUniform("Material.Ka", 0.9f, 0.5f, 0.3f);
		mActiveProgram->SetUniform("Light.La", 0.4f, 0.4f, 0.4f);
		mActiveProgram->SetUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
		mActiveProgram->SetUniform("Light.Ls", 1.0f, 1.0f, 1.0f);
		mActiveProgram->SetUniform("Material.Shininess", 100.0f);
	}

private:
	dot::gl::samples::GLSLProgram *mDiffuseProgram;
	dot::gl::samples::GLSLProgram *mPhongProgram;
	dot::gl::samples::GLSLProgram *mFlatProgram;
	dot::gl::samples::GLSLProgram *mPPPhongProgram;
	dot::gl::samples::GLSLProgram *mPPPhongHalfVecProgram;
	dot::gl::samples::GLSLProgram *mActiveProgram;

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
