workspace "gumlang"
	configurations {"debug", "release"}
	language "C"
	cdialect "C99"
	includedirs "src/include/"
	objdir "bin/obj"
	warnings "Extra"

	filter "debug"
		symbols "On"
	
	filter "release"
		optimize "Speed"

project "libgrt"
	kind "SharedLib"
	files "src/lib/**.c"
	defines "GUM_BUILD"

project "gum"
	kind "ConsoleApp"
	files "src/cli/**.c"
	links "libgrt"