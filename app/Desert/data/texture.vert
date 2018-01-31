#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 vertexUV;

uniform mat4 cameraToClipMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 modelToWorldMatrix;

out vec2 UV;

void main()
{
	vec4 temp = modelToWorldMatrix * position;
	temp = worldToCameraMatrix * temp;
	gl_Position = cameraToClipMatrix * temp;
	UV = vertexUV;
}
