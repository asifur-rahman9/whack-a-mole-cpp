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

uniform sampler2D[2] shadowMap;
uniform mat4[2] lightSpaceMatrix;

out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 norm, int lightNo);


void main()
{
    
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    vec3 norm = normalize(Normal);

    for(int i = 0; i < lightNo; i++){
        
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

    vec3 shadDir = normalize(lightPos[0] - FragPos);
    vec4 fragPosLightSpace = lightSpaceMatrix[0] * vec4(FragPos, 1.0);
    float shadow = ShadowCalculation(fragPosLightSpace, shadDir, norm, 0);

    vec3 shadDir2 = normalize(lightPos[1] - FragPos);
    vec4 fragPosLightSpace2 = lightSpaceMatrix[1] * vec4(FragPos, 1.0);
    float shadow2 = ShadowCalculation(fragPosLightSpace2, shadDir2, norm, 1);
    
    

    vec4 textureColor = texture(textureSampler, vertexUV);
    vec3 lighting = ambient + (1.0 - (shadow/2) - (shadow2/2)) * (diffuse + specular);
    vec4 result = vec4(lighting, 1.0);
    FragColor = result * textureColor;
    FragColor = result * textureColor;
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 norm, int lightNo)
{
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective
    float closestDepth = texture(shadowMap[lightNo], projCoords.xy).r;
    // Current fragment depth from light's perspective
    float currentDepth = projCoords.z;
    // Shadow if current depth > closest depth
    //float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005); 
    float shadow = currentDepth - 0.001 > closestDepth ? 1.0 : 0.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}