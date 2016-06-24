#include "rendering/RendererGL.h"
#include "rendering/RendererContextWGL.h"
#include "core/PlatformContext.h"
#include "core/Log.h"

#include "glimports.h"

namespace dot
{
namespace rendering
{

static const char* getGLString(GLenum _name)
{
	const char* str = (const char*)glGetString(_name);
	glGetError(); // ignore error if glGetString returns NULL.
	if (str != 0)
	{
		return str;
	}

	return "<unknown>";
}

static GLint glGet(GLenum _pname)
{
	GLint result = 0;
	glGetIntegerv(_pname, &result);
	GLenum err = glGetError();
	return 0 == err ? result : 0;
}

RendererGL::RendererGL() : Renderer()
, mContextGL(nullptr)
{

}

RendererGL::~RendererGL()
{

}

RendererType RendererGL::GetRendererType() const
{
	return RendererType::OpenGL;
}

const char * RendererGL::GetRendererName() const
{
	return DOT_RENDERER_OPENGL_NAME;
}

void RendererGL::Flip()
{
	mContextGL->Swap();
}

bool RendererGL::Init(dot::core::PlatformContext * const platformContext)
{
	if (Renderer::Init(platformContext))
	{
		mContextGL = new RendererContextWGL();
		mContextGL->Create(platformContext);

		int loaded = ogl_LoadFunctions();
		if (loaded == ogl_LOAD_FAILED)
		{
			LOGE("Cannot load GL functions\n");
			return false;
		}

		mVendorString = getGLString(GL_VENDOR);
		mRendererString = getGLString(GL_RENDERER);
		mVersionString = getGLString(GL_VERSION);
		mGLSLlVersionString = getGLString(GL_SHADING_LANGUAGE_VERSION);

		if (platformContext->IsDebugModeEnabled())
		{
#define GL_GET(_pname, _min) LOGI("  " #_pname " %d (min: %d)", glGet(_pname), _min)
			LOGI("Defaults:");
#if DOT_RENDERER_OPENGL_TARGET_VERSION >= 41 || BGFX_CONFIG_RENDERER_OPENGLES
			GL_GET(GL_MAX_FRAGMENT_UNIFORM_VECTORS, 16);
			GL_GET(GL_MAX_VERTEX_UNIFORM_VECTORS, 128);
			GL_GET(GL_MAX_VARYING_VECTORS, 8);
#else
			GL_GET(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, 16 * 4);
			GL_GET(GL_MAX_VERTEX_UNIFORM_COMPONENTS, 128 * 4);
			GL_GET(GL_MAX_VARYING_FLOATS, 8 * 4);
#endif
			GL_GET(GL_MAX_VERTEX_ATTRIBS, 8);
			GL_GET(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, 8);
			GL_GET(GL_MAX_CUBE_MAP_TEXTURE_SIZE, 16);
			GL_GET(GL_MAX_TEXTURE_IMAGE_UNITS, 8);
			GL_GET(GL_MAX_TEXTURE_SIZE, 64);
			GL_GET(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, 0);
			GL_GET(GL_MAX_RENDERBUFFER_SIZE, 1);
			GL_GET(GL_MAX_COLOR_ATTACHMENTS, 1);
			GL_GET(GL_MAX_DRAW_BUFFERS, 1);
#undef GL_GET

			LOGI("      Vendor: %s", mVendorString);
			LOGI("    Renderer: %s", mRendererString);
			LOGI("     Version: %s", mVersionString);
			LOGI("GLSL version: %s", mGLSLlVersionString);
		}

		return true;
	}

	return false;
}

} // namespace rendering
} // namespace dot