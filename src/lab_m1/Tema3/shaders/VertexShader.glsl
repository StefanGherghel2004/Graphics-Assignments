#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float Time;

uniform float TreeHeight;
uniform int isTrunk;
uniform int isLeaves;

uniform float BendFrequency;
uniform float BendPhase;
uniform float BendStrength;
uniform int   Level;

uniform float repeatFactor;

out vec2 texcoord;
out vec3 FragPos;

void main()
{
    FragPos = vec3(Model * vec4(v_position, 1.0));
    vec3 pos = v_position;

    if (isTrunk == 1)
    {
        pos.y *= TreeHeight;
        float t = clamp(pos.y / (TreeHeight * 0.75), 0.0, 1.0);
        float scale = 1.0 - t;
        pos.x *= scale;
        pos.z *= scale;
    }

    

    if (isLeaves == 1)
    {
        float t = clamp(pos.y / TreeHeight, 0.0, 1.0);
        float wave = sin(Time * BendFrequency + t * 0.5 + BendPhase);


        float offset = wave * BendStrength * (Level + 1) * 0.3 * t;


        pos.x += offset;
    }

    texcoord = v_texture_coord * repeatFactor;
    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}
