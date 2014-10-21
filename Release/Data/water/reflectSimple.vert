varying vec4 currentPos;

uniform float waterHeight;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;


	mat4 reflectMatrix = mat4(1.0,0.0,0.0,0.0,
							  0.0,-1.0,0.0,waterHeight*2.0,
							  0.0,0.0,1.0,0.0,
							  0.0,0.0,0.0,1.0);

	gl_Position=ftransform();

	
	currentPos = reflectMatrix * gl_Vertex;
}
