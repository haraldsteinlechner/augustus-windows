uniform sampler2D diffuse;
uniform sampler2D normal;

varying vec3 lightDir;
varying vec3 halfVector;

void main()
{
    vec3 n = texture2D(normal, gl_TexCoord[0].st).xyz * 2.0 - 1.0;
    vec3 l = normalize(lightDir);
    vec3 h = normalize(halfVector);

    float nDotL = max(0.0, dot(n, l));
    float nDotH = max(0.0, dot(n, h));
    float power=0.0;
	if(nDotH>0.0)
	{
		power = pow(nDotH, gl_FrontMaterial.shininess);
	}
    
    vec4 ambientCol = vec4(0.08,0.08,0.08,1.0);			//gl_FrontLightProduct[0].ambient;
    vec4 diffuseCol = vec4(1.0,1.0,1.0,1.0) * nDotL;	//gl_FrontLightProduct[0].diffuse * nDotL;
    vec4 specularCol = gl_FrontLightProduct[0].specular * power;
    vec4 color = gl_FrontLightModelProduct.sceneColor + ambientCol + diffuseCol + specularCol;
    
    gl_FragColor = color*texture2D(diffuse, gl_TexCoord[0].st);
}