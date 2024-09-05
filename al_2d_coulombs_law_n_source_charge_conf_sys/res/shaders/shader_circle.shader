#shader vertex
#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(pos, 1.0f);
}

#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(.75f, .75f, .136f, 1.0f);
}
