// 프레임 사이 경과 시간(델타 타임)과 FPS 를 계산한다. GLFW 와 무관해서 단위 테스트로 검사할 수 있다.
#pragma once

// 무엇을: 매 프레임 tick(현재 시각) 을 부르면 지난 프레임 이후 흐른 시간(초)을 계산한다.
//         움직임은 항상 "속도 × deltaSeconds()" 로 계산해야 FPS 와 상관없이 같은 속도가 된다.
// 확장:   슬로 모션이 필요하면 timeScale 같은 값을 추가해 delta 에 곱하면 된다.
//         현재 시각을 밖에서 받기 때문에 테스트에서는 가짜 시각을 넣어 동작을 확인한다.
class FrameTimer {
public:
    // 창을 드래그하는 동안 멈췄다가 돌아오면 delta 가 매우 커진다. 그 값을 잘라낸다.
    static constexpr float kMaxDeltaSeconds = 0.1f;
    // FPS 표시는 0.5초마다 갱신한다(숫자가 너무 빨리 바뀌지 않게).
    static constexpr double kFpsUpdateInterval = 0.5;

    // 프레임 시작에 한 번 호출한다. nowSeconds 는 계속 증가하는 시각(예: glfwGetTime()).
    void tick(double nowSeconds);

    float deltaSeconds() const { return delta_; }
    float fps() const { return fps_; }
    float frameMilliseconds() const { return frameMs_; }

private:
    bool started_ = false;
    double last_ = 0.0;
    float delta_ = 0.0f;

    int framesSinceFpsUpdate_ = 0;
    double fpsTimer_ = 0.0;
    float fps_ = 0.0f;
    float frameMs_ = 0.0f;
};
