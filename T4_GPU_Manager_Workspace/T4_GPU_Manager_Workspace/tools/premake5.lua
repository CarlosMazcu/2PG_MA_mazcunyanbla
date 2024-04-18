--
-- premake5.lua
--
-- GENIe solution to build the EDK project.
-- Ivan Sancho, Escuela Superior de Arte y Tecnologia (ESAT) Valencia, 2023/24
-- Graphic Programming
--
-- All rights are reserved.
--
--

PROJ_DIR = path.getabsolute("./../")

project_names = {
  "PR04_00_GPUManager_Template",
}

-- Solution workspace declaration:
solution("EDK_Workspace_" .. _ACTION)
  location(PROJ_DIR .. "/build/")
  language "C++"
  kind "ConsoleApp"
  startproject "PR04_00_GPUManager_Template"
  platforms {
    "x32",
    "Native",
  }
  defines {
    "_CRT_SECURE_NO_WARNINGS",
  }
  configurations {
    "Debug",
    "Release"
  }

-- Workspace "Debug" configuration:
filter { "configurations:Debug" }
  defines {
    "DEBUG",
  }
  symbols "On"
  warnings "Extra"
  targetsuffix "_d"

  -- Workspace "Release" configuration:
filter { "configurations:Release" }
  defines {
    "NDEBUG",
  }
  symbols "Off"
  warnings "Extra"
  optimize "On"
  optimize "Size"
  flags {
    "No64BitChecks",
    "NoPCH",
  }

--[[********************** PROJECT GENERATOR FUNCTION **********************]]--
function GenerateProject(project_name, project_group)
  project(project_name)
    location (PROJ_DIR .. "/build/" .. project_name .. "/" .. _ACTION)
    includedirs {
      path.join(PROJ_DIR, "./include/"),
      path.join(PROJ_DIR, "./deps/EDK/include/"),
    }
    files {
      path.join(PROJ_DIR, "./include/*.h"),
      path.join(PROJ_DIR, "./include/dev/*.h"),
      path.join(PROJ_DIR, "./src/*.cc"),
      path.join(PROJ_DIR, "./src/dev/*.cc"),
      path.join(PROJ_DIR, "./deps/EDK/include/EDK3/*.h"),
      path.join(PROJ_DIR, "./deps/EDK/include/EDK3/dev/*.h"),
      path.join(PROJ_DIR, "./deps/EDK/include/ESAT/*.h"),
      path.join(PROJ_DIR, "./deps/EDK/include/ESAT_extra/*.h"),
    }

    libdirs { path.join(PROJ_DIR, "./deps/EDK/lib/EDK/x32"), }

    --Debug filters:
    filter { "configurations:Debug", "platforms:x32" }
      architecture "x86"
      targetdir (path.join(PROJ_DIR, "bin/Debug/x32"))
      objdir(path.join(PROJ_DIR, "./build/" .. project_name .. "/Debug"))
      targetsuffix "_d"
      links { path.join(PROJ_DIR, "deps/EDK/lib/EDK/x32/ESAT_d"),
              path.join(PROJ_DIR, "deps/EDK/lib/EDK/x32/ESAT_extra_d"),
              "OpenGL32",
              "winmm" }
    filter { "configurations:Release", "platforms:x32" }
      architecture "x86"
      targetdir (path.join(PROJ_DIR, "bin/Release/x32/"))
      objdir(path.join(PROJ_DIR, "./build/" .. project_name .. "/Release"))
      links { path.join(PROJ_DIR, "deps/EDK/lib/EDK/x32/ESAT"),
              path.join(PROJ_DIR, "deps/EDK/lib/EDK/x32/ESAT_extra"),
              "OpenGL32",
              "winmm" }
end --Function "GenerateProject" ends here.

--Calling the Lua function:
for i, prj in ipairs(project_names) do
  GenerateProject(prj)
end

project "PR04_00_GPUManager_Template"
	files {
	  path.join(PROJ_DIR, "./tests/PR04_00_GPUManager_Template.cc"),
  }
