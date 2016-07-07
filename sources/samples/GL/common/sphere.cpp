#include "sphere.h"

#include "rendering/RendererGLConfig.h"

#include <cstdio>
#include <cmath>

#include <gtc/constants.hpp>

namespace dot
{
namespace gl
{
namespace samples
{

Sphere::Sphere(float radius, uint32_t slices, uint32_t stacks) : Drawable(), mRadius(radius), mSlices(slices), mStacks(stacks) 
{
    mNumVerts = (slices+1) * (stacks + 1);
    mNumElements = (slices * 2 * (stacks-1) ) * 3;

    // Verts
    float * v = new float[3 * mNumVerts];
    // Normals
    float * n = new float[3 * mNumVerts];
    // Tex coords
    float * tex = new float[2 * mNumVerts];
    // Elements
    uint32_t * el = new uint32_t[mNumElements];

    // Generate the vertex data
    GenerateVerts(v, n, tex, el);

    // Create and populate the buffer objects
    unsigned int handle[4];
    glGenBuffers(4, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, (3 * mNumVerts) * sizeof(float), v, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, (3 * mNumVerts) * sizeof(float), n, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
    glBufferData(GL_ARRAY_BUFFER, (2 * mNumVerts) * sizeof(float), tex, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumElements * sizeof(uint32_t), el, GL_STATIC_DRAW);

    delete [] v;
    delete [] n;
    delete [] el;
    delete [] tex;

    // Create the VAO
    glGenVertexArrays(1, &mVaoHandle);
    glBindVertexArray(mVaoHandle);

    glEnableVertexAttribArray(0);  // Vertex position
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);  // Vertex normal
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
    glEnableVertexAttribArray(2);  // Texture coords
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);

    glBindVertexArray(0);
}

void Sphere::Render() const
{
    glBindVertexArray(mVaoHandle);
    glDrawElements(GL_TRIANGLES, mNumElements, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
}

void Sphere::GenerateVerts(float * verts, float * norms, float * tex, uint32_t * el)
{
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / mSlices;
	GLfloat phiFac = glm::pi<float>() / mStacks;
	GLfloat nx, ny, nz, s, t;
	uint32_t idx = 0;
	uint32_t tIdx = 0;
	for (uint32_t i = 0; i <= mSlices; i++)
	{
		theta = i * thetaFac;
		s = (GLfloat)i / mSlices;
		for (GLuint j = 0; j <= mStacks; j++)
		{
			phi = j * phiFac;
			t = (GLfloat)j / mStacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = mRadius * nx;
			verts[idx + 1] = mRadius * ny;
			verts[idx + 2] = mRadius * nz;
			norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for (uint32_t i = 0; i < mSlices; i++)
	{
		GLuint stackStart = i * (mStacks + 1);
		GLuint nextStackStart = (i + 1) * (mStacks + 1);
		for (GLuint j = 0; j < mStacks; j++)
		{
			if (j == 0)
			{
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == mStacks - 1)
			{
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else
			{
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j + 1;
				el[idx + 3] = nextStackStart + j;
				el[idx + 4] = stackStart + j;
				el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}
}

} // samples
} // gl
} // dot
