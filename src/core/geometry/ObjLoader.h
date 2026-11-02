// OBJ 파일을 읽어 MeshData 하나로 만든다. 부품 1개 = 파일 1개.
#pragma once

#include "core/geometry/MeshData.h"

#include <iosfwd>
#include <string>

// 읽는 것: v, vt, vn, f (v · v/vt · v//vn · v/vt/vn), 음수(상대) 인덱스, CRLF 줄 끝.
// 무시하는 것: o, g, s, usemtl, mtllib, 주석, 모르는 키워드. 파일 안의 여러 오브젝트는 하나로 합친다.
// 주의: 사각형 이상의 면은 부채꼴로 나누므로 볼록 다각형이어야 한다. 오목한 면은 내보낼 때 삼각형으로 나눈다.
//       형식이 틀리면 std::runtime_error("<파일>:<줄>: <이유>") 를 던진다.
MeshData loadObj(const std::string& path);

// 스트림에서 읽는다 (테스트용). sourceName 은 오류 메시지에만 쓴다.
MeshData parseObj(std::istream& in, const std::string& sourceName);
