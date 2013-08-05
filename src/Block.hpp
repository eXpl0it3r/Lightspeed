#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace sf
{
	class RenderTarget;
	class Time;
}

class Block : public sf::Drawable
{
public:
	Block(unsigned int column, float position, const sf::Color& color);

	void update(const sf::Time& dt);

	void preview(bool state);
	void speed(float speed);
	float speed() const;
	unsigned int column() const;
	sf::FloatRect bounding_box();
	sf::FloatRect beam_bounding_box();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::RectangleShape m_block;
	sf::RectangleShape m_preview;

	float m_speed;
	unsigned int m_column;
	bool m_show_preview;
};
