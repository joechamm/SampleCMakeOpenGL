#version 450
#define GLSLIFY 1

in vec2 aPosition;
in vec3 aColor;

out vec3 vColor;

void main()
{
	gl_Position = vec4(aPosition, 0.0, 1.0);
    vColor = aColor;
}