#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define ASSERT(x) if(!(x)) __debugbreak()
#ifdef BS_DEBUG
#define GLCall(f) GLClearErrors();\
	f;\
	ASSERT(GLCheckForError(#f, __FILENAME__, __LINE__))
#else
#define GLCall(f) f
#endif

void GLClearErrors();

bool GLCheckForError(const char* func, const char* file, int line);

class VertexBuffer
{
	public:
		VertexBuffer(const void* data, uint32_t size);
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer(VertexBuffer&& other) noexcept;
		~VertexBuffer();

		VertexBuffer& operator= (const VertexBuffer& other) = delete;
		VertexBuffer& operator= (VertexBuffer&& other) noexcept;

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
		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer(IndexBuffer&& other) noexcept;
		~IndexBuffer();

		IndexBuffer& operator= (const IndexBuffer& other) = delete;
		IndexBuffer& operator= (IndexBuffer&& other) noexcept;

		void Bind() const;
		void Unbind() const;

		void UpdateBuffer(const uint32_t* data, uint32_t count);

		inline const uint32_t GetCount() const { return m_Count; }

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
			// static_assert(false);
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
		VertexArray(const VertexArray& other) = delete;
		VertexArray(VertexArray&& other) noexcept;
		~VertexArray();

		VertexArray& operator= (const VertexArray& other) = delete;
		VertexArray& operator= (VertexArray&& other) noexcept;

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_ID;
};

class Shader
{
	public:
		Shader(const std::string& vs, const std::string& fs);
		Shader(const std::string& vs, const std::string& gs, const std::string& fs);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform1i(const std::string& name, int32_t val);
		void SetUniform1f(const std::string& name, float val);

		void ReloadShader();

	private:
		std::string m_VertexShaderPath;
		std::string m_FragmentShaderPath;
		std::string m_GeometryShaderPath;

		uint32_t m_ID;
		std::unordered_map<std::string, int32_t> m_UniformLocations;

		std::string ShaderParse(const std::string& filepath);
		uint32_t ShaderCompile(uint32_t type, const std::string& sourceCode);
		uint32_t ShaderCreate(const std::string& vs, const std::string& fs);
		uint32_t ShaderCreate(const std::string& vs, const std::string& gs, const std::string& fs);

		int32_t GetUniformLocation(const std::string& name);
};

class Texture
{
	public:
		Texture(const std::string& filepath);
		Texture(float* data, uint32_t width, uint32_t height);
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

class Framebuffer
{
	public:
		Framebuffer();
		Framebuffer(const Framebuffer& other) = delete;
		Framebuffer(Framebuffer&& other) noexcept;
		~Framebuffer();

		Framebuffer& operator= (const Framebuffer& other) = delete;
		Framebuffer& operator= (Framebuffer&& other) noexcept;

		void AttachTexture(uint32_t width, uint32_t height);
		void AttachRenderBuffer(uint32_t width, uint32_t height);

		void BindBuffer()					const;
		void BindTexture(uint32_t slot = 0)	const;
		void BindRenderBuffer()				const;
		void UnbindBuffer()					const;
		void UnbindTexture()				const;
		void UnbindRenderBuffer()			const;

		inline uint32_t GetTextureID() const { return m_TextureID; }

		bool IsComplete() const;

	private:
		uint32_t m_ID			  = 0;
		uint32_t m_TextureID	  = 0;
		uint32_t m_RenderbufferID = 0;
};