#version 330

in vec3 frag_color;
in vec3 normal;
in vec3 frag_pos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float alertFactor;

layout(location = 0) out vec4 out_color;

void main()
{
    vec3 baseColor = frag_color;

    // alert for central station
    vec3 alertColor = vec3(1.0, 0.0, 0.0);
    vec3 objectColor = mix(baseColor, alertColor, alertFactor);

    vec3 N = normalize(normal);
    vec3 L = normalize(lightPos - frag_pos);
    vec3 V = normalize(viewPos - frag_pos);

    float distance = length(lightPos - frag_pos);
    // ATENUARE
    float constant = 0.5;
    float linear = 0.09;
    float quadratic = 0.032;
    float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);

    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * objectColor;

    // Diffuse (Lambert)
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = 20 * diff * objectColor;

    // Specular (Phong)
    float specularStrength = 0.5;
    vec3 R = reflect(-L, N);
    float shininess = 50.0;
    float spec = pow(max(dot(V, R), 0.0), shininess);
    vec3 specular = specularStrength * spec * vec3(1.0);

    vec3 finalColor = ambient +  (diffuse + specular) * attenuation;

    out_color = vec4(finalColor, 1.0);
}