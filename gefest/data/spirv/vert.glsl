#version 400
#extension GL_ARB_separate_shader_objects: enable
#extension GL_ARB_shading_language_420pack: enable

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() 
{
   gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}
