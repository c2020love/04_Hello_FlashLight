#version 330 core

struct Material {
	sampler2D specular;
	sampler2D diffuse;
	sampler2D raytexture;
};

struct Light {
	vec3 position;	//聚光的位置向量（光源的位置）
	vec3 direction;	//聚光的方向向量（光源的照射方向）
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutoff;	//切光角
	float constant;
	float linear;
	float quadratic;

	float outcutoff;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

uniform float matrixlight;
uniform float matrixmove;

void main() {
	//环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = vec3(1.0f);
	lightDir = normalize(light.position - FragPos);		//光的方向向量（当w为0的时候），而当w为1的时候就是光的位置向量
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

	//镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

	//点光源系数
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	diffuse *= attenuation;
	specular *= attenuation;
	ambient *= attenuation;

	//放射光
	vec3 emission = vec3(texture(material.raytexture, vec2(TexCoords)));

	//计算theta值，并将其与切光角cutoff对比，来判断这个片段是否在聚光的内部

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff - light.outcutoff;		//内外角之间的距离
	float intensity = clamp((theta - light.outcutoff) / epsilon, 0.0, 1.0);	//平滑化clamp,使值处于0到1之间

	diffuse *= intensity;
	specular *= intensity;
	vec3 result;
	result = (ambient + diffuse + specular);

	if(theta > light.cutoff) {
		result += emission*intensity*attenuation;
	}

	//最终结果

	FragColor = vec4(result, 1.0);
}
