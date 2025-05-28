project "Elementa"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "../lumina/Lumina/src",

      "../lumina/Dependencies/imgui",
      "../lumina/Dependencies/glew/include",
      "../lumina/Dependencies/glfw/include",
      "../lumina/Dependencies/glm",
      "../lumina/Dependencies/glad/include",
      "../lumina/Dependencies/tinygltf",
      "../lumina/Dependencies/imguifd",
      "../lumina/Dependencies/spdlog/include"
   }

    links
    {
        "Lumina",
    }

   buildoptions { "/utf-8" }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "LUMINA_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "LUMINA_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "LUMINA_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "LUMINA_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"