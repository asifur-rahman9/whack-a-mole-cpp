#version 330 core
in vec3 vertexColor;
in vec2 vertexUV;
in vec3 Normal;
in vec3 FragPos;

uniform vec3[10] lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform sampler2D textureSampler;
uniform float specularStrength = 0.5;
uniform float ambientStrength = 0.1;
uniform int lightNo = 5;

out vec4 FragColor;
void main()
{
    
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    for(int i = 0; i < lightNo; i++){
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos[i] - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        float distanceX = (FragPos[0] - lightPos[i][0]);
        float distanceY = (FragPos[1] - lightPos[i][1]);
        float distanceZ = (FragPos[2] - lightPos[i][2]);
        float eucDist = (distanceX*distanceX) + (distanceY * distanceY) + (distanceZ * distanceZ);
        float attenuation = 4000 / eucDist;
        diffuse += diff * lightColor * attenuation;

        // specular
        //float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        specular += specularStrength * spec * lightColor * attenuation;  
    }

    diffuse = diffuse / 8.f;
    specular = specular / 10.f;
    

    vec4 textureColor = texture(textureSampler, vertexUV);
    vec4 result = vec4((ambient + diffuse + specular), 1.0 );
    FragColor = result * textureColor;
}