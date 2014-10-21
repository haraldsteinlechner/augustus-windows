/*#define MacX1600Bugs;
#if defined(MacX1600Bugs)
attribute vec3 fogOfWar;
#define fogOfWar (fogOfWar.x)
#else
attribute float fogOfWar;
#endif*/

attribute float fogOfWar;
attribute vec4 alphaData;

varying vec3 alphaDataPass;
varying vec3 fogOfWarPass;

void main()
{
	vec3 normal, lightDir;
	vec4 diffuse, ambient;
	float NDotL;
	vec4 ecPos;
	
	
	ecPos = gl_ModelViewMatrix * gl_Vertex;
	normal = normalize(gl_NormalMatrix*gl_Normal);
	lightDir = normalize(vec3(gl_LightSource[0].position-ecPos));
	NDotL = max(dot(normal,lightDir),0.0);
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	
	ambient = vec4(0.18,0.22,0.15,1.0);
	
	gl_FrontColor = (NDotL) * diffuse + ambient;


	// Standardtexturkoordinaten
	gl_TexCoord[0] = gl_MultiTexCoord0;// * gl_TextureMatrix[0];

	// Worldspace
	gl_Position = ftransform();
	
	alphaDataPass = alphaData.xyz;
	fogOfWarPass = vec3(alphaData.w,fogOfWar,0.0);
}

	/*if(NDotL>0.0)
	{
		float NDotHV = max(dot(normal,gl_LightSource[0].halfVector.xyz),0.0);
		specular = vec4(1.0,1.0,1.0,1.0)* pow(NDotHV,gl_FrontMaterial.shininess);
	}*/