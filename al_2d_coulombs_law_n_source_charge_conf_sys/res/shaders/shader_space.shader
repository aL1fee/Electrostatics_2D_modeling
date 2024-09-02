#shader vertex
#version 330 core

layout (location = 0) in vec3 pos;

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);
}

#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(.1f, .5f, .9f, 1.0f);
}
