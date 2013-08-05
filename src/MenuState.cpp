#include "MenuState.hpp"

#include "PlayState.hpp"

#include <SFML/System/String.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>

#include <utility>

MenuState::MenuState(sf::RenderWindow& window) :
	State(window),
	m_selection(0),
	m_status(Menu)
{
	m_offscreen.create(m_window.getSize().x, m_window.getSize().y);

	m_font.loadFromFile("assets/DejaVuSans.ttf");

	m_shadows.insert(std::make_pair<std::string, sf::Text>("play", sf::Text("PLAY", m_font, 30U)));
	m_texts.insert(std::make_pair<std::string, sf::Text>("play", sf::Text("PLAY", m_font, 30U)));
	m_shadows.insert(std::make_pair<std::string, sf::Text>("credits", sf::Text("CREDITS", m_font, 30U)));
	m_texts.insert(std::make_pair<std::string, sf::Text>("credits", sf::Text("CREDITS", m_font, 30U)));
	m_shadows.insert(std::make_pair<std::string, sf::Text>("exit", sf::Text("EXIT", m_font, 30U)));
	m_texts.insert(std::make_pair<std::string, sf::Text>("exit", sf::Text("EXIT", m_font, 30U)));

	for(auto& text : m_texts)
	{
		text.second.setStyle(sf::Text::Bold);
		text.second.setColor(sf::Color(0xFF, 0x66, 0x00));
	}

	m_shadows["play"].setPosition({220.f, 72.f});
	m_texts["play"].setPosition({218.f, 70.f});
	m_shadows["credits"].setPosition({193.f, 172.f});
	m_texts["credits"].setPosition({191.f, 170.f});
	m_shadows["exit"].setPosition({222.f, 272.f});
	m_texts["exit"].setPosition({220.f, 270.f});

	m_shadows["play"].setColor(sf::Color(0x44, 0x44, 0x44));
	m_shadows["credits"].setColor(sf::Color(0x44, 0x44, 0x44));
	m_shadows["exit"].setColor(sf::Color(0x44, 0x44, 0x44));

	m_overlay.setTexture(m_offscreen.getTexture());
	m_overlay.setOrigin({0.f, 50.f});
	m_overlay.setPosition({106.f, 88.f});

	m_tex_highlight.loadFromFile("assets/highlight.png");
	m_spr_highlight.setTexture(m_tex_highlight);

	m_tex_credits.loadFromFile("assets/credits.png");
	m_spr_credits.setTexture(m_tex_credits);
}

std::unique_ptr<State> MenuState::run()
{
	while(m_window.isOpen() && m_status != Play && m_status != Exit)
	{
		update();
		render();
	}

	if(m_status == Play)
		return std::unique_ptr<State>(new PlayState(m_window));

	return nullptr;
}

void MenuState::update()
{
	sf::Event event;
	while(m_window.pollEvent(event))
	{
		switch(event.type)
		{
			case sf::Event::Closed:
				m_window.close();
			break;
			case sf::Event::KeyPressed:
				if(event.key.code == sf::Keyboard::Down && m_selection < 2)
				{
					m_spr_highlight.move({0, 100.f});
					++m_selection;
				}
				else if(event.key.code == sf::Keyboard::Up && m_selection > 0)
				{
					m_spr_highlight.move({0, -100.f});
					--m_selection;
				}
				else if(event.key.code == sf::Keyboard::Return)
				{
					if(m_status == Credits)
						m_status = Menu;
					else
					{
						if(m_selection == 0)
							m_status = Play;
						else if(m_selection == 1)
							m_status = Credits;
						else if(m_selection == 2)
							m_status = Exit;
					}
				}
				else if(event.key.code == sf::Keyboard::Escape)
				{
					if(m_status == Credits)
						m_status = Menu;
					else if(m_status == Menu)
						m_status = Exit;
				}
			break;
			default:
			break;
		}
	}


}

void MenuState::render()
{
	m_offscreen.clear(sf::Color(0, 0, 0, 220));
	m_offscreen.draw(m_spr_highlight, sf::BlendNone);
	m_offscreen.display();

	m_window.clear();

	for(auto& shadow : m_shadows)
		m_window.draw(shadow.second);

	for(auto& text : m_texts)
		m_window.draw(text.second);

	m_window.draw(m_overlay);

	if(m_status == Credits)
		m_window.draw(m_spr_credits);

	m_window.display();
}
