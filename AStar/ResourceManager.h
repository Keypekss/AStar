#pragma once
#include <map>
#include <string>

#include <glad/glad.h>

#include "Texture.h"
#include "Shader.h"

class ResourceManager {
public:
	// resource storage
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;

	// loads and generates a shader program from file loading vertex, fragment and geometry shader's source code.
	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name);

	// retrieve loaded shader
	static Shader GetShader(std::string name);

	// loads and generates a texture from file
	static Texture LoadTexture(const char* file, bool alpha, std::string name);

	// retrieves a stored texture
	static Texture GetTexture(std::string name);

	// deallocate all loaded resources
	static void Clear();

private:
	// private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static)
	ResourceManager() {}

	// loads and generates a shader from file
	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile);

	// loads a single texture from file
	static Texture loadTextureFromFile(const char* file, bool alpha);

};

