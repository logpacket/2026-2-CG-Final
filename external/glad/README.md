# GLAD (OpenGL 4.1 Core loader)

이 폴더의 파일은 손으로 고치지 않는다. 다시 만들어야 하면 아래 명령으로 생성한다.

- 생성기: glad 0.1.36 (`pip install glad==0.1.36`)
- 설정: 언어 C/C++, API `gl=4.1`, 프로파일 `core`, 확장 없음, 로더 포함
- 명령 (프로젝트 루트에서):

```bash
python -m glad --profile=core --api=gl=4.1 --generator=c --spec=gl --extensions= --reproducible --out-path=external/glad
```

`--reproducible`은 인터넷에서 최신 사양을 받지 않고 glad 패키지에 들어 있는 `gl.xml`을 쓴다.
그래서 누가 다시 생성해도 같은 파일이 나온다.

라이선스: glad 생성기는 MIT, `KHR/khrplatform.h`는 Khronos 라이선스(MIT 계열)이다.
