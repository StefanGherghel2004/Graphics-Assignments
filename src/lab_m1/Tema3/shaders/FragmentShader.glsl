#version 330

// Input
in vec2 texcoord;
in vec3 FragPos;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform bool is_texture_2;
uniform float time;
uniform bool rotate;

uniform int isFireFly;
uniform vec3 fireFlyColor;    

uniform vec3 cameraPos;
uniform vec3 fogColor;
uniform float fogDistance;


// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.

     float distanceToCamera = length(FragPos - cameraPos);
     float fogFactor = 1.0 - exp(-pow(distanceToCamera / fogDistance, 2.0));
     fogFactor = clamp(fogFactor, 0.0, 1.0);

     if (isFireFly == 1)
    {
        out_color = vec4(fireFlyColor, 1.0);
        vec3 finalColor = mix(out_color.rgb, fogColor, fogFactor);
        out_color = vec4(finalColor, 1.0);
        return;   
    }

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

    vec3 finalColor = mix(out_color.rgb, fogColor, fogFactor);

    out_color = vec4(finalColor, 1.0);

}
