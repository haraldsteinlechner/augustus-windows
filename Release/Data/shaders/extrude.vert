attribute float extrude;

const float extrudeWidth = 0.02;

void main()
{
	float extrudeBool = extrude;
	extrudeBool=1.0;


	if(extrudeBool == 1.0)
	{
		gl_FrontColor = vec4(1.0,0.0,0.0,1.0);
		vec4 pos =  gl_ModelViewMatrix * gl_Vertex;
		vec4 temp = pos + (pos - gl_LightSource[0].position) * extrudeWidth;
		gl_Position =  gl_ProjectionMatrix * temp;
	}
	
	else
	{ 
		gl_FrontColor = vec4(1.0,1.0,1.0,1.0);
		gl_Position = gl_ModelViewProjectionMatrix  * gl_Vertex;
	}
}