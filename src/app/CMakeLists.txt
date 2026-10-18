# app — App(메인 루프)과 main(): 모든 모듈을 조립해 실행 파일 VirtualExhibition 을 만든다 [코어]
file(GLOB_RECURSE VE_APP_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
add_executable(VirtualExhibition ${VE_APP_SOURCES})
target_link_libraries(VirtualExhibition PRIVATE ve_exhibition)  # exhibition 이 engine, core 를 함께 가져온다
ve_apply_defaults(VirtualExhibition)
