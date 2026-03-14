#version 330

// Input
// TODO(student): Get values from vertex shader
uniform float time;

in vec3 frag_color;
in vec3 normal; 

// Output
layout(location = 0) out vec4 out_color;



void main()
{
    // TODO(student): Write pixel out color

    float out_color_x = sin(2 * time + 1) * normal.x;
    float out_color_y = cos(3 * time) * normal.y;
    float out_color_z = 2 * sin(time + 2) * normal.z;

    out_color = abs((vec4(out_color_x, out_color_y, out_color_z, 1)));

    //out_color = vec4(abs(normal), 1);
}
