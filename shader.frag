// blend fragment shader

uniform sampler2D mTexture0;
uniform sampler2D mTexture1;
uniform sampler2D mTexture2;
uniform sampler2D mTexture3;
uniform sampler2D mTexture4;

void main(void)
{
	vec2 coord = vec2(gl_TexCoord[0]);
    vec4 tex0 = texture2D(mTexture0, coord);
    vec4 tex1 = texture2D(mTexture1, coord);
    vec4 tex2 = texture2D(mTexture2, coord);
    vec4 tex3 = texture2D(mTexture3, coord);
    vec4 alpha = texture2D(mTexture4, coord);
	
	
	vec4 channel0 = tex0*(1.0-(alpha.g+alpha.b+alpha.a));
	vec4 channel1 = tex1*alpha.g;
	vec4 channel2 = tex2*alpha.b;
	vec4 channel3 = tex3*alpha.a;

    gl_FragColor = channel0+channel1+channel2+channel3;
}