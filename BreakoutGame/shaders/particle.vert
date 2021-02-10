#version 330 core

layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 tex_coords;
out vec4 particle_color;

uniform mat4 projection;
uniform vec2 offset;
uniform vec4 color;

void main()
{
	float scale = 10.0f;
	tex_coords = vertex.zw;
	particle_color = color;
	gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0f, 1.0f);
}
