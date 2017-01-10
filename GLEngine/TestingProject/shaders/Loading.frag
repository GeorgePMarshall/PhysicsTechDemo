#version 410

in vec2 vTexCoord;

uniform sampler2D diffuse;

void main()
{
	gl_FragColor = texture(diffuse, vTexCoord);
}