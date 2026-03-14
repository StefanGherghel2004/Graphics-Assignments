#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform bool is_texture_2;
uniform float time;
uniform bool rotate;


// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.

    vec2 coords = texcoord;

    if (rotate) {
        coords.x += time;
    }

    vec4 color = texture2D(texture_1, coords);
    if (color.a < 0.5f) {
        discard;
    }
    

    if (is_texture_2) {
        if (rotate) {
            coords.x += time;
        }

        vec4 color2 = texture2D(texture_2, coords);
        out_color = mix(color, color2, 0.5);
    } else {
        out_color = color;
    }

}
