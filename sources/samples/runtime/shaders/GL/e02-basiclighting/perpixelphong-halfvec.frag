#version 410

in vec4 Position;
in vec3 Normal;

layout(location = 0) out vec4 FragColor;

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

vec3 phongModel(in vec4 position, in vec3 normal)
{
	vec3 n = normalize(normal);
	vec3 s = normalize(vec3(Light.Position - position));
    vec3 v = normalize(-position.xyz);
    vec3 h = normalize(v + s);
    float sDotN = max(dot(s, normal), 0.0);

    vec3 ambient = Light.La * Material.Ka;

    vec3 diffuse = Light.Ld * Material.Kd * sDotN;

    vec3 spec = vec3(0.0);
    if (sDotN > 0.0)
	{
       spec = Light.Ls * Material.Ks * pow(max(dot(h, n), 0.0), Material.Shininess);
	}

	return ambient + diffuse + spec;
}

void main()
{
	vec3 lightIntensity = phongModel(Position, Normal);
    FragColor = vec4(lightIntensity, 1.0);
}
