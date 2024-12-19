#pragma once
#include <Siv3D.hpp>
#include <FlexLayout.hpp>

class SimpleGUIButton : public FlexLayout::UIState
{
public:

	void draw(FlexLayout::UIStateQuery query, const FlexLayout::Box& box) override;

	void setTextContent(FlexLayout::UIStateQuery query, const StringView text) override;

	std::unique_ptr<FlexLayout::UIState> clone() override;

	bool clicked() const
	{
		return m_clicked;
	}

	void setEnabled(bool enabled)
	{
		m_enabled = enabled;
	}

	bool enabled() const
	{
		return m_enabled;
	}

private:

	bool m_clicked;

	bool m_enabled = true;
};

class SimpleGUISlider : public FlexLayout::UIState
{
public:

	void attach(FlexLayout::UIStateQuery query) override;

	void draw(FlexLayout::UIStateQuery query, const FlexLayout::Box& box) override;

	std::unique_ptr<FlexLayout::UIState> clone() override;

	double value() const
	{
		return m_value;
	}

	void setEnabled(bool enabled)
	{
		m_enabled = enabled;
	}

	bool enabled() const
	{
		return m_enabled;
	}

private:

	double m_value;

	bool m_enabled = true;
};
