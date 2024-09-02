#include "Texture.h"

Texture::Texture(const string& path)
	: _RendererID(0), _Filepath(path), _LocalBuffer(nullptr),
	_Width(0), _Height(0), _BPP(0)
{
	//flips the texure upside down because OpenGL expects to start our texture pixels to start in the bottom left.
	stbi_set_flip_vertically_on_load(1);

	_LocalBuffer = stbi_load(path.c_str(), &_Width, &_Height, &_BPP, 4);

	if (!_LocalBuffer) {
		cout << "Texture couldn't be loaded!" << endl;
		return;
	}

	glGenTextures(1, &_RendererID);
	glBindTexture(GL_TEXTURE_2D, _RendererID);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Once glTexImage2D is called, the currently bound texture object now has the texture image
	//attached to it.However, currently it only has the base - level of the texture image loaded and if
	//we want to use mipmaps we have to specify all the different images manually(by continually
	//incrementing the second argument) or , we could call glGenerateMipmap after generating the
	//texture.This will automatically generate all the required mipmaps for the currently bound texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _Width, _Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _LocalBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (_LocalBuffer) {
		stbi_image_free(_LocalBuffer);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &_RendererID);
}

void Texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _RendererID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
