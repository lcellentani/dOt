--

function dotProject(_name, _kind, _defines)

	project ("dOt" .. _name)
		uuid (os.uuid("dOt" .. _name))
		kind (_kind)

		includedirs {
			path.join(DOT_DIR, "3rdparty"),
			path.join(DOT_DIR, "3rdparty/glloadgen"),
			path.join(DOT_DIR, "3rdparty/glfw/include"),
--			path.join(DOT_DIR, "3rdparty/glm"),
		}

		defines {
			_defines,
		}

		configuration {}

		includedirs {
			path.join(DOT_DIR, "include"),
			path.join(DOT_DIR, "3rdparty/glloadgen"),
		}

		files {
			path.join(DOT_DIR, "include/**.h"),
			path.join(DOT_DIR, "sources/framework/**.cpp"),
			path.join(DOT_DIR, "sources/framework/**.h"),
			path.join(DOT_DIR, "3rdparty/glloadgen/**.c"),
			path.join(DOT_DIR, "3rdparty/glloadgen/**.h"),
		}

		removefiles {
			path.join(DOT_DIR, "sources/framework/**.bin.h"),
		}

		configuration {}

		copyLib()
end