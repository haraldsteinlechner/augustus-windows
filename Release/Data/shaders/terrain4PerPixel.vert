attribute float fogOfWar;
attribute vec4 alphaData;

varying vec3 alphaDataPass;
varying vec3 fogOfWarPass;

varying vec4 diffuse;
varying vec3 normal,lightDir,halfVector;

void main()
{
    vec4 ecPos;
    vec3 aux;
    
    normal = normalize(gl_NormalMatrix * gl_Normal);
    
    ecPos = gl_ModelViewMatrix * gl_Vertex;
    aux = vec3(gl_LightSource[0].position-ecPos);
    lightDir = normalize(aux);

    halfVector = normalize(gl_LightSource[0].halfVector.xyz);
    
    diffuse = gl_FrontMaterial.diffuse;

	// Standardtexturkoordinaten
	gl_TexCoord[0] = gl_MultiTexCoord0;

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
	
	
	
	
	
/*#define MacX1600Bugs;
#if defined(MacX1600Bugs)
attribute vec3 fogOfWar;
#define fogOfWar (fogOfWar.x)
#else
attribute float fogOfWar;
#endif*/
