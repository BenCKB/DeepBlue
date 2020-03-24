#version 420 core
 
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 TexCoord;

out vec3 Normal;
out vec3 VecPosition;
out vec3 lightPos;
out vec2 TexCoord0;

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

void main()
{
    //Calculate certex position
    mat4 mv = u_v * u_m;
    gl_Position = u_p * mv * vec4(pos, 1.0);

    mat3 normal_matrix = transpose(inverse(mat3(mv)));

    //Set normal vector
    Normal = normalize(normal_matrix * norm);

    //Set texture coordinates
    TexCoord0 = TexCoord;

	//Directional lightPos
	//lightPos = normalize((u_v * normalize(vec4(0, 0, 50, 0.0))).xyz); //Before projection multiply w=1

    //Set vertex position
    //VecPosition = gl_Position.xyz;
    VecPosition = vec3(mv * vec4(pos, 1.0));
}  