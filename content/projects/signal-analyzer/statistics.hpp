#pragma once

#include <span>

struct Statistics {
    double minimum;
    double maximum;
    double mean;
    double range;
};

// Throws std::invalid_argument for an empty range or non-finite value.
// Does not modify the input range.
Statistics calculate_statistics(std::span<const double> values);
