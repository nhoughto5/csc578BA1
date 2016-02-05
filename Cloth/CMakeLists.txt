#================================
# Directory variables.
#================================
set(Cloth_ROOT "${ANIM_SOURCE_DIR}/Cloth")
set(Cloth_INCLUDE_ROOT "${Cloth_ROOT}/include")
set(Cloth_SOURCE_ROOT "${Cloth_ROOT}/source")
set(Cloth_SHADER_ROOT "${Cloth_ROOT}/shaders")
set(Cloth_GENERATED_ROOT "${Cloth_ROOT}/generated")

#================================
# Add subdirectories.
#================================
add_subdirectory(${Cloth_INCLUDE_ROOT})
add_subdirectory(${Cloth_SOURCE_ROOT})
add_subdirectory(${Cloth_SHADER_ROOT})
#================================
# Include directories.
#================================
include_directories(
    ${ATLAS_INCLUDE_DIRS}
    ${Cloth_INCLUDE_ROOT}
    ${Cloth_GENERATED_ROOT})

#================================
# Source groups.
#================================
source_group("include" FILES ${INCLUDE_LIST})
source_group("source" FILES ${SOURCE_LIST})
source_group("shaders" FILES ${SHADER_LIST})

add_executable(Cloth ${SOURCE_LIST} ${INCLUDE_LIST} ${SHADER_LIST})
target_link_libraries(Cloth ${ATLAS_LIBRARIES})

