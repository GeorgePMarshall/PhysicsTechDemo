#version 410

in vec4 vNormal;
out vec4 fragColour;

uniform sampler2D sampler;
uniform float time;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


void main()
{
	fragColour = vec4(rand(vec2(cos(gl_FragCoord.x), sin(gl_FragCoord.y))), rand(vec2(sin(gl_FragCoord.x), sin(gl_FragCoord.y))), rand(vec2(cos(gl_FragCoord.x), cos(gl_FragCoord.y))), 1);
	float d = max(0, dot(vNormal.xyz, vec3(sin(time), sin(time), cos(time))));
	
	//fragColour = vec4(d,d,d,1) * vec4(rand(vec2(cos(gl_FragCoord.x), sin(time))), rand(vec2(sin(gl_FragCoord.x), sin(time))), rand(vec2(cos(gl_FragCoord.x), cos(time))), 1);

}

