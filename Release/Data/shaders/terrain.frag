varying vec4 diffuse,ambientGlobal, ambient;
varying vec3 normal,lightDir,halfVector;
varying float dist;

uniform sampler2D tex;

void main()
{
    vec3 n,halfV,viewV,ldir;
    float NdotL,NdotHV;
    vec4 color = ambientGlobal;
    float att;
    
    n = normalize(normal);
    
    NdotL = max(dot(n,normalize(lightDir)),0.0);

        att = 1.0;
        color += att * (diffuse * NdotL + ambient);

    if (NdotL > 0.0) {
    
    
        
        halfV = normalize(halfVector);
        NdotHV = max(dot(n,halfV),0.0);
        color += att * gl_FrontMaterial.specular * gl_LightSource[0].specular * 
                        pow(NdotHV,gl_FrontMaterial.shininess);
    }

    gl_FragColor = texture2D(tex,gl_TexCoord[0].st)*color;
}