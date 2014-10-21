attribute float extrude;

const float extrudeWidth = 0.1;

void main()
{
	vec3 normal, lightDir;
	normal = normalize(gl_NormalMatrix * gl_Normal);


	lightDir = normalize(vec3(gl_LightSource[0].position));
	float NDotL = dot(normal,lightDir);

	if(extrude==1.0 && NDotL>0.0)
	{
		gl_FrontColor = vec4(1.0,0.0,0.0,1.0);
		vec4 pos =  gl_ModelViewMatrix * gl_Vertex;
		vec4 temp = pos + (pos - gl_LightSource[0].position) * extrudeWidth;
		vec4 temp2 =  vec4(vec3(temp),1.0);
		gl_Position = gl_ProjectionMatrix  * temp2;
	}
	else
	{ 
		gl_FrontColor = NDotL * vec4(0.0,1.0,0.0,1.0);
		gl_Position = gl_ModelViewProjectionMatrix  * gl_Vertex;
	}
}