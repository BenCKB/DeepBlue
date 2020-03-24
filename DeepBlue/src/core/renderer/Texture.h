#pragma once
#include <memory>
#include <string>

#include "../../vendor/stb_image/stb_image.h"
#include "glad/glad.h"
#include "../../util/Log.h"

class Texture {
public:
	Texture(const std::string& file, int slot);

	~Texture();

	void bind();

	void bind(int index);

	void unbind();

	void unbind(int index);

	unsigned int texID = 0;
	int texWidth = 0;
	int texHeight = 0;
	int texChannels = 0;
	unsigned char* data = nullptr;
};