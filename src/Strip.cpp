#include "Strip.hpp"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

#include <iostream>

Strip::Strip(float speed, unsigned int column, const sf::Color& color) :
	m_vertices(28),
	m_background(sf::Vector2f(SIZE, SIZE*NO_ROW)),
	m_speed(1.f),
	m_offset(0.f),
	m_column(0),
	m_hue_shift(false)
{
	this->speed(speed);
	this->column(column);

	for(unsigned int i=0; i < NO_ROW+1; ++i)
	{
		m_vertices[4*i].position = sf::Vector2f(SIZE*m_column, SIZE*i);
		m_vertices[4*i+1].position = sf::Vector2f(SIZE*m_column, SIZE*(i+1));
		m_vertices[4*i+2].position = sf::Vector2f(SIZE*(m_column+1), SIZE*(i+1));
		m_vertices[4*i+3].position = sf::Vector2f(SIZE*(m_column+1), SIZE*i);

		m_vertices[4*i].texCoords = sf::Vector2f(0.f, 0.f);
		m_vertices[4*i+1].texCoords = sf::Vector2f(0.f, SIZE);
		m_vertices[4*i+2].texCoords = sf::Vector2f(SIZE, SIZE);
		m_vertices[4*i+3].texCoords = sf::Vector2f(SIZE, 0.f);
	}

	m_background.setPosition(SIZE*m_column, 0.f);
	m_background.setFillColor(color);
}

void Strip::reset()
{
	for(unsigned int i=0; i < NO_ROW+1; ++i)
	{
		m_vertices[4*i].position = sf::Vector2f(SIZE*m_column, SIZE*i);
		m_vertices[4*i+1].position = sf::Vector2f(SIZE*m_column, SIZE*(i+1));
		m_vertices[4*i+2].position = sf::Vector2f(SIZE*(m_column+1), SIZE*(i+1));
		m_vertices[4*i+3].position = sf::Vector2f(SIZE*(m_column+1), SIZE*i);

		m_vertices[4*i].texCoords = sf::Vector2f(0.f, 0.f);
		m_vertices[4*i+1].texCoords = sf::Vector2f(0.f, SIZE);
		m_vertices[4*i+2].texCoords = sf::Vector2f(SIZE, SIZE);
		m_vertices[4*i+3].texCoords = sf::Vector2f(SIZE, 0.f);
	}
}

void Strip::hue_shift(bool state)
{
	m_hue_shift = state;
}

void Strip::color(const sf::Color& color)
{
	m_background.setFillColor(color);
}

float Strip::top()
{
	return m_vertices[0].position.y;
}

void Strip::speed(float speed)
{
	if(speed > 0.f)
		m_speed = speed;
}

float Strip::speed() const
{
	return m_speed;
}

void Strip::column(unsigned int column)
{
	if(column >= 0 && column < NO_COLUMN)
		m_column = column;
}

unsigned int Strip::column() const
{
	return m_column;
}

void Strip::offset(float offset)
{
	m_offset = offset;
}

float Strip::offset() const
{
	return m_offset;
}

void Strip::update(const sf::Time& dt)
{
	//std::cout << dt.asSeconds() << ":" << m_speed << ":" << m_vertices[0].position.y << std::endl;

	// Reset positions
	if(m_vertices[0].position.y + m_speed*dt.asSeconds() >= 0)
	{
		for(auto &vertex : m_vertices)
			vertex.position.y += m_speed*dt.asSeconds()-SIZE;
	}
	else
	{
		for(auto &vertex : m_vertices)
			vertex.position.y += m_speed*dt.asSeconds();
	}
}

void Strip::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(m_hue_shift)
		target.draw(m_background, states);

	target.draw(m_vertices.data(), m_vertices.size(), sf::PrimitiveType::Quads, states);
}

const unsigned int Strip::NO_COLUMN = 8;
const unsigned int Strip::NO_ROW = 6;
const float Strip::SIZE = 64.f;
