#version 460 core
#define PI_RCP (0.31830988618379067153776752674503)

layout(location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform mat4 viewInv;
uniform mat4 projectionInv;

uniform mat4 lightView;
uniform mat4 lightProjection;

uniform sampler2D depthMap;
uniform sampler2D lightDepthMap;

struct DirectionalLight {
	vec3 position;
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight directionalLight;

uniform vec3 viewPos;

const int NUM_SAMPLES = 256;

float volumetric_lighting_directional(vec3 frag_pos);
vec3 world_pos_from_depth(float depth);

void main()
{
	float depth = texture(depthMap, TexCoords).r;
	
	vec3 frag_pos = world_pos_from_depth(depth);
	
	vec4 outColor = vec4(volumetric_lighting_directional(frag_pos) * directionalLight.diffuse, depth);

	FragColor = outColor;
}

float volumetric_lighting_directional(vec3 frag_pos)
{
	vec4 end_pos_worldspace  = vec4(viewPos, 1.0);
	vec4 start_pos_worldspace = vec4(frag_pos, 1.0);
	vec4 delta_worldspace = normalize(end_pos_worldspace - start_pos_worldspace);
	
	vec4 end_pos_lightview = lightView * end_pos_worldspace;
	vec4 start_pos_lightview = lightView * start_pos_worldspace;
	vec4 delta_lightview = normalize(end_pos_lightview - start_pos_lightview);
	
	float raymarch_distance_lightview = length(end_pos_lightview - start_pos_lightview);
	float step_size_lightview = raymarch_distance_lightview / NUM_SAMPLES;
	
	float raymarch_distance_worldspace = length(end_pos_worldspace - start_pos_worldspace);
	float step_size_worldspace = raymarch_distance_worldspace / NUM_SAMPLES;
	
	vec4 ray_position_lightview = start_pos_lightview + step_size_lightview * delta_lightview;
	vec4 ray_position_worldspace = start_pos_worldspace + step_size_worldspace * delta_worldspace;
	
	float light_contribution = 0.0;
	for (float l = raymarch_distance_worldspace; l > step_size_worldspace; l -= step_size_worldspace) {
		vec4 ray_position_lightspace = lightProjection * vec4(ray_position_lightview.xyz, 1);
		// perform perspective divide
		vec3 proj_coords = ray_position_lightspace.xyz / ray_position_lightspace.w;
		
		// transform to [0,1] range
		proj_coords = proj_coords * 0.5 + 0.5;
		
		// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
		vec4 closest_depth = texture(lightDepthMap, proj_coords.xy);
		
		float shadow_term = 1.0;
		
		if (proj_coords.z > closest_depth.r) {
			shadow_term = 0.0;
		}

		if(shadow_term > 0) light_contribution += 1.0f / NUM_SAMPLES;

		ray_position_lightview += step_size_lightview * delta_lightview;
		ray_position_worldspace += step_size_worldspace * delta_worldspace;
	}

	return min(max(light_contribution, 0.1), 1.0);
}

vec3 world_pos_from_depth(float depth)
{
	float z = depth * 2.0 - 1.0;

    vec4 clip_space_position = vec4(TexCoords * 2.0 - 1.0, z, 1.0);
    vec4 view_space_position = projectionInv * clip_space_position;

    // Perspective divide
    view_space_position /= view_space_position.w;

    vec4 world_space_position = viewInv * view_space_position;
    return world_space_position.xyz;
}