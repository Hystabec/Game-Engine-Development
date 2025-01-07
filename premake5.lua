workspace "GameEngineDevelopment"
	architecture "x86"
	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}/%{cfg.system}-%{cfg.architecture}"

project "GameEngineDevelopment"
	location "GameEngineDevelopment"
	kind "consoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
		"Libraries/imgui-master/imgui.cpp",
		"Libraries/imgui-master/imgui_demo.cpp",
		"Libraries/imgui-master/imgui_draw.cpp",
		"Libraries/imgui_sdl-master/imgui_sdl.h",
		"Libraries/imgui_sdl-master/imgui_sdl.cpp",
		"Libraries/imgui-master/backends/imgui_impl_sdl.h",
		"Libraries/imgui-master/backends/imgui_impl_sdl.cpp",
		"Libraries/imgui-master/imgui_tables.cpp",
		"Libraries/imgui-master/imgui_widgets.cpp"
	}

	includedirs
	{
		"Libraries/SDL2-2.28.3/include",
		"Libraries/imgui-master",
		"Libraries/imgui_sdl-master"
	}

	libdirs
	{
		"Libraries/SDL2-2.28.3/lib"
	}

	links
	{
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_ttf.lib"
	}

	postbuildcommands
	{
		("{copy} ../Libraries/SDL2-2.28.3/lib/SDL2.dll ../bin/" .. outputdir .. "/GameEngineDevelopment"),
		("{copy} ../Libraries/SDL2-2.28.3/lib/SDL2_ttf.dll ../bin/" .. outputdir .. "/GameEngineDevelopment")
	}

	nuget { "pugixml:1.14.0" }

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "on"

	filter "configurations:Realease"
		optimize "on"