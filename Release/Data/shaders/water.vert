varying vec3 lightDir;
varying vec3 eyeDir;


void main()
{
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;

    vec3 t = normalize(gl_NormalMatrix * gl_MultiTexCoord1.xyz);
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
    vec3 b = cross(n,t);
    
    mat3 tbnMatrix = mat3(t, b, n);


    lightDir = vec3(100.0,150.0,000.0);
    lightDir *= tbnMatrix;

    eyeDir = vec3(gl_ModelViewMatrix * gl_Vertex);
    eyeDir *= tbnMatrix;
}