# engine — 화면에 그리고 조작하는 공통 기능: 창·입력, OpenGL 래퍼, 씬, 렌더러, 피킹·조립 규칙
#   하위 폴더: platform/ 창·입력·ImGui 초기화, gfx/ Shader·Mesh·Texture·Material, scene/ Transform·Entity·Camera,
#              render/ Renderer·PBR [코어]·ShadowPass [A], interaction/ 피킹·드래그·AssemblyController [코어]
ve_module(engine
  DEPENDS core
  LIBRARIES glad glfw imgui
)
