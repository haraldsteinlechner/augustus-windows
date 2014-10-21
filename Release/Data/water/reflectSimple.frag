varying vec4 currentPos;

uniform float waterHeight;
uniform sampler2D texture0;

void main()
{	
	if(currentPos.y>-waterHeight) discard;


    gl_FragColor = texture2D(texture0,gl_TexCoord[0].st);
}