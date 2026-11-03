#include "core/geometry/ObjLoader.h"

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <system_error>
#include <tuple>
#include <utility>
#include <vector>

namespace {

constexpr std::size_t kMissing = std::numeric_limits<std::size_t>::max();  // 꼭짓점에 vt 나 vn 이 없음
constexpr float kMinNormalLength = 1e-12f;

// 면의 꼭짓점 하나. 값은 0부터 시작하는 배열 인덱스다.
struct Corner {
    std::size_t position = kMissing;
    std::size_t uv = kMissing;
    std::size_t normal = kMissing;
};

using CornerKey = std::tuple<std::size_t, std::size_t, std::size_t>;

// 넓이가 0 인 삼각형이나 잘못된 vn 처럼 방향을 정할 수 없으면 +Y 를 쓴다.
glm::vec3 unitOrUp(const glm::vec3& v)
{
    const float length = glm::length(v);
    if (length < kMinNormalLength) {
        return {0.0f, 1.0f, 0.0f};
    }
    return v / length;
}

class ObjParser {
public:
    explicit ObjParser(std::string sourceName) : sourceName_(std::move(sourceName)) {}

    void parseLine(const std::string& line, int lineNumber);
    MeshData takeMesh() { return std::move(mesh_); }

private:
    [[noreturn]] void fail(const std::string& reason) const;
    float readFloat(std::istringstream& in) const;
    glm::vec2 readVec2(std::istringstream& in) const;
    glm::vec3 readVec3(std::istringstream& in) const;
    std::size_t resolveIndex(const std::string& text, std::size_t count) const;
    Corner readCorner(const std::string& token) const;
    std::vector<Corner> readCorners(std::istringstream& in) const;
    void addFace(const std::vector<Corner>& corners);
    std::uint32_t vertexIndex(const Corner& corner, const glm::vec3& faceNormal);
    std::uint32_t addVertex(const Corner& corner, const glm::vec3& normal);

