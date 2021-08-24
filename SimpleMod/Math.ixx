module;

#include <cmath>

export module Math.Simple;

#define SQR(x) ((x)*(x))

namespace Math {
	export struct Point {
		double x;
		double y;
	};

	double DistanceSquared(Point const& p1, Point const& p2);

	export template<typename T>
		T Add(T a, T b) {
		return a + b;
	}

	export double Distance(Point const& p1, Point const& p2) {
		return std::sqrt(DistanceSquared(p1, p2));
	}

	double DistanceSquared(Point const& p1, Point const& p2) {
		return SQR(p1.x - p2.x) + SQR(p1.y - p2.y);
	}
}
