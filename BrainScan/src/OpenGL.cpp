#include "OpenGL.hpp"
#include "stbi_image/stb_image.hpp"

#include <sstream>
#include <iostream>
#include <string>

void GLClearErrors()
{
	while(glGetError() != GL_NO_ERROR);
}

bool GLCheckForError(const char* func, const char* file, int line)
{
	if(GLenum error = glGetError())
	{
		std::cout << "[OpenGL ERROR " << error << "]: " << file << ":" << line << " while calling " << func << std::endl;

		return false;
	}

	return true;
}

VertexBuffer::VertexBuffer(const void* data, uint32_t size)
	: m_ID(0)
{
	GLCall(glGenBuffers(1, &m_ID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	if(m_ID != 0)
	{
		GLCall(glDeleteBuffers(1, &m_ID));
	}
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::UpdateBuffer(const void* data, uint32_t size)
{
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
	: m_ID(0), m_Count(count)
{
	GLCall(glGenBuffers(1, &m_ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_DYNAMIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	if(m_ID != 0)
	{
		GLCall(glDeleteBuffers(1, &m_ID));
	}
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

VertexArray::VertexArray()
	: m_ID(0)
{
	GLCall(glGenVertexArrays(1, &m_ID));
	GLCall(glBindVertexArray(m_ID));
}

VertexArray::~VertexArray()
{
	if(m_ID != 0)
		GLCall(glDeleteVertexArrays(1, &m_ID));
}

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout, uint32_t divisor)
{
	Bind();
	vbo.Bind();

	const auto& elements = layout.GetElements();
	uint32_t offset = 0;

	for(uint32_t i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];

		GLCall(glEnableVertexAttribArray(i + m_CurrentAttribID));
		GLCall(glVertexAttribPointer(i + m_CurrentAttribID, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		GLCall(glVertexAttribDivisor(i + m_CurrentAttribID, divisor));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

	m_CurrentAttribID += elements.size();
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_ID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

Shader::Shader(const std::string& vs, const std::string& fs)
	: m_ID(0)
{
	m_ID = ShaderCreate(ShaderParse(vs), ShaderParse(fs));
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_ID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_ID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformVec3(const std::string& name, const glm::vec3& vec)
{
	GLCall(glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z));
}

void Shader::SetUniformVec2(const std::string& name, const glm::vec2& vec)
{
	GLCall(glUniform2f(GetUniformLocation(name), vec.x, vec.y));
}

std::string Shader::ShaderParse(const std::string& filepath)
{
	FILE* file;

	fopen_s(&file, filepath.c_str(), "r");

	if(!file)
	{
		std::cout << "[ERROR] Unable to open file: " << filepath << std::endl;

		return "";
	}

	char line[512];
	std::stringstream ss;

	while(fgets(line, 512, file) != NULL)
		ss << line;

	fclose(file);

	return ss.str();
}

uint32_t Shader::ShaderCompile(uint32_t type, const std::string& sourceCode)
{
	GLCall(uint32_t id = glCreateShader(type));
	const char* src = sourceCode.c_str();
	int result = 0;

	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if(result == GL_FALSE)
	{
		int len = 0;

		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));

		char* message = (char*)_malloca(len * sizeof(char));

		GLCall(glGetShaderInfoLog(id, len, &len, message));

		std::cout << "[ERROR] Failed to compile shader:\n" << message << ".\n";

		GLCall(glDeleteShader(id));

		return 0;
	}

	return id;
}

uint32_t Shader::ShaderCreate(const std::string& vs, const std::string& fs)
{
	GLCall(uint32_t program = glCreateProgram());
	uint32_t vsID = ShaderCompile(GL_VERTEX_SHADER, vs);
	uint32_t fsID = ShaderCompile(GL_FRAGMENT_SHADER, fs);

	GLCall(glAttachShader(program, vsID));
	GLCall(glAttachShader(program, fsID));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	int success = 0;

	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &success));

	if(success == GL_FALSE)
	{
		int len = 0;

		GLCall(glGetShaderiv(program, GL_INFO_LOG_LENGTH, &len));

		char* message = (char*)_malloca(len * sizeof(char));

		GLCall(glGetShaderInfoLog(program, len, &len, message));

		std::cout << "[ERROR] Failed to link shaders:\n" << message << ".\n";

		GLCall(glDeleteShader(program));

		return 0;
	}

	GLCall(glDeleteShader(vsID));
	GLCall(glDeleteShader(fsID));

	return program;
}

int32_t Shader::GetUniformLocation(const std::string& name)
{
	if(m_UniformLocations.find(name) != m_UniformLocations.end())
		return m_UniformLocations[name];

	GLCall(int32_t location = glGetUniformLocation(m_ID, name.c_str()));

	if(location == -1)
		std::cout << "[WARNING] Uniform " << name << " does not exist.\n";
	else
		m_UniformLocations[name] = location;

	return location;
}

VertexBufferLayout::VertexBufferLayout()
	: m_Stride(0)
{}

void VertexBufferLayout::Clear()
{
	m_Elements.clear();
	m_Stride = 0;
}

uint32_t VertexBufferElement::GetSizeOfType(uint32_t t)
{
	switch(t)
	{
		case GL_FLOAT:			return sizeof(GLfloat);
		case GL_UNSIGNED_INT:	return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:	return sizeof(GLbyte);
	}

	return 0;
}

Texture::Texture(const std::string& filepath)
	: m_RendererID(0), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), m_Path(filepath)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if(m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

int Texture::GetWidth() const
{
	return m_Width;
}

int Texture::GetHeight() const
{
	return m_Height;
}

std::string Texture::GetPath() const
{
	return m_Path;
}