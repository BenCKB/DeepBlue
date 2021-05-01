workspace "DeepBlue"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"
VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "Dependencies/glfw/include"

include "Dependencies/glfw"

project "DeepBlue"
    location "DeepBlue"
    language "C++"

    filter "configurations:Debug"
        kind "ConsoleApp"
    

    filter "configurations:Release"
        kind "WindowedApp"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c",
        "%{prj.name}/res/Materials/**.h",
        "%{prj.name}/res/Materials/**.cpp"
    }

    includedirs
    {
        "Dependencies/spdlog/include",
        "Dependencies/GLUT/include",
        "%{IncludeDir.GLFW}",
        "%{VULKAN_SDK}/Include"
    }

    libdirs
    {
        "%{VULKAN_SDK}/Lib",
        "Dependencies/GLFW_3.2.1/x64/lib"
    }

    links
    {
        "opengl32.lib",
        "GLFW",
        "User32.lib",
        "vulkan-1.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "_MBCS",
            "IMGUI_IMPL_OPENGL_LOADER_GLAD"
        }

        postbuildcommands
        {
            ("{COPY} res/Models ../bin/" .. outputdir .. "/%{prj.name}/res/Models"),
            ("{COPY} res/Shaders ../bin/" .. outputdir .. "/%{prj.name}/res/Shaders"),
            ("{COPY} res/Textures ../bin/" .. outputdir .. "/%{prj.name}/res/Textures")
        }

    filter "configurations:Debug"
        symbols "On"
    

    filter "configurations:Release"
        optimize "On"
        linkoptions "/ENTRY:mainCRTStartup"
    