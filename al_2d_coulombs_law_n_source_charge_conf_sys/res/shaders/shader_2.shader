#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color_out;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	color_out = aColor;
}

#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 color_out;

void main()
{
	FragColor = vec4(color_out, 1.0f);
}
