#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

smooth out vec4 interpColor;

uniform mat4 cameraToClipMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 modelToWorldMatrix;

uniform float time;

void main()
{
	float timeScale = 3.14159f * 2.0f / 5.0f;
	float currTime = mod(time, 5.0f);
	vec4 totalOffset = vec4(cos(currTime * timeScale) * 2.0f, sin(currTime * timeScale) * 0.5f, cos(currTime * timeScale) * 3.0f, 0.0f);
	vec4 pomocnicza = position + totalOffset;
	vec4 temp = modelToWorldMatrix * pomocnicza;
	temp = worldToCameraMatrix * temp;
	gl_Position = cameraToClipMatrix * temp;
	interpColor = color;
}
