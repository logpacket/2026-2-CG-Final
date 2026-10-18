# =============================================================================
# 모듈 목록. 모듈마다 폴더 하나와 CMakeLists.txt 하나가 있고, CMake 타깃 이름은 ve_<모듈>.
# 위 모듈은 아래 모듈만 쓸 수 있으므로 아래부터 차례로 추가한다 (검사: cmake/ProjectModules.cmake).
#
#   app          실행 파일: 모든 모듈을 조립한다                       코어
#    └ exhibition  전시관 내용: room/ exhibits/ ui/                    A, B
#       └ engine     창·입력, OpenGL 래퍼, 씬, 렌더러, 피킹·조립 규칙  코어 (그림자: A)
#          └ core      GL 없는 기반 코드와 도형 데이터(geometry/)       코어, B
#
# 모듈 안의 하위 폴더(gfx/, scene/, room/ …)는 정리와 담당 구분을 위한 것이고 따로 빌드하지 않는다.
# 새 파일은 알맞은 하위 폴더에 두면 자동으로 포함된다.
# =============================================================================
add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(exhibition)
add_subdirectory(app)
