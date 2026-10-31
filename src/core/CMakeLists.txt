# core — GL·GLFW 없이 돌아가는 기반 코드: 수학(AABB·Ray), FrameTimer, 입력 상태, 에셋 경로, 도형 데이터
#   하위 폴더: math/ Aabb, geometry/ MeshData·기본 도형·OBJ 로더 [코어]
ve_module(core
  LIBRARIES glm
)

# 에셋 폴더의 절대 경로. AssetPath.cpp 가 이 값을 기준으로 파일 경로를 만든다.
target_compile_definitions(ve_core PRIVATE VE_ASSET_DIR="${PROJECT_SOURCE_DIR}/assets")
