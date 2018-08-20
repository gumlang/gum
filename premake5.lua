workspace "gumlang"
	configurations {"debug", "release"}

project "gum"
	kind "ConsoleApp"
	language "C"
	cdialect "C99"

	files "src/**.c"
	objdir "bin/%{cfg.buildcfg}/obj"
	targetdir "bin/%{cfg.buildcfg}"

	filter "debug"
		symbols "On"
	
	filter "release"
		optimize "Speed"
		flags "LinkTimeOptimization"