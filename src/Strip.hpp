#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <vector>

namespace sf
{
	class Time;
	class RenderTarget;
	class Color;
}

class Strip : public sf::Drawable
{
public:
	Strip(float speed, unsigned int column, const sf::Color& color);

	void update(const sf::Time& dt);

	void reset();
	void hue_shift(bool state);
	float top();
	void color(const sf::Color& color);
	void speed(float speed);
	float speed() const;
	void column(unsigned int column);
	unsigned int column() const;
	void offset(float offset);
	float offset() const;

	static const unsigned int NO_COLUMN;
	static const unsigned int NO_ROW;
	static const float SIZE;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<sf::Vertex> m_vertices;
	sf::RectangleShape m_background;
	float m_speed;
	float m_offset;
	unsigned int m_column;
	bool m_hue_shift;

};
