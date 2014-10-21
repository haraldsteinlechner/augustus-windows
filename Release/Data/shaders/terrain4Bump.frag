// Texturing

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

// Fog Of WAR
varying vec3 alphaDataPass;  // Gewichtungen T1,T2,T3
varying vec3 fogOfWarPass;	 // Gewichtung T4, FogOfWar, zero

// LICHT  BUMP ---------------
varying vec3 lightDir;
varying vec3 halfVector;

void main()
{	
	// TEXTURING -----------
	
	vec3 fow = fogOfWarPass;
	vec3 alphaDataInternal = alphaDataPass;
	

		vec4 color;
    
			color = vec4(texture2D(texture0,gl_TexCoord[0].st).rgb,alphaDataInternal.x);
			color = mix(color,texture2D(texture1,gl_TexCoord[0].st),alphaDataInternal.y);
			color = mix(color,texture2D(texture2,gl_TexCoord[0].st),fow.x);
			//color = mix(color,texture2D(texture3,gl_TexCoord[0].st),fow.x);
			color = color * fow.y;

		color.a = 1.0;

    // LICHT ---------------
    vec3 n = vec3(0.0,1.0,0.0);//texture2D(texture3, gl_TexCoord[0].st).xyz * 2.0 - 1.0;
    vec3 l = normalize(lightDir);
    vec3 h = normalize(halfVector);

    float nDotL = max(0.0, dot(n, l));
    float nDotH = max(0.0, dot(n, h));
    float power = (nDotL == 0.0) ? 0.0 : pow(nDotH, gl_FrontMaterial.shininess);
    
    vec4 ambientCol = vec4(0.08,0.08,0.08,1.0);			//gl_FrontLightProduct[0].ambient;
    vec4 diffuseCol = vec4(1.0,1.0,1.0,1.0) * nDotL;	//gl_FrontLightProduct[0].diffuse * nDotL;
    vec4 specularCol = gl_FrontLightProduct[0].specular * power;
    vec4 lcolor = gl_FrontLightModelProduct.sceneColor + ambientCol + diffuseCol + specularCol;
    
    gl_FragColor = color;
}






























/*uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

varying vec4 alphaDataPass;

void main()
{	
    vec4 color;
    
        color = vec4(vec3(texture2D(texture0,gl_TexCoord[0].st)),alphaDataPass.x);
        color = color * (1.0 - alphaDataPass.y) + texture2D(texture1,gl_TexCoord[0].st) * alphaDataPass.y;
        color = color * (1.0 - alphaDataPass.z) + texture2D(texture2,gl_TexCoord[0].st) * alphaDataPass.z;
        color = color * (1.0 - alphaDataPass.w) + texture2D(texture3,gl_TexCoord[0].st) * alphaDataPass.w;

    color.a = 1.0;

	gl_FragColor = color *  gl_Color;
}
*/