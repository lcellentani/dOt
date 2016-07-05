--

solution "dOt"
	configurations {
		"Debug",
		"Release",
	}

	platforms {
		"x32",
		"x64",
--		"Xbox360",
		"Native", -- for targets where bitness is not specified
	}

	language "C++"
	startproject "e00-helloworld"

MODULE_DIR = path.getabsolute("../")
DOT_DIR = path.getabsolute("..")

local DOT_BUILD_DIR = path.join(DOT_DIR, ".build")
local DOT_THIRD_PARTY_DIR = path.join(DOT_DIR, "3rdparty")

dofile (path.join(DOT_DIR, "scripts/toolchain.lua"))
if not toolchain(DOT_BUILD_DIR, DOT_THIRD_PARTY_DIR) then
	return -- no action specified
end

function copyLib()
end

function exampleProject(_name, _basedir, _commonProject)

	project ("e" .. _name)
		uuid (os.uuid("example-" .. _name))
		kind "WindowedApp"

	configuration {}

	debugdir (path.join(DOT_DIR, "sources/samples/runtime"))

	includedirs {
		path.join(DOT_DIR, "3rdparty/glloadgen"),
		path.join(DOT_DIR, "3rdparty/glm"),
		path.join(DOT_DIR, "sources/framework"),
		path.join(DOT_DIR, "sources/samples/", _basedir),
	}

	files {
		path.join(DOT_DIR, "sources/samples/", _basedir, _name, "**.c"),
		path.join(DOT_DIR, "sources/samples/", _basedir, _name, "**.cpp"),
		path.join(DOT_DIR, "sources/samples/", _basedir, _name, "**.h"),
	}

	removefiles {
		path.join(DOT_DIR, "sources/samples/", _basedir, _name, "**.bin.h"),
	}

	links {
		"dot",
		_commonProject
	}
	
	configuration { "x32", "vs*" }
		libdirs {
			path.join(DOT_BUILD_DIR, "win32_" .. _ACTION, "bin"),
			path.join(DOT_BUILD_DIR, "../3rdparty/glfw/lib"),
		}

	configuration { "x64", "vs*" }
		libdirs {
			path.join(DOT_BUILD_DIR, "win64_" .. _ACTION, "bin"),
			path.join(DOT_BUILD_DIR, "../3rdparty/glfw/lib"),
		}
		
	configuration { "vs*", "x32 or x64" }
		linkoptions {
			"/ignore:4199", -- LNK4199: /DELAYLOAD:*.dll ignored; no imports found from *.dll
		}

	configuration { "vs20*", "x32 or x64" }
		links {
			"gdi32",
			"opengl32",
		}

	configuration { "vs20*", "Debug" }
		links {
			"glfw3_d",
		}
		
	configuration { "vs20*", "Release" }
		links {
			"glfw3",
		}
		
	configuration {}

	strip()
end

function examplesCommonProject(_name, _basedir)
	project (_name)
		uuid (os.uuid(_name))
		kind "StaticLib"
	
	configuration {}	

	includedirs {
		path.join(DOT_DIR, "3rdparty/glloadgen"),
		path.join(DOT_DIR, "3rdparty/glm"),
		path.join(DOT_DIR, "sources/framework"),
		path.join(DOT_DIR, "sources/samples/", _basedir),
	}

	files {
		path.join(DOT_DIR, "sources/samples/", _basedir, "common", "**.cpp"),
		path.join(DOT_DIR, "sources/samples/", _basedir, "common", "**.h"),
	}

	removefiles {
		path.join(DOT_DIR, "sources/framework/**.bin.h"),
	}

	configuration {}

	copyLib()
end

dofile "dot.lua"

group "libs"
dotProject("", "StaticLib", {})

group "examples/GL"
examplesCommonProject("gl-examples-common", "GL")
exampleProject("00-helloworld", "GL", "gl-examples-common")
exampleProject("01-uniformblock", "GL", "gl-examples-common")

