#include <Siv3D.hpp>
#include <FlexLayout.hpp>
#include "SimpleGUIConnector.hpp"
#include "Confetti.hpp"

struct Page
{
	StringView title;
	StringView filePath;
};

Array<Page> Pages = {
	Page{.title = U"", .filePath = U"Title.xml" },
	Page{.title = U"注意事項", .filePath = U"First.xml" },
	Page{.title = U"OpenSiv3D(Siv3D)とは？", .filePath = U"WhatIsSiv3D.xml" },
	Page{.title = U"Siv3DのGUI環境", .filePath = U"Problem.xml" },
	Page{.title = U"FlexLayout", .filePath = U"FlexLayout.xml" },
	Page{.title = U"FlexLayoutの構成", .filePath = U"Structure.xml" },
	Page{.title = U"デモ1", .filePath = U"Demo1.xml" },
	Page{.title = U"デモ2", .filePath = U"Demo2.xml" },
	Page{.title = U"おわりに", .filePath = U"Conclusion.xml" }
};

void Main()
{
	int currentPage = 0;
	bool showPointer = false;

	Image pointerImg{ Emoji{U"👆️"} };
	Vec2 pointerHotspot = Vec2{ 0.4, 0.1 } * pointerImg.size();
	pointerImg.scale(0.7);
	pointerHotspot *= 0.7;
	Cursor::RegisterCustomCursorStyle(U"yubi", pointerImg, pointerHotspot.asPoint());

	Texture structureTex{ U"FlexLayout.drawio.png" };

	// Scene::SetBackground(Palette::White);
	Scene::Resize(1280, 720);
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetToggleFullscreenEnabled(true);
	Window::SetStyle(WindowStyle::Sizable);

	FlexLayout::Layout presentationLayout;
	presentationLayout.registerCustomComponent<SimpleGUIButton>(U"SimpleGUI.Button");
	presentationLayout.load(U"MainLayout.xml", FlexLayout::EnableHotReload::Yes);
	presentationLayout.setConstraints(Scene::Rect());
	auto root = *presentationLayout.document();
	auto header = *root.getElementById(U"header");
	auto titleText = *root.getElementById(U"title-text");
	auto main = *root.getElementById(U"main");
	auto footer = *root.getElementById(U"footer");
	auto dateText = *root.getElementById(U"date-text");
	auto pageNumberText = *root.getElementById(U"page-number-text");
	auto pointerButton = *root.getElementById(U"pointer-button")->as<SimpleGUIButton>();
	auto backButton = *root.getElementById(U"back-button")->as<SimpleGUIButton>();
	auto forwardButton = *root.getElementById(U"forward-button")->as<SimpleGUIButton>();

	FlexLayout::Layout mainLayout;
	mainLayout.registerCustomComponent<SimpleGUIButton>(U"SimpleGUI.Button");
	mainLayout.registerCustomComponent<SimpleGUISlider>(U"SimpleGUI.Slider");
	mainLayout.load(Pages[0].filePath, FlexLayout::EnableHotReload::Yes);
	titleText.setTextContent(Pages[0].title);
	pageNumberText.setTextContent(U"0");
	dateText.setTextContent(Format(DateTime::Today()).substr(0, 10));

	Confetti confetti{ 3.5s };
	bool showConfetti = false;
	const double SpawnInterval = 0.03;
	double accumulatedTime = 0.0;

	while (System::Update())
	{
		// レイアウト計算

		presentationLayout.handleHotReload();
		presentationLayout.calculateLayout();

		mainLayout.handleHotReload();
		mainLayout.setConstraints(*main.rect());
		mainLayout.calculateLayout();

		// 更新

		presentationLayout.updateUI();
		mainLayout.updateUI();

		// 紙吹雪を更新
		auto confettiArea = mainLayout.document()->getElementById(U"confetti-area");
		if (confettiArea)
		{
			accumulatedTime += Scene::DeltaTime();

			if (auto rect = confettiArea->rect();
				rect && showConfetti)
			{
				while (SpawnInterval <= accumulatedTime)
				{
					const Vec2 pos = RandomVec2(RectF{ rect->x - 20, rect->y, rect->w + 40, 10 });
					const Vec2 startVelocity = (Vec2{ 0, 240 } + RandomVec2(120.0));
					const double size = Random(20.0, 28.0);
					const double hue = Random(360.0);
					confetti.add(pos, startVelocity, size, hue);
					accumulatedTime -= SpawnInterval;
				}
			}

			confetti.update();
		}

		// 描画

		if (not titleText.textContent().empty())
		{
			header.drawFrame(Palette::White);
		}
		footer.drawFrame(Palette::White);

		mainLayout.drawUI();

		if (auto disp = mainLayout.document()->getElementById(U"sample1-display"))
		{
			disp->drawFrame();
			if (auto rect = disp->rect())
			{
				ScopedViewport2D vp{ rect->pos.asPoint(), rect->size.asPoint() };
				Transformer2D tf2{ Mat3x2::Identity(), Mat3x2::Translate(rect->pos) };
				Circle{ Cursor::PosF(), 30}.draw(Palette::Orange);
			}
		}

		if (auto disp = mainLayout.document()->getElementById(U"structure-fig"))
		{
			if (auto rect = disp->rect())
			{
				double scale = Min(rect->w / structureTex.width(), rect->h / structureTex.height());
				structureTex
					.scaled(scale)
					.drawAt(rect->center());
			}
		}

		presentationLayout.drawUI();

		// 紙吹雪を描画
		if (auto confettiButton = mainLayout.document()->getElementById(U"confetti-btn"))
		{
			if (auto btnRef = confettiButton->as<SimpleGUIButton>();
				btnRef && btnRef->state().clicked())
			{
				showConfetti = !showConfetti;
			}
		}
		if (confettiArea && showConfetti)
		{
			confetti.draw();
		}

		// その他処理

		if (pointerButton->clicked())
		{
			showPointer = !showPointer;
		}

		if (showPointer)
		{
			Cursor::RequestStyle(U"yubi");
		}

		backButton->setEnabled(currentPage > 0);
		if (backButton->enabled() && (backButton->clicked() || KeyLeft.down()))
		{
			currentPage--;
			titleText.setTextContent(Pages[currentPage].title);
			pageNumberText.setTextContent(Format(currentPage));
			mainLayout.load(Pages[currentPage].filePath, FlexLayout::EnableHotReload::Yes);
		}

		forwardButton->setEnabled(currentPage < Pages.size() - 1);
		if (forwardButton->enabled() && (forwardButton->clicked() || KeyRight.down()))
		{
			currentPage++;
			titleText.setTextContent(Pages[currentPage].title);
			pageNumberText.setTextContent(Format(currentPage));
			mainLayout.load(Pages[currentPage].filePath, FlexLayout::EnableHotReload::Yes);
		}
	}
}
