#version 330

layout(location = 0) out vec4 out_color;

//in vec3 color;
in vec3 world_position;
in vec3 world_normal;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

void main()
{
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);

	float ambient_light = 0.25;

	float diffuse_light = material_kd * max(dot(world_normal,L), 0);

	float specular_light = 0;

	if(diffuse_light > 0) {
		specular_light = material_ks * pow(max(dot(world_normal, H), 0), material_shininess);
	}
	float dis = distance(world_position, light_position) * distance(world_position, light_position);
	float atenuare = 1/dis;
	float light = ambient_light + atenuare * 40 * (diffuse_light + specular_light);

	vec3 color = vec3(object_color * light);
	out_color = vec4(color,1);
}