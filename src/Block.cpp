#include "Block.hpp"

#include "Strip.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

Block::Block(unsigned int column, float position, const sf::Color& color) :
	m_block({54.f, 54.f}),
	m_preview({34.f, 384.f}),
	m_speed(2.f),
	m_column(column),
	m_show_preview(true)
{
	m_block.setFillColor(color);
	m_block.setPosition(column*Strip::SIZE+5.f, position-Strip::SIZE*2.f+5.f);
	m_block.setOutlineColor(sf::Color::Black);
	m_block.setOutlineThickness(2.f);

	m_preview.setFillColor(sf::Color(color.r, color.g, color.b, 100));
}

void Block::update(const sf::Time& dt)
{
	auto pos = m_block.getPosition();
	pos.y += m_speed*dt.asSeconds();
	m_block.setPosition(pos);

	pos.x += 10.f;
	pos.y += m_block.getSize().y + 5.f;
	m_preview.setPosition(pos);
	m_preview.setSize({34.f, 384.f-pos.y});
}

void Block::preview(bool state)
{
	m_show_preview = state;
}

void Block::speed(float speed)
{
	if(speed > 0.f)
		m_speed = speed;
}

float Block::speed() const
{
	return m_speed;
}

unsigned int Block::column() const
{
	return m_column;
}

sf::FloatRect Block::bounding_box()
{
	return m_block.getGlobalBounds();
}

sf::FloatRect Block::beam_bounding_box()
{
	return m_preview.getGlobalBounds();
}

void Block::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_block, states);

	if(m_show_preview)
		target.draw(m_preview, states);
}
