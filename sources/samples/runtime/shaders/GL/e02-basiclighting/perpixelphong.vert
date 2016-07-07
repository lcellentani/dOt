#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec4 Position;
out vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void computeEyeSpace(out vec4 position, out vec3 normal)
{
	normal = normalize(NormalMatrix * VertexNormal);
	position = ModelViewMatrix * vec4(VertexPosition, 1.0);
}

void main()
{
	vec3 eyeNormal;
    vec4 eyePosition;

    // Get the position and normal in eye space
	computeEyeSpace(eyePosition, eyeNormal);

    gl_Position = MVP * vec4(VertexPosition, 1.0);

	Position = eyePosition;
	Normal = eyeNormal;
}
