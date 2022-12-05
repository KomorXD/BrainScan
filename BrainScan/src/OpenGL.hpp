#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define ASSERT(x) if(!(x)) __debugbreak()
#define GLCall(f) GLClearErrors();\
	f;\
	ASSERT(GLCheckForError(#f, __FILENAME__, __LINE__))

void GLClearErrors();

bool GLCheckForError(const char* func, const char* file, int line);

class VertexBuffer
{
	public:
		VertexBuffer(const void* data, uint32_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void UpdateBuffer(const void* data, uint32_t size);

	private:
		uint32_t m_ID;
};

class IndexBuffer
{
	public:
		IndexBuffer(const uint32_t* data, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_ID;
		uint32_t m_Count;
};

struct VertexBufferElement
{
	uint32_t type;
	uint32_t count;
	uint8_t  normalized;

	static uint32_t GetSizeOfType(uint32_t t);
};

class VertexBufferLayout
{
	public:
		VertexBufferLayout();

		template<typename T>
		void Push(uint32_t count)
		{
			static_assert(true);
		}

		template<>
		void Push<float>(uint32_t count)
		{
			m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
			m_Stride += count * sizeof(GLfloat);
		}

		template<>
		void Push<uint32_t>(uint32_t count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			m_Stride += count * sizeof(GLuint);
		}

		template<>
		void Push<uint8_t>(uint32_t count)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
			m_Stride += count * sizeof(GLbyte);
		}

		void Clear();

		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		inline const uint32_t GetStride() const { return m_Stride; }

	private:
		std::vector<VertexBufferElement> m_Elements;
		uint32_t m_Stride;
};

class VertexArray
{
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout, uint32_t divisor = 0);

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_ID;
		uint32_t m_CurrentAttribID = 0;
};

class Shader
{
	public:
		Shader(const std::string& vs, const std::string& fs);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
		void SetUniformVec3(const std::string& name, const glm::vec3& vec);
		void SetUniformVec2(const std::string& name, const glm::vec2& vec);

	private:
		uint32_t m_ID;
		std::unordered_map<std::string, int32_t> m_UniformLocations;

		std::string ShaderParse(const std::string& filepath);
		uint32_t ShaderCompile(uint32_t type, const std::string& sourceCode);
		uint32_t ShaderCreate(const std::string& vs, const std::string& fs);

		int32_t GetUniformLocation(const std::string& name);
};

class Texture
{
	public:
		Texture(const std::string& filepath);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		int GetWidth() const;
		int GetHeight() const;
		inline unsigned int GetID() const { return m_RendererID; }

		std::string GetPath() const;

	private:
		unsigned int   m_RendererID;
		unsigned char* m_LocalBuffer;
		int			   m_Width;
		int			   m_Height;
		int			   m_BPP;
		std::string	   m_Path;
};