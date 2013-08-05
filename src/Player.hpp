#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class RenderTarget;
	class Time;
}

class Player : public sf::Drawable
{
public:
	enum Direction
	{
		Left,
		Right
	};

public:
	Player();

	void move(Direction direction, bool faster, const sf::Time& dt);

	void speed(float speed);
	float speed() const;
	sf::FloatRect bounding_box();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::RectangleShape m_player;

	float m_speed;

};
