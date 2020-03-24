#version 420 core
out vec4 FragColor;
  
in vec3 Normal;
in vec3 VecPosition;
in vec3 lightPos;

void main()
{
    FragColor = vec4(0.6, 0.8, 0.9, 1.0);
} 