#include "Application.hpp"

#include "State.hpp"
#include "PlayState.hpp"
#include "MenuState.hpp"

#include <memory>

Application::Application() :
	m_window(sf::VideoMode(512, 384), "Lightspeed", sf::Style::Titlebar | sf::Style::Close)
{
	m_icon.loadFromFile("assets/icon.png");
	m_window.setIcon(48, 48, m_icon.getPixelsPtr());
	m_window.setFramerateLimit(60);
}

void Application::run()
{
	std::unique_ptr<State> state(new MenuState(m_window));

	while(state != nullptr)
		state = state->run();
}