    std::string sourceName_;
    int lineNumber_ = 0;
    std::vector<glm::vec3> positions_;
    std::vector<glm::vec2> uvs_;
    std::vector<glm::vec3> normals_;
    std::map<CornerKey, std::uint32_t> vertexByCorner_;  // 노멀이 있는 (v, vt, vn) → 이미 만든 정점
    MeshData mesh_;
};

void ObjParser::parseLine(const std::string& line, int lineNumber)
{
    lineNumber_ = lineNumber;
    // # 뒤는 주석이다. CRLF 의 '\r' 은 >> 가 공백으로 보고 건너뛴다.
    std::istringstream in(line.substr(0, line.find('#')));
    std::string keyword;
    in >> keyword;
    if (keyword == "v") {
        positions_.push_back(readVec3(in));
    } else if (keyword == "vt") {
        uvs_.push_back(readVec2(in));
    } else if (keyword == "vn") {
        normals_.push_back(readVec3(in));
    } else if (keyword == "f") {
        addFace(readCorners(in));
    }
}

void ObjParser::fail(const std::string& reason) const
{
    throw std::runtime_error(sourceName_ + ":" + std::to_string(lineNumber_) + ": " + reason);
}

float ObjParser::readFloat(std::istringstream& in) const
{
    float value = 0.0f;
    if ((in >> value).fail()) {
        fail("expected a number");
    }
    return value;
}

// vt 의 세 번째 값(w)처럼 뒤에 남은 값은 쓰지 않는다.
glm::vec2 ObjParser::readVec2(std::istringstream& in) const
{
    const float x = readFloat(in);
    const float y = readFloat(in);
    return {x, y};
}

glm::vec3 ObjParser::readVec3(std::istringstream& in) const
{
    const float x = readFloat(in);
    const float y = readFloat(in);
    const float z = readFloat(in);
    return {x, y, z};
}

std::size_t ObjParser::resolveIndex(const std::string& text, std::size_t count) const
{
    long long index = 0;
    const char* const last = text.data() + text.size();
    const auto [end, error] = std::from_chars(text.data(), last, index);
    if (error != std::errc() || end != last) {
        fail("invalid index '" + text + "'");
    }
    // OBJ 인덱스는 1부터 센다. 음수는 지금까지 나온 것 중 뒤에서 몇 번째인지를 뜻한다.
    const long long size = static_cast<long long>(count);
    const long long resolved = index > 0 ? index - 1 : size + index;
    if (index == 0 || resolved < 0 || resolved >= size) {
        fail("index " + text + " is out of range");
    }
    return static_cast<std::size_t>(resolved);
}

// 꼭짓점 형식: v, v/vt, v//vn, v/vt/vn
Corner ObjParser::readCorner(const std::string& token) const
{
    Corner corner;
    const std::size_t firstSlash = token.find('/');
    corner.position = resolveIndex(token.substr(0, firstSlash), positions_.size());
    if (firstSlash == std::string::npos) {
        return corner;
    }
    const std::size_t secondSlash = token.find('/', firstSlash + 1);
    const std::string uvText = token.substr(firstSlash + 1, secondSlash - firstSlash - 1);
    if (!uvText.empty()) {
        corner.uv = resolveIndex(uvText, uvs_.size());
    }
    if (secondSlash != std::string::npos) {
        corner.normal = resolveIndex(token.substr(secondSlash + 1), normals_.size());
    }
    return corner;
}

std::vector<Corner> ObjParser::readCorners(std::istringstream& in) const
{
    std::vector<Corner> corners;
    std::string token;
    while (in >> token) {
        corners.push_back(readCorner(token));
    }
    return corners;
}

void ObjParser::addFace(const std::vector<Corner>& corners)
{
    if (corners.size() < 3) {
        fail("a face needs at least 3 vertices");
    }
    // 부채꼴 분할: 삼각형 (0, i, i + 1). 꼭짓점 순서를 그대로 두므로 앞면 방향(CCW)이 유지된다.
    for (std::size_t i = 1; i + 1 < corners.size(); ++i) {
        const std::array<Corner, 3> triangle{corners[0], corners[i], corners[i + 1]};
        const glm::vec3& a = positions_[triangle[0].position];
        const glm::vec3& b = positions_[triangle[1].position];
        const glm::vec3& c = positions_[triangle[2].position];
        const glm::vec3 faceNormal = unitOrUp(glm::cross(b - a, c - a));
        for (const Corner& corner : triangle) {
            mesh_.indices.push_back(vertexIndex(corner, faceNormal));
        }
    }
}

std::uint32_t ObjParser::vertexIndex(const Corner& corner, const glm::vec3& faceNormal)
{
    // 노멀이 없는 꼭짓점은 같은 위치라도 면마다 노멀이 달라서 합치지 않는다.
    if (corner.normal == kMissing) {
        return addVertex(corner, faceNormal);
    }
    const CornerKey key{corner.position, corner.uv, corner.normal};
    const auto found = vertexByCorner_.find(key);
    if (found != vertexByCorner_.end()) {
        return found->second;
    }
    const std::uint32_t index = addVertex(corner, unitOrUp(normals_[corner.normal]));
    vertexByCorner_.emplace(key, index);
    return index;
}

std::uint32_t ObjParser::addVertex(const Corner& corner, const glm::vec3& normal)
{
    Vertex vertex;
    vertex.position = positions_[corner.position];
    vertex.normal = normal;
    if (corner.uv != kMissing) {
        vertex.uv = uvs_[corner.uv];
    }
    mesh_.vertices.push_back(vertex);
    return static_cast<std::uint32_t>(mesh_.vertices.size() - 1);
}

}  // namespace

MeshData loadObj(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error(path + ": cannot open file");
    }
    return parseObj(file, path);
}

MeshData parseObj(std::istream& in, const std::string& sourceName)
{
    ObjParser parser(sourceName);
    std::string line;
    int lineNumber = 0;
    while (std::getline(in, line)) {
        ++lineNumber;
        parser.parseLine(line, lineNumber);
    }
    return parser.takeMesh();
}
