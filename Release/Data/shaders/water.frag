uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D skyTexture;

uniform vec2 timer;

varying vec3 lightDir;
varying vec3 eyeDir;

void main()
{
	// noise1.tga loop = 20; noise2.tga loop = 40;

    vec3 n = texture2D(normal, (gl_TexCoord[0].st) * 8.0 + timer*5.0).xyz * 2.0 - 1.0;
    vec3 l = normalize(lightDir);
    vec3 h = normalize(reflect(eyeDir, n));

	n = normalize(n);
	
    float nDotL = max(0.0, dot(n, l));
    float nDotH = max(0.0, dot(n, h));
    float power=0.0;
	if(nDotH>0.0)
	{
		power = pow(nDotH, 2.0);
		//discard;
	}
    
    vec4 ambientCol = vec4(0.08,0.08,0.08,1.0);			//gl_FrontLightProduct[0].ambient;
    vec4 diffuseCol = vec4(0.9,0.9,0.9,1.0) * nDotL;	//gl_FrontLightProduct[0].diffuse * nDotL;
    vec4 specularCol = vec4(1.0,1.0,1.0,1.0) * power;
    vec4 color = gl_FrontLightModelProduct.sceneColor + diffuseCol + specularCol;
    
    color.a = 1.0;
	
	// 0.8
	vec4 skyLookUp = texture2D(skyTexture,(gl_TexCoord[0].st*3.0)+n.xy*0.3) * 0.86;
	skyLookUp.a = texture2D(diffuse, gl_TexCoord[0].st).a;
	
	gl_FragColor = skyLookUp * color;
}