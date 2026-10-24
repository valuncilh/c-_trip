#include "statistics.hpp"

#include <cmath>
#include <stdexcept>

Statistics calculate_statistics(std::span<const double> values) {
    if (values.empty()) {
        throw std::invalid_argument("measurements must not be empty");
    }

    for (const double value : values) {
        if (!std::isfinite(value)) {
            throw std::invalid_argument("measurements must be finite");
        }
    }

    double minimum = values.front();
    double maximum = values.front();
    double sum = 0.0;

    for (const double value : values) {
        if (value < minimum) {
            minimum = value;
        }
        if (value > maximum) {
            maximum = value;
        }
        sum += value;
    }

    const double mean = sum / static_cast<double>(values.size());
    return Statistics{minimum, maximum, mean, maximum - minimum};
}
