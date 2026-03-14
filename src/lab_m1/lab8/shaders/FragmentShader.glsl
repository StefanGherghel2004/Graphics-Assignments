#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 light_position1;

uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform float spotLight;
uniform float cutOff;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize(eye_position   - world_position);
    vec3 R = reflect(-L, N);

    float d = distance(light_position, world_position);

    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0.2;
    vec3 ambient = ambient_light * object_color;

    float diffuse_light = material_kd * max (dot(N,L), 0);
    vec3 diffuse =  diffuse_light * object_color;

    // TODO(student): Compute specular light component
    int primesteLumina = 0;
    if (dot(N, L) > 0)
        primesteLumina = 1;

    float specular_light = material_ks * primesteLumina * pow(max(dot(V, R), 0), material_shininess);
     vec3 specular = specular_light * vec3(1.0);

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.


    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

    float attenuation = 1.0 / (0.5 * d * d + d + 0.1);
    vec3 color = ambient + attenuation * (diffuse + specular);

    if (spotLight == 1) {
        float cut_off = radians(cutOff);

        vec3 normDir = normalize(light_direction);

        float spot_light = dot(-L, normDir);
        float spot_light_limit = cos(cut_off);

        float linear_att =  clamp((spot_light - spot_light_limit) / (1.0f - spot_light_limit), 0.0f, 1.0f);
        float light_att_factor = pow(linear_att, 1.2);

        diffuse = diffuse * vec3(0, 0, 1);

        color = ambient + light_att_factor * 1.2 * (diffuse + specular);

    }

    vec3 L2 = normalize(light_position1 - world_position);
    float d2 = distance(light_position1, world_position);
    vec3 R2 = reflect(-L2, N);

    float diffuse2 = material_kd * max(dot(N, L2), 0);
    float specular2 = material_ks *
                      (dot(N, L2) > 0 ? 1 : 0) *
                      pow(max(dot(V, R2), 0), material_shininess);

    float attenuation2 = 1.0 / (0.5 * d2 * d2 + d2 + 0.1);

    vec3 diffuse_color2  = diffuse2  * object_color * vec3(1, 0, 0);
    vec3 specular_color2 = specular2 * vec3(1);

    color += attenuation2 * (diffuse_color2 + specular_color2);

    // TODO(student): Write pixel out color
    out_color = vec4(color, 1);

}
