#================================
# Directory variables.
#================================
set(A1_ROOT "${ANIM_SOURCE_DIR}/A1")
set(A1_INCLUDE_ROOT "${A1_ROOT}/include")
set(A1_SOURCE_ROOT "${A1_ROOT}/source")
set(A1_SHADER_ROOT "${A1_ROOT}/shaders")
set(A1_GENERATED_ROOT "${A1_ROOT}/generated")

#================================
# Add subdirectories.
#================================
add_subdirectory(${A1_INCLUDE_ROOT})
add_subdirectory(${A1_SOURCE_ROOT})
add_subdirectory(${A1_SHADER_ROOT})
#================================
# Include directories.
#================================
include_directories(
    ${ATLAS_INCLUDE_DIRS}
    ${A1_INCLUDE_ROOT}
    ${A1_GENERATED_ROOT})

#================================
# Source groups.
#================================
source_group("include" FILES ${INCLUDE_LIST})
source_group("source" FILES ${SOURCE_LIST})
source_group("shaders" FILES ${SHADER_LIST})

add_executable(A1 ${SOURCE_LIST} ${INCLUDE_LIST} ${SHADER_LIST})
target_link_libraries(A1 ${ATLAS_LIBRARIES})

