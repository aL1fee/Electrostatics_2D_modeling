#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
uniform vec4 horiz_offset;

void main()
{
	gl_Position = vec4(-aPos.x + horiz_offset.x,
	-aPos.y + horiz_offset.x,
	aPos.z + horiz_offset.z, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;
uniform vec4 the_color;

void main()
{
	FragColor = the_color;
}
