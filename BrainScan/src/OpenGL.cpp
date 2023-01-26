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

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
{
	*this = std::move(other);
}

VertexBuffer::~VertexBuffer()
{
	if (m_ID != 0)
		GLCall(glDeleteBuffers(1, &m_ID));
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
	if (this == &other)
		return *this;

	this->m_ID = other.m_ID;

	other.m_ID = 0;

	return *this;
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
	Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
	: m_ID(0), m_Count(count)
{
	GLCall(glGenBuffers(1, &m_ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_DYNAMIC_DRAW));
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
{
	*this = std::move(other);
}

IndexBuffer::~IndexBuffer()
{
	if (m_ID != 0)
		GLCall(glDeleteBuffers(1, &m_ID));
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
	if (this == &other)
		return *this;

	this->m_ID = other.m_ID;
	this->m_Count = other.m_Count;

	other.m_ID = 0;
	other.m_Count = 0;

	return *this;
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::UpdateBuffer(const uint32_t* data, uint32_t count)
{
	Bind();
	m_Count = count;
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_DYNAMIC_DRAW));
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

VertexBufferLayout::VertexBufferLayout()
	: m_Stride(0)
{}

VertexArray::VertexArray()
	: m_ID(0)
{
	GLCall(glGenVertexArrays(1, &m_ID));
	GLCall(glBindVertexArray(m_ID));
}

VertexArray::VertexArray(VertexArray&& other) noexcept
{
	*this = std::move(other);
}

VertexArray::~VertexArray()
{
	if (m_ID != 0)
		GLCall(glDeleteVertexArrays(1, &m_ID));
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this == &other)
		return *this;

	this->m_ID = other.m_ID;

	other.m_ID = 0;

	return *this;
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	uint32_t offset = 0;

	for (uint32_t i = 0; i < elements.size(); ++i)
	{
		const VertexBufferElement& element = elements[i];

		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
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
	: m_VertexShaderPath(vs), m_FragmentShaderPath(fs), m_ID(0)
{
	m_ID = ShaderCreate(ShaderParse(vs), ShaderParse(fs));
}

Shader::Shader(const std::string& vs, const std::string& gs, const std::string& fs)
	: m_VertexShaderPath(vs), m_GeometryShaderPath(gs), m_FragmentShaderPath(fs), m_ID(0)
{
	m_ID = ShaderCreate(ShaderParse(vs), ShaderParse(gs), ShaderParse(fs));
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

void Shader::SetUniform1i(const std::string& name, int32_t val)
{
	GLCall(glUniform1i(GetUniformLocation(name), val));
}

void Shader::SetUniform1f(const std::string& name, float val)
{
	GLCall(glUniform1f(GetUniformLocation(name), val));
}

void Shader::ReloadShader()
{
	Unbind();
	GLCall(glDeleteProgram(m_ID));

	if (m_GeometryShaderPath.empty())
	{
		m_ID = ShaderCreate(ShaderParse(m_VertexShaderPath), ShaderParse(m_FragmentShaderPath));

		return;
	}

	m_ID = ShaderCreate(ShaderParse(m_VertexShaderPath), ShaderParse(m_GeometryShaderPath), ShaderParse(m_FragmentShaderPath));
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

		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len));

		char* message = (char*)_malloca(len * sizeof(char));

		GLCall(glGetProgramInfoLog(program, len, &len, message));

		std::cout << "[ERROR] Failed to link shaders: " << message << ".\n";

		GLCall(glDeleteProgram(program));

		return 0;
	}

	GLCall(glDeleteShader(vsID));
	GLCall(glDeleteShader(fsID));

	return program;
}

uint32_t Shader::ShaderCreate(const std::string& vs, const std::string& gs, const std::string& fs)
{
	GLCall(uint32_t program = glCreateProgram());
	uint32_t vsID = ShaderCompile(GL_VERTEX_SHADER, vs);
	uint32_t gsID = ShaderCompile(GL_GEOMETRY_SHADER, gs);
	uint32_t fsID = ShaderCompile(GL_FRAGMENT_SHADER, fs);

	GLCall(glAttachShader(program, vsID));
	GLCall(glAttachShader(program, gsID));
	GLCall(glAttachShader(program, fsID));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	int success = 0;

	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &success));

	if (success == GL_FALSE)
	{
		int len = 0;

		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len));
		
		char* message = (char*)_malloca(len * sizeof(char));

		GLCall(glGetProgramInfoLog(program, len, &len, message));

		std::cout << "[ERROR] Failed to link shaders: " << message << ".\n";

		GLCall(glDeleteProgram(program));

		return 0;
	}

	GLCall(glDeleteShader(vsID));
	GLCall(glDeleteShader(gsID));
	GLCall(glDeleteShader(fsID));

	return program;
}

int32_t Shader::GetUniformLocation(const std::string& name)
{
	if(m_UniformLocations.contains(name))
		return m_UniformLocations[name];

	GLCall(int32_t location = glGetUniformLocation(m_ID, name.c_str()));

	if(location == -1)
		std::cout << "[WARNING] Uniform " << name << " does not exist.\n";
	else
		m_UniformLocations[name] = location;

	return location;
}

Texture::Texture(const std::string& filepath)
	: m_RendererID(0), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), m_Path(filepath)
{
	stbi_set_flip_vertically_on_load(0);
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

Texture::Texture(float* data, uint32_t width, uint32_t height)
	: m_RendererID(0), m_LocalBuffer(nullptr), m_Width(width), m_Height(height), m_BPP(0), m_Path("")
{
	if (!data)
	{
		return;
	}

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture()
{
	if(m_RendererID != 0)
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

Framebuffer::Framebuffer()
{
	GLCall(glGenFramebuffers(1, &m_ID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ID));
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
{
	*this = std::move(other);
}

Framebuffer::~Framebuffer()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glDeleteFramebuffers(1, &m_ID));

	if (m_TextureID != 0)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
		GLCall(glDeleteTextures(1, &m_TextureID));
	}

	if (m_RenderbufferID != 0)
	{
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		GLCall(glDeleteRenderbuffers(1, &m_RenderbufferID));
	}
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
{
	if (this == &other)
		return *this;

	this->m_ID = other.m_ID;
	this->m_TextureID = other.m_TextureID;
	this->m_RenderbufferID = other.m_RenderbufferID;

	other.m_ID = 0;
	other.m_TextureID = 0;
	other.m_RenderbufferID = 0;

	return *this;
}

void Framebuffer::AttachTexture(uint32_t width, uint32_t height)
{
	GLCall(glGenTextures(1, &m_TextureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Framebuffer::AttachRenderBuffer(uint32_t width, uint32_t height)
{
	GLCall(glGenRenderbuffers(1, &m_RenderbufferID));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderbufferID));

	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderbufferID));

	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void Framebuffer::BindBuffer() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ID));
}

void Framebuffer::BindTexture(uint32_t slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}

void Framebuffer::BindRenderBuffer() const
{
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderbufferID));
}

void Framebuffer::UnbindBuffer() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::UnbindTexture() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Framebuffer::UnbindRenderBuffer() const
{
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

bool Framebuffer::IsComplete() const
{
	GLCall(bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	return complete;
}
