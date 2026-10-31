// 프레임 사이 경과 시간(델타 타임)과 FPS 를 계산한다.
// 현재 시각을 밖에서 받으므로 테스트에서는 가짜 시각을 넣어 확인할 수 있다.
#pragma once

// 확장: 슬로 모션이 필요하면 timeScale 같은 값을 추가해 delta 에 곱한다.
// 주의: 움직임은 항상 "속도 × deltaSeconds()" 로 계산해야 FPS 와 상관없이 같은 속도가 된다.
class FrameTimer {
public:
    // 창을 드래그하는 동안 멈췄다 돌아오면 delta 가 매우 커지므로 이 값을 넘지 않게 한다.
    static constexpr float kMaxDeltaSeconds = 0.1f;
    // FPS 숫자가 너무 빨리 바뀌지 않도록 이 간격마다 갱신한다.
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
