#include "Player.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>

Player::Player() :
	m_player(sf::Vector2f(40.f, 40.f)),
	m_speed(180.f)
{
	m_player.setFillColor(sf::Color(0x1D, 0x41, 0x47));
	m_player.setPosition(sf::Vector2f(236.f, 300.f));
}

void Player::move(Direction direction, bool faster, const sf::Time& dt)
{
	float tmp_speed = m_speed;

	if(faster)
		tmp_speed *= 2;

	if(direction == Left)
		tmp_speed = -tmp_speed;

	m_player.move(tmp_speed*dt.asSeconds(), 0.f);
}

void Player::speed(float speed)
{
	if(speed > 0.f)
		m_speed = speed;
}

float Player::speed() const
{
	return m_speed;
}

sf::FloatRect Player::bounding_box()
{
	return m_player.getGlobalBounds();
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_player, states);
}
