#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 vertexUV;

uniform mat4 cameraToClipMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 modelToWorldMatrix;
uniform vec3 offset;
uniform float time;

out vec2 UV;

void main()
{
	float timeScale = 3.14159f * 2.0f / 8.0f;
	float currTime = mod(time, 8.0f);
	vec4 totalOffset = vec4(sin(currTime * timeScale) * offset.x, sin(currTime * timeScale) * offset.y, cos(currTime * timeScale) * offset.z, 0.0f);
	vec4 pomocnicza = position + totalOffset;

	vec4 temp = modelToWorldMatrix * pomocnicza;
	temp = worldToCameraMatrix * temp;
	gl_Position = cameraToClipMatrix * temp;
	UV = vertexUV;
	
}
