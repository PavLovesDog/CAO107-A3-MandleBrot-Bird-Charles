#version 430 core

in vec3 outColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D Texture;
uniform sampler2D Texture2;

uniform float currentTime;

void main()
{
	//vec3 colorTemp = outColor * abs(sin(currentTime));
	//color = vec4(colorTemp, 1.0f) ;
	
	//color = texture(ourTexture, TexCoord) * vec4(outColor, 1.0f) * abs(sin(currentTime))  ;
	
	color = mix(texture(Texture, TexCoord), texture(Texture2, TexCoord), 0.2) * vec4(outColor, 1.0f) * abs(sin(currentTime));
}