#version 410 core

// Blinn-Phong: 방향광 하나 + 앰비언트. 색은 모두 선형 값으로 받아 계산하고 마지막에 감마 보정한다.

in vec3 vPositionWorld;
in vec3 vNormalWorld;

uniform vec3 uCameraPositionWorld;
uniform vec3 uLightDirectionWorld;  // 빛이 나아가는 방향
uniform vec3 uLightColor;           // 선형, 세기를 곱한 값
uniform vec3 uAmbientColor;         // 선형
uniform vec3 uBaseColor;            // 선형
uniform float uMetallic;
uniform float uRoughness;

out vec4 outColor;

const float kGamma = 2.2;
const float kMinRoughness = 0.05;
const vec3 kDielectricSpecular = vec3(0.04);  // 비금속 표면이 정면에서 반사하는 비율

// roughness(0 ~ 1)를 Blinn-Phong 지수로 바꾼다. 거칠수록 지수가 작아져 하이라이트가 넓고 흐려진다.
float shininessFromRoughness(float roughness)
{
    float r = max(roughness, kMinRoughness);
    return max(2.0 / (r * r * r * r) - 2.0, 1.0);
}

void main()
{
    vec3 normal = normalize(vNormalWorld);  // 보간된 노멀은 길이가 1 이 아니다
    vec3 toLight = normalize(-uLightDirectionWorld);
    vec3 toCamera = normalize(uCameraPositionWorld - vPositionWorld);
    vec3 halfway = normalize(toLight + toCamera);

    float diffuse = max(dot(normal, toLight), 0.0);
    float specular = 0.0;
    if (diffuse > 0.0) {
        specular = pow(max(dot(normal, halfway), 0.0), shininessFromRoughness(uRoughness));
    }

    vec3 diffuseColor = uBaseColor * (1.0 - uMetallic);
    vec3 specularColor = mix(kDielectricSpecular, uBaseColor, uMetallic);
    vec3 color = uAmbientColor * uBaseColor + uLightColor * (diffuseColor * diffuse + specularColor * specular);

    // 감마 보정은 여기 한 곳에서만 한다 (CODING_CONVENTION 9.8).
    outColor = vec4(pow(color, vec3(1.0 / kGamma)), 1.0);
}
