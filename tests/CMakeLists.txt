# tests — 단위 테스트 실행 파일 unit_tests: 창이나 GPU 없이 돌아가는 코드만 테스트한다 (ctest 로 실행)
# 담당: 모두 (자기 모듈의 테스트는 각자 작성). tests/ 에 *Test.cpp 를 추가하면 자동으로 포함된다.
file(GLOB VE_TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
add_executable(unit_tests ${VE_TEST_SOURCES})
target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 어느 모듈이든 테스트할 수 있도록 모든 모듈을 링크한다. 링크만 할 뿐 창은 열지 않는다.
get_property(VE_ALL_MODULES GLOBAL PROPERTY VE_MODULES)
target_link_libraries(unit_tests PRIVATE ${VE_ALL_MODULES})
ve_apply_defaults(unit_tests)

add_test(NAME unit_tests COMMAND unit_tests)
