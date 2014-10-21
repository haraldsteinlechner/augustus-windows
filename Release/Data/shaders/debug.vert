const float alpha=0.3;

attribute float indexData;

void main()
{
    gl_Position=ftransform();
	
	if(indexData==0.0) // Land
		gl_FrontColor = vec4(0.0,1.0,0.0,alpha);
		
	if(indexData==1.0) // Wasser
		gl_FrontColor = vec4(0.0,0.0,1.0,alpha);
		
	if(indexData==2.0) // Luft
		gl_FrontColor = vec4(0.0,0.5,0.5,alpha);
		
	if(indexData==3.0) // Niemand
		gl_FrontColor = vec4(1.0,0.0,0.0,alpha);

}