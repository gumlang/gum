function gum_project(name, type, src)
	src = src or name
	project(name)
		kind(type)
		files("src/" .. src .. "/**.c")
end

workspace "gumlang"
	configurations {"release", "debug"}
	language "C"
	cdialect "C99"
	includedirs "src/include/"
	objdir "bin/obj"
	warnings "Extra"
	
	filter "release"
		optimize "Speed"

	filter "debug"
		symbols "On"
		defines "GUM_DEBUG"

gum_project("libgum", "SharedLib")
	defines "GUM_BUILD"

gum_project("gob", "ConsoleApp")
	links "libgum"