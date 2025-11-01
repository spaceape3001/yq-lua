################################################################################
##
##  YOUR QUILL
##
################################################################################

include(yq_lua.cmake)

if(Build_Qt6)
    # If we do a Qt binding... it'll be here
endif()
if(Build_Vulkan)
    include(yq_luavk.cmake)
endif()
