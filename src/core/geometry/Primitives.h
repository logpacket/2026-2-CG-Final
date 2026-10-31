// 기본 도형: 바닥 평면과 상자. 모델 파일 없이도 렌더러를 확인할 수 있게 한다.
#pragma once

#include "core/geometry/MeshData.h"

#include <glm/vec3.hpp>

// 무엇을: 코드로 바로 만드는 단순한 도형. 전시할 부품은 OBJ 파일(assets/models/)에서 불러온다.
// 확장:   새 도형은 같은 방식으로 함수를 추가한다. 면을 만들 때는 Primitives.cpp 의 appendQuad 를 쓰면
//         감김 방향(CCW)과 노멀이 자동으로 맞는다.

// y = 0 에 놓인 정사각형 바닥. 가운데가 원점, 한 변이 size, 노멀은 +Y, uv 는 (0, 0) ~ (1, 1).
MeshData makePlane(float size);

// 가운데가 원점이고 x/y/z 길이가 size 인 상자.
// 면마다 정점 4개를 따로 둬서 모서리가 각지게(면 노멀로) 보인다.
MeshData makeBox(const glm::vec3& size);
