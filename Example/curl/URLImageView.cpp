#include <ViewDesign/view/widget/DefaultWindow.h>
#include <ViewDesign/view/frame/MaxFrame.h>
#include <ViewDesign/view/frame/CenterFrame.h>
#include <ViewDesign/view/frame/ScaleFrame.h>
#include <ViewDesign/view/layout/SplitLayout.h>
#include <ViewDesign/view/control/TextView.h>
#include <ViewDesign/view/control/TextEditor.h>
#include <ViewDesign/view/control/ImageView.h>
#include <ViewDesign/view/wrapper/HitTestHelper.h>
#include <ViewDesign/view/wrapper/Background.h>
#include <ViewDesign/view/widget/FilledButton.h>
#include <ViewDesign/view/widget/Box.h>
#include <ViewDesign/view/widget/TextViewAdapter.h>
#include <ViewDesign/common/unicode.h>

#include <optional>


using namespace ViewDesign;


namespace {

std::string ToNarrowString(const u16string& str) {
	u8string u8 = to_u8string(str);
	return std::string(as_char_str(u8.c_str()), u8.size());
}

} // namespace


class LoadImageWindow : public DefaultBackground<DefaultWindow> {
private:
	struct Style : DefaultWindow::Style {
		Style() {
			width.min(px(500.0f));
			height.fix(px(100.0f));
		}
	};

public:
	LoadImageWindow(std::optional<Image>& image) : Base(
		Style(),
		u"Load Image From URL",
		new CenterFrame<Fixed, Fixed>(
			new SplitLayoutHorizontal(
				url_input_box = new Box(
					Margin(5.0f, 0.0f), Border(2.0f, border_color_normal), color_transparent, Padding(3.0f),
					TextViewAdapter<Fixed, Fixed>(
						url_input = new TextEditor(TextEditor::Style(), url_default)
					)
				),
				new LoadButton(*this)
			)
		)
	), image(image) {}

private:
	class LoadButton : public HitSelf<FilledButton<DefaultBackground<ViewFrame>>>, public SizeTrait<Auto, Auto> {
	public:
		LoadButton(LoadImageWindow& window) : Base(
			Base::Style(),
			[&] { window.TryLoadImage(); },
			new MaxFrame<Auto, Auto>(
				size_infinite,
				new Box(
					Margin(0.0f), Border(1.0f, ColorCode::Black), color_transparent, Padding(20.0f, 5.0f),
					new TextView(TextView::Style(), u"load")
				)
			)
		) {}
	};

private:
	inline static const u16string url_default = u"";
private:
	static constexpr Color border_color_normal = ColorCode::Black;
	static constexpr Color border_color_error = ColorCode::Red;
private:
	ref_ptr<TextEditor> url_input;
	ref_ptr<Box<Fixed, Fixed>> url_input_box;
	Timer reset_border_timer = Timer([&] { url_input_box->SetBorderColor(border_color_normal); });
private:
	std::optional<Image>& image;
private:
	void TryLoadImage() {
		try {
			image.emplace(ToNarrowString(url_input->GetText()));
			Close();
		} catch (...) {
			url_input_box->SetBorderColor(border_color_error);
			reset_border_timer.Set(3000);
		}
	}
};


void App() {
	std::optional<Image> image;
	desktop.AddWindow(new LoadImageWindow(image));
	desktop.EventLoop();

	if (!image.has_value()) {
		return;
	}

	desktop.AddWindow(
		new DefaultWindow(
			DefaultWindow::Style(),
			u"URL Image (hint: zoom with Ctrl + scroll)",
			new CenterFrame<Fixed, Fixed>(
				new ScaleFrame(
					new Stateful::ImageView(*image)
				)
			)
		)
	);
	desktop.EventLoop();
}
