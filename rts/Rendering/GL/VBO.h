/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef GL_VBO_H
#define GL_VBO_H

#include "Rendering/GL/myGL.h"

/**
 * @brief VBO
 *
 * Vertex buffer Object class (ARB_vertex_buffer_object).
 */
class VBO
{
public:
	VBO(GLenum _defTarget = GL_ARRAY_BUFFER, const bool storage = false);
	VBO(const VBO& other) = delete;
	VBO(VBO&& other) { *this = std::move(other); }
	virtual ~VBO();

	VBO& operator = (const VBO& other) = delete;
	VBO& operator = (VBO&& other);

	void Generate() const { glGenBuffers(1, &vboId); }
	void Delete() const { glDeleteBuffers(1, &vboId); vboId = 0; }

	/**
	 * @param target can be either GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER or GL_UNIFORM_BUFFER_EXT
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	 */
	void Bind() const { Bind(defTarget); }
	void Bind(GLenum target) const;
	void Unbind() const;

	/**
	 * @param usage can be either GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY
	 * @param data (optional) initialize the VBO with the data (the array must have minimum `size` length!)
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	 */
	void Resize(GLsizeiptr newSize, GLenum newUsage = GL_STREAM_DRAW);
	void New(GLsizeiptr newSize, GLenum newUsage = GL_STREAM_DRAW, const void* newData = nullptr);
	void Invalidate(); //< discards all current data (frees the memory w/o resizing)

	/**
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glMapBufferRange.xml
	 */
	GLubyte* MapBuffer(GLbitfield access = GL_WRITE_ONLY);
	GLubyte* MapBuffer(GLintptr offset, GLsizeiptr size, GLbitfield access = GL_WRITE_ONLY);

	void UnmapBuffer();
	void UnmapIf() {
		if (!mapped)
			return;

		Bind();
		UnmapBuffer();
		Unbind();
	}


	GLuint GetId() const {
		if (vboId == 0)
			Generate();
		return vboId;
	}

	size_t GetSize() const { return bufSize; }
	const GLvoid* GetPtr(GLintptr offset = 0) const;

public:
	mutable GLuint vboId = 0;
	size_t bufSize = 0; // can be smaller than memSize
	size_t memSize = 0; // actual length of <data>; only set when !isSupported

	mutable GLenum curBoundTarget = 0;
	GLenum defTarget = GL_ARRAY_BUFFER;
	GLenum usage = GL_STREAM_DRAW;

public:
	mutable bool bound = false;
	bool mapped = false;
	bool nullSizeMapped = false; // Nvidia workaround
	bool immutableStorage = false;
};

#endif /* VBO_H */
