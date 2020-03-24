#include "Texture.h"

Texture::Texture(const std::string& file, int slot) {
	stbi_set_flip_vertically_on_load(true); //OpenGL Starts at bottom left, unlike stb_image space, which is top left
	data = stbi_load(file.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);//STBI_rgb_alpha

	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texID);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		LOG_WARN("Failed to load texture");
	}

	stbi_image_free(data);
}

void Texture::bind() {
	bind(0);
}

void Texture::bind(int index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::unbind() {
	unbind(0);
}

void Texture::unbind(int index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {}