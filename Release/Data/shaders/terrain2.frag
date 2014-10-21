uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

varying vec4 alphaDataPass;
varying float fogOfWarPass;

void main()
{	
    vec4 color;
    
        color = vec4(vec3(texture2D(texture0,gl_TexCoord[0].st)),alphaDataPass.x);
        color = mix(color,texture2D(texture1,gl_TexCoord[0].st),alphaDataPass.y);

    color.a = 1.0;

	gl_FragColor = color *  gl_Color * fogOfWarPass;
}






























/*uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

varying vec4 alphaDataPass;

void main()
{	
    vec4 color;
    
        color = vec4(vec3(texture2D(texture0,gl_TexCoord[0].st)),alphaDataPass.x);
        color = color * (1.0 - alphaDataPass.y) + texture2D(texture1,gl_TexCoord[0].st) * alphaDataPass.y;
        color = color * (1.0 - alphaDataPass.z) + texture2D(texture2,gl_TexCoord[0].st) * alphaDataPass.z;
        color = color * (1.0 - alphaDataPass.w) + texture2D(texture3,gl_TexCoord[0].st) * alphaDataPass.w;

    color.a = 1.0;

	gl_FragColor = color *  gl_Color;
}
*/