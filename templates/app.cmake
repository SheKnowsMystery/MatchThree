add_executable(${current})

file(GLOB source "source/*.cxx")

source_group(source FILES ${source})

target_sources(${current} PUBLIC ${source})

# Accumulation object files
get_property(GLOBAL_SOURCE GLOBAL PROPERTY GLOBAL_SOURCE)
target_sources(${current} PRIVATE ${GLOBAL_SOURCE})

# CMake sets "CompileAsCpp" explicitly in VS project file
# for our module files for some reason
# so we're getting rid of it
set_source_files_properties(${module} APPEND PROPERTIES VS_SETTINGS "CompileAs=CompileAsCppModule")

set(msvc $<CXX_COMPILER_ID:MSVC>)
set(clang $<CXX_COMPILER_ID:Clang>)
set(gcc $<CXX_COMPILER_ID:GCC>)

target_compile_features(${current} PRIVATE cxx_std_20)

if (${WIN32})
	target_link_options(
		${current}
		PRIVATE
		$<${msvc}:/SUBSYSTEM:WINDOWS>
		$<${clang}:-Wl,/SUBSYSTEM:WINDOWS>
	)
endif()