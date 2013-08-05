#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

class State
{
public:
	State(sf::RenderWindow& window) :
		m_window(window)
	{
		m_window.setVerticalSyncEnabled(true);
	}

	virtual std::unique_ptr<State> run() = 0;

protected:
	virtual void update() = 0;
	virtual void render() = 0;

protected:
	sf::RenderWindow& m_window;
};
