#if DOT_RENDERER_OPENGL
#	if DOT_RENDERER_OPENGL_TARGET_VERSION >= 31 && DOT_RENDERER_OPENGL_TARGET_VERSION <= 33
#		if DOT_RENDERER_OPENGL_TARGET_VERSION == 31
#			include <gl_core_3_1.c>
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 32
#			include <gl_core_3_2.c>
#		else
#			include <gl_core_3_3.c>
#		endif
#	elif DOT_RENDERER_OPENGL_TARGET_VERSION >= 40 && DOT_RENDERER_OPENGL_TARGET_VERSION <= 45
#		if DOT_RENDERER_OPENGL_TARGET_VERSION == 40
#			include <gl_core_4_0.c>
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 41
#			include <gl_core_4_1.c>
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 42
#			include <gl_core_4_2.c>
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 43
#			include <gl_core_4_3.c>
#		elif DOT_RENDERER_OPENGL_TARGET_VERSION == 44
#			include <gl_core_4_4.c>
#		else
#			include <gl_core_4_5.c>
#		endif
#	else
#		include <gl_core_2_1.c>
#	endif
#endif