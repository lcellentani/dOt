--

function dotProject(_name, _kind, _defines)

	project ("dot" .. _name)
		uuid (os.uuid("dot" .. _name))
		kind (_kind)

		includedirs {
			path.join(DOT_DIR, "3rdparty"),
			path.join(DOT_DIR, "3rdparty/khronos"),
		}

		defines {
			_defines,
		}

		configuration {}

		includedirs {
			path.join(DOT_DIR, "include"),
		}

		files {
			path.join(DOT_DIR, "include/**.h"),
			path.join(DOT_DIR, "sources/framework/**.cpp"),
			path.join(DOT_DIR, "sources/framework/**.h"),
		}

		removefiles {
			path.join(DOT_DIR, "sources/framework/**.bin.h"),
		}

		configuration {}

		copyLib()
end
