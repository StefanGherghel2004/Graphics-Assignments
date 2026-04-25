#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 normal;
out vec3 frag_pos;

void main()
{

    frag_color = v_color;

    frag_pos = vec3(Model * vec4(v_position, 1.0));

    normal = normalize(mat3(transpose(inverse(Model))) * v_normal);

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);

}
