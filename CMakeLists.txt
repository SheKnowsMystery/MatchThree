cmake_minimum_required(VERSION 3.28)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(
	"MatchThree"
	LANGUAGES CXX
	VERSION 1.0
)

set(Application "MatchThree")

# External libraries
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

# Project parts
add_subdirectory(project/utility)
add_subdirectory(project/components)
add_subdirectory(project/core)
add_subdirectory(project/application)

# Clang compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/templates/.clangd.in
	${CMAKE_CURRENT_SOURCE_DIR}/.clangd
	@ONLY
)

# Installation
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Install path prefix" FORCE)
# set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
# ?????????????????????????????????????????????????????

set(VCPKG_PACKAGES_DIR ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET})
set(VCPKG_BINARY_DIR ${VCPKG_PACKAGES_DIR}/bin/)

install(
	TARGETS ${Application}
	RUNTIME DESTINATION bin
	CONFIGURATIONS Release
)

# install(
# 	FILES $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
# 	DESTINATION bin
# )
# ^^^^^^^^^^^^
# Doesn't work

if(WIN32)
	set(LIBRARY_EXTENSION ".dll")
elseif(APPLE)
	set(LIBRARY_EXTENSION ".dylib")
elseif(UNIX)
	set(LIBRARY_EXTENSION ".so")
endif()

install(
	DIRECTORY ${VCPKG_BINARY_DIR}
	DESTINATION bin
	CONFIGURATIONS Release
	FILES_MATCHING PATTERN "*${LIBRARY_EXTENSION}"
)

install(
	DIRECTORY assets/
	DESTINATION bin/assets
	CONFIGURATIONS Release
)