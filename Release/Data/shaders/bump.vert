varying vec3 lightDir;
varying vec3 halfVector;

void main()
{
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;

    vec3 t = normalize(gl_NormalMatrix * gl_MultiTexCoord1.xyz);
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
    vec3 b = cross(n,t);
    
    mat3 tbnMatrix = mat3(t, b, n);


    lightDir = gl_LightSource[0].position.xyz-gl_Vertex.xyz;
    lightDir *= tbnMatrix;

    halfVector = gl_LightSource[0].halfVector.xyz;
    halfVector *= tbnMatrix;
}