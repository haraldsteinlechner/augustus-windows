varying vec3 lightDir;
varying vec3 eyeDir;

void main()
{
	vec4 ecPos = gl_ModelViewMatrix * gl_Vertex;
	
	gl_TexCoord[0] = gl_TextureMatrix[0] * ecPos;
	gl_TexCoord[1] = gl_MultiTexCoord0;
	
	vec3 t = vec3(1.0,0.0,0.0); //normalize(gl_NormalMatrix * gl_MultiTexCoord1.xyz);
	vec3 n = vec3(0.0,1.0,0.0); //normalize(gl_NormalMatrix * gl_Normal);
    vec3 b = vec3(0.0,0.0,1.0); //cross(n,t);
    
    mat3 tbnMatrix = mat3(t, b, n);
	
	lightDir = vec3(1.0,1.0,1.0);
    //lightDir *= tbnMatrix;

    eyeDir = vec3(gl_ModelViewMatrix * gl_Vertex);
    eyeDir *= tbnMatrix;
	
	
	gl_Position = ftransform();
}