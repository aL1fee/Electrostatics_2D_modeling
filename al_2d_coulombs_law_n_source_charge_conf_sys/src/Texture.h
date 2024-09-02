#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

#include "vendor/stb_image/stb_image.h"

using namespace std;

class Texture
{
private:
	unsigned int _RendererID;
	string _Filepath;
	unsigned char* _LocalBuffer;
	int _Width, _Height, _BPP;

public:
	Texture(const string& path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return _Width; }
	inline int getHeight() const { return _Height; }




};