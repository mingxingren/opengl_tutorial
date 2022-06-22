#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;     // 漫反射贴图

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main() {
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;  // 漫反射贴图可以替代为环境光

    vec3 norm = normalize(Normal);      // 法线单位向量化
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); // 算出光线与法线的夹角
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; // 反射光为环境光

    // 计算反射光 和 高光
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);   // 算出反射光
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;    // 高光根据镜面纹理贴图获取

    vec3 result  = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
