#version 330

// Input
// TODO(student): Get values from vertex shader

in vec3 frag_color;
in vec3 normal; 

uniform float alertFactor; 

// Output
layout(location = 0) out vec4 out_color;


void main()
{
     vec3 baseColor = frag_color;

    vec3 alertColor = vec3(1.0, 0.0, 0.0);
    vec3 finalColor = mix(baseColor, alertColor, alertFactor);

    out_color = vec4(finalColor, 1.0);
}
