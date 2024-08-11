#pragma once
#include "../algorithm/smart_algorithm.h"
#include <queue>

class SDFS : public SmartAlgorithm {
  private:
    std::queue<RelativePoint> points_of_interest;

    Step nextStep() override;
};