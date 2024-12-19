#include "SimpleGUIConnector.hpp"

void SimpleGUIButton::draw(FlexLayout::UIStateQuery query, const FlexLayout::Box& box)
{
	auto rect = box.contentAreaRect();
	if (rect)
	{
		m_clicked = SimpleGUI::Button(
			query.textContent(),
			rect->pos,
			rect->w,
			m_enabled
		);
	}
	else
	{
		m_clicked = false;
	}
}

void SimpleGUIButton::setTextContent(FlexLayout::UIStateQuery query, const StringView text)
{
	auto minSize = SimpleGUI::ButtonRegion(text, { 0, 0 }).size;

	query.setStyle(
		U"min-width",
		FlexLayout::StyleValue::Length(static_cast<float>(minSize.x), FlexLayout::LengthUnit::Pixel)
	);
	query.setStyle(
		U"min-height",
		FlexLayout::StyleValue::Length(static_cast<float>(minSize.y), FlexLayout::LengthUnit::Pixel)
	);
}

std::unique_ptr<FlexLayout::UIState> SimpleGUIButton::clone()
{
	auto ptr = std::make_unique<SimpleGUIButton>();

	ptr->m_enabled = m_enabled;

	return std::move(ptr);
}

void SimpleGUISlider::attach(FlexLayout::UIStateQuery query)
{
	auto minSize = SimpleGUI::SliderRegion({ 0, 0 }, 0, 0).size;

	query.setStyle(
		U"min-width",
		FlexLayout::StyleValue::Length(static_cast<float>(minSize.x), FlexLayout::LengthUnit::Pixel)
	);
	query.setStyle(
		U"min-height",
		FlexLayout::StyleValue::Length(static_cast<float>(minSize.y), FlexLayout::LengthUnit::Pixel)
	);
}

void SimpleGUISlider::draw(FlexLayout::UIStateQuery query, const FlexLayout::Box& box)
{
	auto rect = box.contentAreaRect();
	if (rect)
	{
		SimpleGUI::Slider(m_value, rect->pos, rect->w, m_enabled);
	}
}

std::unique_ptr<FlexLayout::UIState> SimpleGUISlider::clone()
{
	return std::make_unique<SimpleGUISlider>();
}
