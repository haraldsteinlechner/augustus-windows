attribute vec4 alphaData;

varying vec3 alphaDataPass;
varying vec3 fogOfWarPass;

varying vec4 diffuse;
varying vec3 normal,lightDir,halfVector;

varying vec4 currentPos;

uniform float waterHeight;

void main()
{
    vec4 ecPos;
    vec3 aux;
    
    normal = normalize(gl_NormalMatrix * gl_Normal);
    
    ecPos = gl_ModelViewMatrix * gl_Vertex;
    aux = vec3(-gl_LightSource[0].position-ecPos);
    lightDir = normalize(aux);

    halfVector = normalize(gl_LightSource[0].halfVector.xyz);
    
    diffuse = gl_FrontMaterial.diffuse;

	// Standardtexturkoordinaten
	gl_TexCoord[0] = gl_MultiTexCoord0;


	mat4 reflectMatrix = mat4(1.0,0.0,0.0,0.0,
							  0.0,-1.0,0.0,waterHeight*2.0,
							  0.0,0.0,1.0,0.0,
							  0.0,0.0,0.0,1.0);

	gl_Position=ftransform();

	
	currentPos = reflectMatrix * gl_Vertex;
	
	
	alphaDataPass = alphaData.xyz;
	fogOfWarPass = vec3(alphaData.w,0.0,0.0);
}
