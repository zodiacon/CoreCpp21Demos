#include <vector>
#include <memory>
#include "Shapes.h"

using namespace std;

DrawingContext& DrawingContext::Get() {
	static DrawingContext context;
	return context;
}

Ellipse::Ellipse(double r1, double r2) {
}

Rectangle::Rectangle(double width, double height) {
}

void DrawAll(vector<unique_ptr<Shape>> const& shapes, DrawingContext& dc) {
	for (auto& s : shapes)
		s->Draw(dc);
}

void BuildDrawing() {
	vector<unique_ptr<Shape>> shapes;
	shapes.push_back(make_unique<Circle>(6));
	shapes.push_back(make_unique<Rectangle>(4, 10));
	shapes.push_back(make_unique<Ellipse>(10, 3));

	auto& dc = DrawingContext::Get();
	DrawAll(shapes, dc);
}


int main() {
	BuildDrawing();
	return 0;
}

