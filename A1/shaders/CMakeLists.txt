set(SHADER_SOURCE
    "${A1_SHADER_ROOT}/angularSpring.fs.glsl"
    "${A1_SHADER_ROOT}/angularSpring.vs.glsl"
	"${A1_SHADER_ROOT}/Spring.fs.glsl"
    "${A1_SHADER_ROOT}/Spring.vs.glsl")

set_source_files_properties(${SHADER_SOURCE} 
    PROPERTIES HEADER_FILE_ONLY TRUE)

set(SHADER_INCLUDE "${A1_GENERATED_ROOT}/ShaderPaths.hpp")

configure_file(
    "${A1_SHADER_ROOT}/ShaderPaths.hpp.in"
    ${SHADER_INCLUDE})

set(SHADER_LIST
    ${SHADER_SOURCE}
    ${SHADER_INCLUDE}
    PARENT_SCOPE)

