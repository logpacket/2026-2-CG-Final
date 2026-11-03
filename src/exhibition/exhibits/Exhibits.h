// 전시할 부품 목록과, 목록을 읽어 그릴 수 있는 상태로 만드는 함수.
#pragma once

#include "engine/gfx/Material.h"
#include "engine/gfx/Mesh.h"
#include "engine/scene/Transform.h"

#include <glm/vec2.hpp>

#include <string>
#include <vector>

// 부품 하나를 어디에 어떻게 놓을지 적은 데이터
struct ExhibitSpec {
    std::string objPath;  // assets/ 기준 경로. 예) "models/gear.obj"
    Material material;
    glm::vec2 floorPositionXZ{0.0f};  // 바닥 위에 놓을 자리 (x, z), m. 부품의 가로 중심이 여기에 온다
    float yawRadians = 0.0f;          // y축 회전
};

// 불러와서 GPU 에 올린 부품
struct Exhibit {
    Mesh mesh;
    Material material;
    Transform transform;
};

// 확장: 부품을 더하려면 Exhibits.cpp 의 목록에 한 줄을 추가한다.
std::vector<ExhibitSpec> exhibitSpecs();

// OBJ 를 읽어 GPU 에 올리고 바닥에 놓는다. 읽지 못한 부품은 로그를 남기고 1 m 상자로 대신한다.
// 주의: Mesh 를 만들므로 GL 컨텍스트가 있어야 한다.
std::vector<Exhibit> loadExhibits(const std::vector<ExhibitSpec>& specs);
