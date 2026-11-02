#version 410 core

// 조명 없이 한 가지 색으로 그린다. GL 래퍼와 메시 업로드를 확인하는 용도다.

layout(location = 0) in vec3 aPosition;

uniform mat4 uModelViewProjection;

void main()
{
    gl_Position = uModelViewProjection * vec4(aPosition, 1.0);
}
