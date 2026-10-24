#include "statistics.hpp"

#include <cassert>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

namespace {
bool near(double lhs, double rhs, double tolerance = 1e-9) {
    return std::abs(lhs - rhs) < tolerance;
}

template <typename F>
void expect_invalid_argument(F&& function) {
    bool thrown = false;
    try {
        function();
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown && "expected std::invalid_argument");
}
} // namespace

int main() {
    {
        const std::vector values{1.20, 1.25, 1.18, 1.31};
        const auto result = calculate_statistics(values);
        assert(near(result.minimum, 1.18));
        assert(near(result.maximum, 1.31));
        assert(near(result.mean, 1.235));
        assert(near(result.range, 0.13));
    }

    {
        const std::vector values{-4.0, -2.0, -3.0};
        const auto result = calculate_statistics(values);
        assert(near(result.minimum, -4.0));
        assert(near(result.maximum, -2.0));
        assert(near(result.mean, -3.0));
        assert(near(result.range, 2.0));
    }

    {
        const std::vector values{5.0};
        const auto result = calculate_statistics(values);
        assert(near(result.minimum, 5.0));
        assert(near(result.maximum, 5.0));
        assert(near(result.mean, 5.0));
        assert(near(result.range, 0.0));
    }

    {
        const std::vector<double> values;
        expect_invalid_argument([&] { (void)calculate_statistics(values); });
    }

    {
        const std::vector values{1.0, std::numeric_limits<double>::quiet_NaN()};
        expect_invalid_argument([&] { (void)calculate_statistics(values); });
    }

    {
        const std::vector values{1.0, std::numeric_limits<double>::infinity()};
        expect_invalid_argument([&] { (void)calculate_statistics(values); });
    }
}
