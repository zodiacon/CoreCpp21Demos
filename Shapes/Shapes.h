#pragma once

struct DrawingContext {
	static DrawingContext& Get();
};

enum class Color {
	Black, White, Green, Red, Blue
};

struct Shape abstract {
	virtual Color GetColor() const = 0;
	virtual void Draw(DrawingContext& dc) const = 0;
	virtual ~Shape() = default;

protected:
	Shape() = default;
};

struct Rectangle : Shape {
	Rectangle(double width, double height);

	Color GetColor() const override {
		return Color::Blue;
	}
	void Draw(DrawingContext& dc) const override {}
};

struct Ellipse : Shape {
	Ellipse(double r1, double r2);

	Color GetColor() const override {
		return Color::Green;
	}
	void Draw(DrawingContext& dc) const override {}
};

struct Circle : Ellipse {
	Circle(double radius) : Ellipse(radius, radius) {}

	Color GetColor() const override {
		return Color::Red;
	}
};
