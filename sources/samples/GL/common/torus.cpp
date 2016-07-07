#include "torus.h"

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

Torus::Torus(float outerRadius, float innerRadius, uint32_t nsides, uint32_t nrings) : Drawable(), mRings(nrings), mSides(nsides)
{
    mFaces = mSides * mRings;
	// One extra ring to duplicate first ring
    int nVerts  = mSides * (mRings + 1);

    // Verts
    float * v = new float[3 * nVerts];
    // Normals
    float * n = new float[3 * nVerts];
    // Tex coords
    float * tex = new float[2 * nVerts];
    // Elements
    unsigned int * el = new unsigned int[6 * mFaces];

    // Generate the vertex data
    GenerateVerts(v, n, tex, el, outerRadius, innerRadius);

    // Create and populate the buffer objects
    unsigned int handle[4];
    glGenBuffers(4, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(float), v, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(float), n, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
    glBufferData(GL_ARRAY_BUFFER, (2 * nVerts) * sizeof(float), tex, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * mFaces * sizeof(unsigned int), el, GL_STATIC_DRAW);

    delete [] v;
    delete [] n;
    delete [] el;
    delete [] tex;

    // Create the VAO
    glGenVertexArrays(1, &mVaoHandle);
    glBindVertexArray(mVaoHandle);

    glEnableVertexAttribArray(0);  // Vertex position
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

    glEnableVertexAttribArray(1);  // Vertex normal
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer( (GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

    glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
    glEnableVertexAttribArray(2);  // Texture coords
    glVertexAttribPointer( (GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);

    glBindVertexArray(0);
}

Torus::~Torus()
{
	if (mVaoHandle != 0)
	{
		glDeleteVertexArrays(1, &mVaoHandle);
		mVaoHandle = 0;
	}
}

void Torus::Render() const
{
    glBindVertexArray(mVaoHandle);
    glDrawElements(GL_TRIANGLES, 6 * mFaces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
}

void Torus::GenerateVerts(float * verts, float * norms, float * tex, unsigned int * el, float outerRadius, float innerRadius)
{
	float ringFactor = glm::two_pi<float>() / mRings;
	float sideFactor = glm::two_pi<float>() / mSides;
    int idx = 0, tidx = 0;
	for (uint32_t ring = 0; ring <= mRings; ring++)
	{
		float u = ring * ringFactor;
		float cu = cos(u);
		float su = sin(u);
		for (uint32_t side = 0; side < mSides; side++)
		{
			float v = side * sideFactor;
			float cv = cos(v);
			float sv = sin(v);
			float r = (outerRadius + innerRadius * cv);
			verts[idx] = r * cu;
			verts[idx + 1] = r * su;
			verts[idx + 2] = innerRadius * sv;
			norms[idx] = cv * cu * r;
			norms[idx + 1] = cv * su * r;
			norms[idx + 2] = sv * r;
			tex[tidx] = u / glm::two_pi<float>();
			tex[tidx + 1] = v / glm::two_pi<float>();
			tidx += 2;
			// Normalize
			float len = sqrt(norms[idx] * norms[idx] + norms[idx + 1] * norms[idx + 1] + norms[idx + 2] * norms[idx + 2]);
			norms[idx] /= len;
			norms[idx + 1] /= len;
			norms[idx + 2] /= len;
			idx += 3;
		}
	}

    idx = 0;
	for (uint32_t ring = 0; ring < mRings; ring++)
	{
		int ringStart = ring * mSides;
		int nextRingStart = (ring + 1) * mSides;
		for (uint32_t side = 0; side < mSides; side++)
		{
			int nextSide = (side + 1) % mSides;
			// The quad
			el[idx] = (ringStart + side);
			el[idx + 1] = (nextRingStart + side);
			el[idx + 2] = (nextRingStart + nextSide);
			el[idx + 3] = ringStart + side;
			el[idx + 4] = nextRingStart + nextSide;
			el[idx + 5] = (ringStart + nextSide);
			idx += 6;
		}
	}
}

} // samples
} // gl
} // dot
