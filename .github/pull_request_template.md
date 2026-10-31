Closes #

## Changes

<!-- 바뀐 것을 항목마다 한 줄로
- engine/gfx: Shader 컴파일 에러를 로그로 출력
- exhibition/ui: 기어비 슬라이더 추가
-->

-

## Screenshots

<!-- 화면이 바뀌면 전/후 캡처를 붙인다. 움직임(애니메이션·드래그·조립)은 GIF 나 짧은 영상으로. -->

## Checklist

- [ ] 새 컴파일 경고 없이 빌드된다
- [ ] `ctest --preset <preset>` 통과
- [ ] `cmake --build --preset <preset> --target format-check` 통과
- [ ] clang-tidy 경고를 새로 만들지 않았다 (`-DVE_CLANG_TIDY=ON`)
- [ ] 허용된 라이브러리(GLFW, GLAD, GLM, Dear ImGui) 외에는 쓰지 않았다
- [ ] OpenGL 4.1 Core / GLSL 410 에 없는 기능을 쓰지 않았다 (macOS 에서 돌아야 한다)
- [ ] 만든 GL 리소스(버퍼 · 텍스처 · 셰이더 · FBO)는 해제된다
- [ ] `core` 로직을 바꿨다면 `tests/<이름>Test.cpp` 에 테스트를 추가했다
- [ ] 빌드 방법 · 모듈 구조 · 조작 키가 바뀌었다면 README 에 반영했다

## Notes

<!-- (선택) 확인한 방법, 테스트한 환경(프리셋), 리뷰어가 봐 줬으면 하는 곳, 남은 일 -->
