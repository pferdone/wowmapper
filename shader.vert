// vertex shader
uniform mat4 mWorldViewProj;

void main()
{
	gl_Position = mWorldViewProj * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}