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

uniform float time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 normal;

void main()
{
    // TODO(student): Send output to fragment shader
    float speed = 1;

    frag_color = v_color;
    normal = v_normal;
    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position + vec3(3 * sin(time * speed), 5 * cos(time * speed), 0), 1.0);

}
