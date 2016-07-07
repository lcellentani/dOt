#version 410

subroutine vec3 shadeModelType(in vec4 position, in vec3 normal);
subroutine uniform shadeModelType shadeModel;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

struct LightInfo
{
  vec4 Position; // Light position in eye coords.
  vec3 La;       // Ambient light intensity
  vec3 Ld;       // Diffuse light intensity
  vec3 Ls;       // Specular light intensity
};
uniform LightInfo Light;

struct MaterialInfo
{
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void computeEyeSpace(out vec4 position, out vec3 normal)
{
	normal = normalize(NormalMatrix * VertexNormal);
	position = ModelViewMatrix * vec4(VertexPosition, 1.0);
}

subroutine(shadeModelType)
vec3 phongModel(in vec4 position, in vec3 normal)
{
	vec3 s = normalize(vec3(Light.Position - position));
    vec3 v = normalize(-position.xyz);
    vec3 r = reflect(-s, normal);
    float sDotN = max(dot(s, normal), 0.0);

    vec3 ambient = Light.La * Material.Ka;

    vec3 diffuse = Light.Ld * Material.Kd * sDotN;

    vec3 spec = vec3(0.0);
    if (sDotN > 0.0)
	{
       spec = Light.Ls * Material.Ks * pow(max(dot(r, v), 0.0), Material.Shininess);
	}

	return ambient + diffuse + spec;
}

subroutine(shadeModelType)
vec3 lambertModel(in vec4 position, in vec3 normal)
{
	vec3 ambient = Light.La * Material.Ka;

	vec3 s = normalize(vec3(Light.Position - position));

	float sDotN = max(dot(s, normal), 0.0);
	vec3 diffuse = Light.Ld * Material.Kd * sDotN;

	return ambient + diffuse;
}

void main()
{
	vec3 eyeNormal;
    vec4 eyePosition;

    // Get the position and normal in eye space
	computeEyeSpace(eyePosition, eyeNormal);

	// Evaluate the lighting equation.
    LightIntensity = shadeModel(eyePosition, eyeNormal);

    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
