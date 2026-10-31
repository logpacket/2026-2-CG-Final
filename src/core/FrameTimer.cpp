// FrameTimer 구현.
#include "core/FrameTimer.h"

#include <algorithm>

void FrameTimer::tick(double nowSeconds)
{
    if (!started_) {
        started_ = true;  // 첫 프레임은 비교할 이전 시각이 없으므로 delta = 0, FPS 계산에도 넣지 않는다
        last_ = nowSeconds;
        delta_ = 0.0f;
        return;
    }
    const double rawDelta = nowSeconds - last_;
    last_ = nowSeconds;
    delta_ = std::min(static_cast<float>(rawDelta), kMaxDeltaSeconds);

    ++framesSinceFpsUpdate_;
    fpsTimer_ += rawDelta;
    if (fpsTimer_ >= kFpsUpdateInterval) {
        fps_ = static_cast<float>(framesSinceFpsUpdate_ / fpsTimer_);
        frameMs_ = static_cast<float>(1000.0 * fpsTimer_ / framesSinceFpsUpdate_);
        framesSinceFpsUpdate_ = 0;
        fpsTimer_ = 0.0;
    }
}
