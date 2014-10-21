attribute float fogOfWar;
attribute vec4 alphaData;
attribute vec3 tangent;

varying vec3 alphaDataPass;
varying vec3 fogOfWarPass;

varying vec3 lightDir;
varying vec3 halfVector;

void main()
{
	// Bump light
    vec3 t = normalize(gl_NormalMatrix * tangent);
    vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	vec3 b = cross(n,t);
    
    
	mat3 tbnMatrix = mat3(t, b, n);
	/*mat3 tbnMatrix = mat3(1.0,0.0,0.0,
						  0.0,1.0,0.0,
						  0.0,0.0,1.0);*/


    lightDir = gl_LightSource[0].position.xyz;
    lightDir *= tbnMatrix;

    halfVector = gl_LightSource[0].halfVector.xyz;
    halfVector *= tbnMatrix;

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
