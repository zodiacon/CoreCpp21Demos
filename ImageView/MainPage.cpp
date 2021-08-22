#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;

namespace winrt::ImageView::implementation {
	MainPage::MainPage()     {
		InitializeComponent();
	}

	int32_t MainPage::MyProperty() {
		throw hresult_not_implemented();
	}

	void MainPage::MyProperty(int32_t /* value */)     {
		throw hresult_not_implemented();
	}

	fire_and_forget MainPage::OnChooseImage(IInspectable const&, RoutedEventArgs const&)     {
		FileOpenPicker picker;
		picker.ViewMode(PickerViewMode::Thumbnail);
		picker.FileTypeFilter().ReplaceAll({ L".jpg", L".png" });
		picker.SuggestedStartLocation(PickerLocationId::PicturesLibrary);

		auto file = co_await picker.PickSingleFileAsync();
		if (file == nullptr)
			co_return;

		auto stm = co_await file.OpenReadAsync();
		BitmapImage bmp;
		bmp.SetSource(stm.as<IRandomAccessStream>());
		_image().Source(bmp);
	}
}
