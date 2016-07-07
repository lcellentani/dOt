#ifndef __DOT_SAMPLES_TEAPOT_H_HEADER_GUARD__
#define __DOT_SAMPLES_TEAPOT_H_HEADER_GUARD__

#include "drawable.h"

#include <stdint.h> // uint32_t
#include <glm.hpp>

namespace dot
{
namespace gl
{
namespace samples
{


class Teapot : public Drawable
{
public:
    Teapot(int grid, const glm::mat4& lidTransform);
	virtual ~Teapot();

	virtual void Render() const override;

private:
	void GeneratePatches(float * v, float * n, float *tc, unsigned int* el, int grid);
	void BuildPatchReflect(int patchNum, float *B, float *dB, float *v, float *n, float *, unsigned int *el, int &index, int &elIndex, int &, int grid, bool reflectX, bool reflectY);
	void BuildPatch(glm::vec3 patch[][4], float *B, float *dB, float *v, float *n, float *, unsigned int *el, int &index, int &elIndex, int &, int grid, glm::mat3 reflect, bool invertNormal);
	void GetPatch(int patchNum, glm::vec3 patch[][4], bool reverseV);

	void ComputeBasisFunctions(float * B, float * dB, int grid);
	glm::vec3 Evaluate(int gridU, int gridV, float *B, glm::vec3 patch[][4]);
	glm::vec3 EvaluateNormal(int gridU, int gridV, float *B, float *dB, glm::vec3 patch[][4]);
	void MoveLid(int, float *, const glm::mat4 &);

private:
	uint32_t mVaoHandle;
	uint32_t mFaces;
};

} // samples
} // gl
} // dot

#endif // __DOT_SAMPLES_TEAPOT_H_HEADER_GUARD__
