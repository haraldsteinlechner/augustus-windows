uniform sampler2D localReflection;
uniform sampler2D normalMap;
uniform sampler2D skyRelction;
uniform sampler2D refraction;

uniform vec2 timer;

varying vec3 lightDir;
varying vec3 eyeDir;

void main()
{
	vec3 nH = texture2D(normalMap, (gl_TexCoord[1].st) * 40.0 + timer).xyz * 2.0 - 1.0;
	vec3 n = texture2D(normalMap, (gl_TexCoord[1].st) * 20.0 + timer).xyz * 2.0 - 1.0;
    vec3 l = normalize(lightDir);
    vec3 h = normalize(reflect(eyeDir, n));
    
    n = normalize(n);
    nH = normalize(nH);
    
    float nDotL = max(0.0, dot(n, l));
    float nDotH = max(0.0, dot(nH, h));
    float power=0.0;
	if(nDotH>0.0)
	{
		power = pow(nDotH, 5.0);
		//discard;
	}
    
    vec4 ambientCol = vec4(0.0,0.0,0.0,1.0);			//gl_FrontLightProduct[0].ambient;
    vec4 diffuseCol = vec4(0.5,0.5,0.5,1.0) * nDotL;	//gl_FrontLightProduct[0].diffuse * nDotL;
    vec4 specularCol = vec4(1.0,1.0,1.0,1.0) * power;
    vec4 color =ambientCol + diffuseCol + specularCol;
    
    color.a = 1.0;
    
    vec4 sky = texture2D(skyRelction,(gl_TexCoord[1].st + vec2(n.x,n.y))*100.0);
    sky.a=0.3;
	

	vec4 dissorted = gl_TexCoord[0] + vec4(n.x,n.y,0.0,0.0) * 4.0;
	vec4 dissortedHeavy = gl_TexCoord[0] + vec4(n.x,n.y,0.0,0.0) * 20.0;
	vec4 refractionCol = texture2DProj(refraction,dissortedHeavy);
	vec4 reflectionCol = texture2DProj(localReflection,dissorted);
	vec4 gub = mix(reflectionCol,refractionCol,0.3);
	gub = gub + color + sky*0.3;
	gub.a=power*0.5+0.4;
	//gub.a=0.6;
	
	
	//gub = vec4(1.0,1.0,1.0,1.0)*power;
	//gub.a=1.0;
	gl_FragColor = gub;//texture2DProj(localReflection,gl_TexCoord[0]);
}