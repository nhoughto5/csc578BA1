set(SHADER_SOURCE
	"${Cloth_SHADER_ROOT}/ball.vs.glsl"
	"${Cloth_SHADER_ROOT}/ball.fs.glsl"
	"${Cloth_SHADER_ROOT}/spline.vs.glsl"
	"${Cloth_SHADER_ROOT}/spline.fs.glsl"
	"${Cloth_SHADER_ROOT}/Grid.vs.glsl"
	"${Cloth_SHADER_ROOT}/Grid.fs.glsl"
	"${Cloth_SHADER_ROOT}/Axis.vs.glsl"
	"${Cloth_SHADER_ROOT}/Axis.fs.glsl"
    "${Cloth_SHADER_ROOT}/Cloth.fs.glsl"
    "${Cloth_SHADER_ROOT}/Cloth.vs.glsl")

set_source_files_properties(${SHADER_SOURCE} 
    PROPERTIES HEADER_FILE_ONLY TRUE)

set(SHADER_INCLUDE "${Cloth_GENERATED_ROOT}/ShaderPaths.hpp")

configure_file(
    "${Cloth_SHADER_ROOT}/ShaderPaths.hpp.in"
    ${SHADER_INCLUDE})

set(SHADER_LIST
    ${SHADER_SOURCE}
    ${SHADER_INCLUDE}
    PARENT_SCOPE)

