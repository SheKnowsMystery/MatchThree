
# Variable for evading copy-paste issues
set(current ${Application})

# Template uses "current" variable to set up each part similarly
include(../../templates/app.cmake)

target_link_libraries(${current} PRIVATE "Core")
