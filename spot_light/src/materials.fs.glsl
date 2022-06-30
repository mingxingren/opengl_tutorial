#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// 手电筒结构体
struct Light {
    vec3 position;  // 手电筒位置
    vec3 direction; // 手电筒方向
    float cutOff;   // 手电筒内圆锥余弦值
    float outerCutOff;  // 手电筒外圆锥余弦值

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; // 光衰减公式中的常量
    float linear;   // 光衰减公式中的线性值
    float quadratic;    // 光衰减公式中的二次项
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;     // 漫反射贴图

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main() {
    vec3 lightDir = normalize(light.position - FragPos);
    
    float theta = dot(lightDir,  normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon,  0.0, 1.0);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;  // 漫反射贴图可以替代为环境光

    vec3 norm = normalize(Normal);      // 法线单位向量化
    float diff = max(dot(norm, lightDir), 0.0); // 算出光线与法线的夹角
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; // 反射光为环境光

    // 计算反射光 和 高光
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);   // 算出反射光
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;    // 高光根据镜面纹理贴图获取

    // 计算光衰减
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    vec3 result  = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}
