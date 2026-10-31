// FrameTimer 단위 테스트 (테스트 작성 예시로도 쓴다).
#include "core/FrameTimer.h"

#include "TestFramework.h"

namespace {

constexpr double kFrame60 = 1.0 / 60.0;  // 60 FPS 일 때 한 프레임 시간

}  // namespace

TEST_CASE(firstTickHasZeroDelta)
{
    FrameTimer timer;
    timer.tick(12.5);  // 시작 시각은 아무 값이어도 된다
    CHECK(timer.deltaSeconds() == 0.0f);
}

TEST_CASE(deltaIsTimeBetweenTicks)
{
    FrameTimer timer;
    timer.tick(1.0);
    timer.tick(1.0 + kFrame60);
    CHECK_NEAR(timer.deltaSeconds(), kFrame60, 1e-6);
}

TEST_CASE(longPauseIsClampedToMaxDelta)
{
    FrameTimer timer;
    timer.tick(0.0);
    timer.tick(3.0);  // 창을 3초 동안 드래그했다고 가정
    CHECK_NEAR(timer.deltaSeconds(), FrameTimer::kMaxDeltaSeconds, 1e-6);
}

TEST_CASE(fpsIsAveragedOverHalfSecond)
{
    FrameTimer timer;
    double now = 0.0;
    timer.tick(now);
    CHECK(timer.fps() == 0.0f);  // 0.5초가 지나기 전에는 아직 계산하지 않는다

    for (int frame = 0; frame < 36; ++frame) {  // 0.6초 = 갱신 간격(0.5초)보다 조금 길게
        now += kFrame60;
        timer.tick(now);
    }
    CHECK_NEAR(timer.fps(), 60.0, 0.5);
    CHECK_NEAR(timer.frameMilliseconds(), 1000.0 * kFrame60, 0.1);
}
