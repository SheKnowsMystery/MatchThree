
add_library("Components" INTERFACE)

add_subdirectory(SDL)

set(Components "SDL")

set_property(TARGET ${Components} PROPERTY FOLDER "Components")

target_link_libraries("Components" INTERFACE ${Components})