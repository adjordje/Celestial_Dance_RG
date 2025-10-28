//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D textureDiffuse;
// dirctional light
uniform vec3 lightDir;
uniform vec3 viewPos;
uniform bool blinn;

void main() {
    vec3 baseColor = texture(textureDiffuse, fs_in.TexCoords).rgb;

    // diffuse
    vec3 normal = normalize(fs_in.Normal);
    vec3 dirLight = normalize(-lightDir);
    float diff = max(dot(dirLight, normal), 0.0);
    vec3 diffuse = diff * baseColor;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    float spec = 0.0;
    if (blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    vec3 specular = vec3(0.02) * spec;

    // ambient
    vec3 ambient = 0.2 * baseColor;

    vec3 dayColor = ambient + diffuse + specular;

    FragColor = vec4(dayColor, 1.0);
}