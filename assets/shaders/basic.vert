#version 410 core

// 정점을 월드 좌표와 화면 좌표로 옮긴다. 조명은 basic.frag 에서 월드 좌표로 계산한다.

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uViewProjection;  // projection * view, CPU 에서 한 번 곱한다
uniform mat3 uNormalMatrix;    // transpose(inverse(mat3(uModel)))

out vec3 vPositionWorld;
out vec3 vNormalWorld;

void main()
{
    vec4 positionWorld = uModel * vec4(aPosition, 1.0);
    vPositionWorld = positionWorld.xyz;
    vNormalWorld = uNormalMatrix * aNormal;
    gl_Position = uViewProjection * positionWorld;
}
