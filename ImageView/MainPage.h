#pragma once

#include "MainPage.g.h"

namespace winrt::ImageView::implementation {
	struct MainPage : MainPageT<MainPage>     {
		MainPage();

		int32_t MyProperty();
		void MyProperty(int32_t value);

		fire_and_forget OnChooseImage(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
	};
}

namespace winrt::ImageView::factory_implementation {
	struct MainPage : MainPageT<MainPage, implementation::MainPage>     {
	};
}
