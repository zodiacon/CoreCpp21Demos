#include "Shapes.h"
#include <tuple>

void DrawShape(Shape auto const& s) {
	s.Draw();
}

template<size_t Index = 0, Shape... Ts>
constexpr void DrawAll(std::tuple<Ts...> const& shapes) {
	if constexpr (Index == sizeof...(Ts)) {
		return;
	}
	else {
		DrawShape(std::get<Index>(shapes));
		DrawAll<Index + 1>(shapes);
	}
}

int main() {
	Circle c1(9);
	DrawShape(c1);
	Rectangle r1(10, 4);
	DrawShape(r1);
	
	std::cout << std::endl;

	std::tuple shapes{ c1, r1, Ellipse(7, 12), Circle(20), Rectangle(5, 6) };
	DrawAll(shapes);

	std::cout << std::endl;

	auto shapes2 = std::tuple_cat(shapes, std::make_tuple(Ellipse(100, 10)));
	DrawAll(shapes2);
}

