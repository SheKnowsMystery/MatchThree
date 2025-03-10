
# Variable for evading copy-paste issues
set(current "SDL")

# Template uses "current" variable to set up each part similarly
include(../../../templates/part.cmake)

target_link_libraries(${current} PRIVATE "Utility")
target_link_libraries(${current} PUBLIC $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
target_link_libraries(${current} PUBLIC $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
target_link_libraries(${current} INTERFACE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>)