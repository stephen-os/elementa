-- premake5.lua
workspace "Elementa"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Elementa"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Lumina/LuminaExternal.lua"

group "App"
   include "Elementa/Elementa.lua"
group ""