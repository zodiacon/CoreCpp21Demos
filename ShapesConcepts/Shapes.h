#pragma once

#include <concepts>
#include <iostream>
#define __cpp_lib_format
#include <format>

enum class Color {
	Black, White, Green, Red, Blue
};

template<typename T>
concept Shape = requires(T s) {
	{ s.template GetColor() } -> std::convertible_to<Color>;
	s.template Draw();
};


struct Rectangle {
	Rectangle(double width, double height) : _width(width), _height(height) {}

	auto GetColor() const {
		return Color::Blue;
	}

	void Draw() const {
		std::cout << std::format("Drawing Rectangle ({},{})\n", _width, _height);
	}

private:
	double _width, _height;
};

struct Ellipse {
	Ellipse(double radius1, double radius2) : _radius1(radius1), _radius2(radius2) {}

	auto GetColor() const {
		return Color::White;
	}

	void Draw() const {
		std::cout << std::format("Drawing Ellipse ({},{})\n", _radius1, _radius2);
	}

protected:
	double _radius1, _radius2;
};

struct Circle : Ellipse {
	Circle(double radius) : Ellipse(radius, radius) {}

	auto GetColor() const {
		return Color::Red;
	}
	void Draw() const {
		std::cout << std::format("Drawing Circle ({})\n", _radius1);
	}
};

struct Polygon {
	Color GetColor() const {
		return Color::Black;
	}
};

static_assert(Shape<Circle>);
static_assert(Shape<Ellipse>);
//static_assert(Shape<Polygon>);