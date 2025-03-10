
# Variable for evading copy-paste issues
set(current "Core")

# Template uses "current" variable to set up each part similarly
include(../../templates/part.cmake)

target_link_libraries(${current} PUBLIC "Utility")
target_link_libraries(${current} PUBLIC "Components")
