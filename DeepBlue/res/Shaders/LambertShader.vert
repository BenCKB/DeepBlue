#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 TexCoord;

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

out vec3 Normal;
out vec3 VecPosition;
out vec3 lightPos;

void main()
{
    //Calculate certex position
    mat4 mv = u_v * u_m;
    gl_Position = u_p * mv * vec4(pos, 1.0);

    mat3 normal_matrix = transpose(inverse(mat3(mv)));

    //Set normal vector
    Normal = normalize(normal_matrix * norm);
    
    //Calculate light position  
    lightPos = (u_v * vec4(0.0, 70.0, 50.0, 1.0)).xyz;

    //Set vertex position
    VecPosition = vec3(mv * vec4(pos, 1.0));
}