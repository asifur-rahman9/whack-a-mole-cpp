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
uniform float alpha = 1.0;

uniform sampler2D[5] shadowMap;
uniform mat4[5] lightSpaceMatrix;

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

    vec3[5] shadDir;
    float[5] shadow;
    float shadowAvg = 0;


    for(int i = 0; i < 5; i++){
        shadDir[i] = normalize(lightPos[i] - FragPos);
        vec4 fragPosLightSpace = lightSpaceMatrix[i] * vec4(FragPos, 1.0);
        shadow[i] = ShadowCalculation(fragPosLightSpace, shadDir[i], norm, i);
        shadowAvg += shadow[i];

    }
    
    shadowAvg /= 3;
    

    vec4 textureColor = texture(textureSampler, vertexUV);
    
    vec3 lighting = ambient + (1.0 - shadowAvg) * (diffuse + specular);
    vec4 result = vec4(lighting, alpha);
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