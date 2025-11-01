################################################################################
##
##  YOUR QUILL
##
################################################################################

add_executable(lualua lualua.cpp)
target_link_libraries(lualua yq_core yq_lua yq_process)

if(Build_Vulkan)
    add_executable(luavk luavk.cpp)
    target_link_libraries(luavk 
    PUBLIC
        yq_tachyon yq_lua # yq_luavk
    PRIVATE
        ImGuiFileDialog
    )
endif()
