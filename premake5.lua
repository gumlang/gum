workspace "gumlang"
	configurations {"debug", "release"}
	language "C"
	cdialect "C99"
	includedirs "src/include/"
	objdir "bin/obj"

	filter "debug"
		symbols "On"
	
	filter "release"
		optimize "Speed"
		flags "LinkTimeOptimization"

project "libgum"
	kind "SharedLib"
	files "src/lib/**.c"

project "gum"
	kind "ConsoleApp"
	files "src/cli/**.c"
	links "libgum"