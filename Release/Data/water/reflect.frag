// Texturing

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

// Fog Of WAR
varying vec3 alphaDataPass;  // Gewichtungen T1,T2,T3
varying vec3 fogOfWarPass;	 // Gewichtung T4, FogOfWar, zero

// LICHT ---------------
varying vec4 diffuse;
varying vec3 normal,lightDir,halfVector;
const float ambient = 0.2;

varying vec4 currentPos;

uniform float waterHeight;

void main()
{	
	if(currentPos.y>-waterHeight) discard;


	// TEXTURING -----------
	
	vec3 fow = fogOfWarPass;
	vec3 alphaDataInternal = alphaDataPass;
	

		vec4 color;
    
			color = vec4(texture2D(texture0,gl_TexCoord[0].st).rgb,alphaDataInternal.x);
			color = mix(color,texture2D(texture1,gl_TexCoord[0].st),alphaDataInternal.y);
			color = mix(color,texture2D(texture2,gl_TexCoord[0].st),alphaDataInternal.z);
			color = mix(color,texture2D(texture3,gl_TexCoord[0].st),fow.x);

		color.a = 1.0;

    // LICHT ---------------
    vec3 n, halfV,viewV,ldir;
    float NdotL,NdotHV;
    vec4 lcolor = vec4(ambient,ambient,ambient,1.0);
    n = normalize(normal);
    
    NdotL = max(dot(n,normalize(lightDir)),0.0);

    lcolor += diffuse * NdotL + ambient;
        
    halfV = normalize(halfVector);
    NdotHV = max(dot(n,halfV),0.0);
	if(NdotHV>0.0)
	{
		lcolor += (gl_FrontMaterial.specular * 
                        pow(NdotHV,gl_FrontMaterial.shininess));
	}
    
    gl_FragColor = color *  lcolor;
}