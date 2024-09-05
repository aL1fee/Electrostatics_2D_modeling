#shader vertex
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

uniform mat4 projection;

out vec3 color;

void main()
{
	gl_Position = projection * vec4(pos.x, pos.y, pos.z, 1.0f);
	color = col;
}

#shader fragment
#version 330 core

in vec3 color;
out vec4 FragColor;

void main()
{
	FragColor = vec4(color, 1.0f);
}
