add_library(${current} OBJECT)

# Globbing All files related to this part
file(GLOB source "source/*.cxx")
file(GLOB module "module/*.ixx")

# Assuming that there is a primary module interface unit
string(TOLOWER "interface/${current}.ixx" ifc)

# Structuring source file for convenience
source_group("source" FILES ${source})
source_group("module" FILES ${module})
source_group("interface" FILES ${ifc})

# Adding the primary module interface unit to all module list
list(APPEND module ${ifc})

# Adding source files + modules files to library
target_sources(${current} PRIVATE ${source})
target_sources(${current} PUBLIC FILE_SET CXX_MODULES FILES ${module})

# Visual Studio workarounds
set(
	module_VS_SETTINGS
		# CMake sets "CompileAsCpp" explicitly in VS project file
		# for our module files for some reason
		# so we're getting rid of it
		"CompileAs=CompileAsCppModule"
		# Visual Studio generates <module-name>.ixx.obj files by default
		# but searches for <module-name>.obj files
		# when creating OBJECT type library everything falls apart
		"ObjectFileName=$(IntDir)%(Filename).obj"
)
set_source_files_properties(
	${module} APPEND PROPERTIES VS_SETTINGS "${module_VS_SETTINGS}"
)

# C++ standard
target_compile_features(${current} PRIVATE cxx_std_20)

# Adding library sources to global list to link it all into one library
set(LOCAL_SOURCE $<TARGET_OBJECTS:${current}>)                               # getting all sources
get_property(GLOBAL_SOURCE GLOBAL PROPERTY GLOBAL_SOURCE)                    # getting old list
set_property(GLOBAL PROPERTY GLOBAL_SOURCE ${GLOBAL_SOURCE} ${LOCAL_SOURCE}) # combining local and global