#include "PlayState.hpp"

#include "Utility.hpp"
#include "MenuState.hpp"

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>
#include <chrono>
#include <sstream>

PlayState::PlayState(sf::RenderWindow& window) :
	State(window),
	m_generator(std::chrono::system_clock::now().time_since_epoch().count()),
	m_speed(200, 300),
	m_color(0, 255),
	m_state(0, 3),
	m_block(1, 100),
	m_strip(0, 7),
	m_duration(5, 10),
	m_hue(0.f, 1.f),
	m_hues(Strip::NO_COLUMN, 0.f),
	m_gen_cooldown(sf::seconds(1.f)),
	m_gen_count(sf::Time::Zero),
	m_target_speed(250.f),
	m_background(sf::Color(0x05, 0xA0, 0xA0)),
	m_go_overlay(static_cast<sf::Vector2f>(window.getSize())),
	m_menu({300.f, 200.f}),
	m_selection(3.f, 12.f),
	m_menu_state(0)
{
	m_tex_shadow.loadFromFile("assets/shadow.png");
	m_tex_strip.loadFromFile("assets/strip.png");
	m_snd_beam.loadFromFile("assets/beam.wav");
	m_ply_beam.setBuffer(m_snd_beam);
	m_snd_hit.loadFromFile("assets/boom.wav");
	m_ply_hit.setBuffer(m_snd_hit);
	m_font.loadFromFile("assets/DejaVuSans.ttf");
	m_shadow.setTexture(m_tex_shadow);

	m_points.setFont(m_font);
	m_points.setCharacterSize(20U);
	m_points.setStyle(sf::Text::Bold);
	m_points.setString("0");
	m_points.setPosition({42.f, 5.f});

	m_double.setFont(m_font);
	m_double.setCharacterSize(25U);
	m_double.setStyle(sf::Text::Bold);
	m_double.setColor(sf::Color(0xFF, 0xDD, 0xDD));
	m_double.setString("3x");
	m_double.setPosition({5.f, 1.f});

	m_effect_name.setFont(m_font);
	m_effect_name.setCharacterSize(25U);
	m_effect_name.setStyle(sf::Text::Bold);
	m_effect_name.setColor(sf::Color(0xFF, 0xDD, 0xDD));
	m_effect_name.setString("");
	m_effect_name.setPosition({350.f, 5.f});

	for(unsigned int i = 0; i < Strip::NO_COLUMN; ++i)
	{
		m_strips.emplace_back(Strip(m_target_speed, i, sf::Color(Utility::next_random(m_generator, m_color), Utility::next_random(m_generator, m_color), Utility::next_random(m_generator, m_color))));
	}

	for(auto &hue : m_hues)
		hue = Utility::next_random(m_generator, m_hue);

	// Over
	m_go_overlay.setFillColor(sf::Color(0x66, 0x22, 0x22, 0xDD));
	m_go_text.setFont(m_font);
	m_go_text.setCharacterSize(60U);
	m_go_text.setStyle(sf::Text::Bold);
	m_go_text.setColor(sf::Color(0xEE, 0x11, 0x11));
	m_go_text.setString("GAME OVER!");
	m_go_text.setPosition({50.f, 162.f});

	// Menu
	m_menu.setFillColor(sf::Color(0x22, 0x22, 0x22));
	m_menu.setPosition({106.f, 92.f});
	m_selection.setFillColor(sf::Color(0x05, 0xA0, 0xA0));
	m_selection.setPosition({150.f, 133.f});
	m_resume.setFont(m_font);
	m_resume.setCharacterSize(30U);
	m_resume.setColor(sf::Color(0x05, 0xA0, 0xA0));
	m_resume.setString("Resume");
	m_resume.setPosition({180.f, 120.f});
	m_restart.setFont(m_font);
	m_restart.setCharacterSize(30U);
	m_restart.setColor(sf::Color(0x05, 0xA0, 0xA0));
	m_restart.setString("Restart");
	m_restart.setPosition({180.f, 170.f});
	m_exit.setFont(m_font);
	m_exit.setCharacterSize(30U);
	m_exit.setColor(sf::Color(0x05, 0xA0, 0xA0));
	m_exit.setString("Exit");
	m_exit.setPosition({180.f, 220.f});
}

std::unique_ptr<State> PlayState::run()
{
	while(m_window.isOpen() && m_current_state.status != Restart && m_current_state.status != Exit)
	{
		update();
		render();
	}

	if(m_current_state.status == Restart)
		return std::unique_ptr<State>(new PlayState(m_window));

	return std::unique_ptr<State>(new MenuState(m_window));
}


void PlayState::speed_state_machine()
{
	if(m_current_state.speed_count >= m_current_state.speed_duration)
	{
		switch(m_current_state.speed)
		{
			default:
			case Normal:
				if(m_current_state.diff && m_current_state.fast)
					if(Utility::bin_random(m_generator))
						m_current_state.speed = Fast;
					else
						m_current_state.speed = SpeedDiff;
				else if(m_current_state.diff)
					if(Utility::bin_random(m_generator))
						m_current_state.speed = SpeedUp;
					else
						m_current_state.speed = SpeedDiff;
				else if(m_current_state.fast)
					if(Utility::bin_random(m_generator))
						m_current_state.speed = Drift;
					else
						m_current_state.speed = Fast;
				else
					if(Utility::bin_random(m_generator))
						m_current_state.speed = Drift;
					else
						m_current_state.speed = SpeedUp;
			break;
			case Fast:
				if(m_current_state.diff)
					if(Utility::bin_random(m_generator))
						m_current_state.speed = SlowDown;
					else
						m_current_state.speed = SpeedDiff;
				else
					if(Utility::bin_random(m_generator))
						m_current_state.speed = SlowDown;
					else
						m_current_state.speed = Drift;
			break;
			case SpeedDiff:
				if(m_current_state.fast)
					if(Utility::bin_random(m_generator))
						m_current_state.speed = Join;
					else
						m_current_state.speed = Fast;
				else
					if(Utility::bin_random(m_generator))
						m_current_state.speed = Join;
					else
						m_current_state.speed = SpeedUp;
			break;
			case SpeedUp:
				m_current_state.speed = Fast;
				m_current_state.fast = true;
			break;
			case SlowDown:
				m_current_state.speed = Normal;
				m_current_state.fast = false;

				m_target_speed = Utility::next_random(m_generator, m_speed);
			break;
			case Drift:
				m_current_state.speed = SpeedDiff;
				m_current_state.diff = true;
			break;
			case Join:
				m_current_state.speed = Normal;
				m_current_state.diff = false;

				// Force reset
				for(auto &strip : m_strips)
				{
					strip.reset();
					strip.speed(m_target_speed);
				}
			break;
		}

		m_current_state.speed_count = sf::Time::Zero;
		m_current_state.speed_duration = sf::seconds(Utility::next_random(m_generator, m_duration));

		// Post action
		switch(m_current_state.speed)
		{
			case Drift:
				for(auto &strip : m_strips)
				{
					strip.offset(Utility::next_random(m_generator, 400.f, 800.f));
					strip.speed(strip.speed() + strip.offset()/m_current_state.speed_duration.asSeconds());
				}
			break;
			case Join:
				for(auto &strip : m_strips)
				{
					strip.speed(strip.speed() - strip.offset()/m_current_state.speed_duration.asSeconds());

				}
			break;
			default:
			break;
		}
	}
	else
	{
		m_current_state.speed_count += m_delta;
	}
}

void PlayState::effect_state_machine()
{
	if(m_current_state.effect_count >= m_current_state.effect_duration)
	{
		switch(m_current_state.effect)
		{
			case NoEffect:
				m_current_state.effect = static_cast<EffectStates>(Utility::next_random(m_generator, 1, 3));
			break;
			default:
				m_current_state.effect = NoEffect;
			break;
		}

		m_current_state.effect_count = sf::Time::Zero;
		m_current_state.speed_duration = sf::seconds(Utility::next_random(m_generator, m_duration));

		// Post action
		for(auto &strip : m_strips)
			strip.hue_shift(false);

		for(auto &block : m_blocks)
			block.preview(true);

		m_background = sf::Color(0x50, 0xA0, 0xA0);

		switch(m_current_state.effect)
		{
			case HueShift:
				for(auto &strip : m_strips)
					strip.hue_shift(true);

				for(auto &block : m_blocks)
					block.preview(false);

				m_effect_name.setString("Hue Shift");
			break;
			case BlackOut:
				m_background = sf::Color::Black;

				for(auto &block : m_blocks)
					block.preview(false);

				m_effect_name.setString("Black Out");
			break;
			case Shadow:
				for(auto &block : m_blocks)
					block.preview(false);

				m_effect_name.setString("Shadow");
			break;
			default:
				m_effect_name.setString("");
			break;
		}
	}
	else
	{
		m_current_state.effect_count += m_delta;
	}
}

void PlayState::update()
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
				if(event.key.code == sf::Keyboard::Escape)
				{
					if(m_current_state.status == Menu)
						std::swap(m_current_state.status, m_current_state.last);
					else
					{
						std::swap(m_current_state.status, m_current_state.last);
						m_current_state.status = Menu;
					}
				}
				else if(event.key.code == sf::Keyboard::Down && m_current_state.status == Menu)
				{
					if(m_menu_state < 2)
					{
						m_selection.move(0.f, 50.f);
						++m_menu_state;
					}
				}
				else if(event.key.code == sf::Keyboard::Up && m_current_state.status == Menu)
				{
					if(m_menu_state > 0)
					{
						m_selection.move(0.f, -50.f);
						--m_menu_state;
					}
				}
				else if(event.key.code == sf::Keyboard::Return && m_current_state.status == Menu)
				{
					if(m_menu_state == 0)
						std::swap(m_current_state.status, m_current_state.last);
					else if(m_menu_state == 1)
					{
						std::swap(m_current_state.status, m_current_state.last);
						m_current_state.status = Restart;
					}
					else if(m_menu_state == 2)
					{
						m_current_state.status = Exit;
					}
				}
				else if(event.key.code == sf::Keyboard::R && m_current_state.status == Game)
				{
					m_current_state.status = Restart;
				}
			break;
			default:

			break;
		}
	}

	switch(m_current_state.status)
	{
		case Game:
		{
			speed_state_machine();
			effect_state_machine();

			switch(m_current_state.speed)
			{
				case SpeedUp:
					for(auto &strip : m_strips)
					{
						if(strip.speed() < m_target_speed*1.5f)
							strip.speed(strip.speed() + 5.f);
					}
				break;
				case SlowDown:
					for(auto &strip : m_strips)
					{
						if(strip.speed() > m_target_speed)
							strip.speed(strip.speed() - 5.f);
					}
				break;
				default:
				break;
			}

			switch(m_current_state.effect)
			{
				case HueShift:
					for(unsigned int i = 0; i < Strip::NO_COLUMN; ++i)
					{
						m_hues[i] += 0.004f;
						if(m_hues[i] > 1.f)
							m_hues[i] = 0.f;

						float r = std::abs(3.f - 6.f * m_hues[i]) - 1.f;
						float g = 2.f - std::abs(2.f - 6.f * m_hues[i]);
						float b = 2.f - std::abs(4.f - 6.f * m_hues[i]);

						m_strips[i].color(sf::Color(Utility::clamp(r, 0.f, 1.f) * 255, Utility::clamp(g, 0.f, 1.f) * 255, Utility::clamp(b, 0.f, 1.f) * 255));
					}
				break;
				default:
				break;
			}

			// Generate Blocks
			if(m_gen_count >= m_gen_cooldown)
			{
				if(Utility::next_random(m_generator, m_block) >= 50)
				{
					unsigned int pick = Utility::next_random(m_generator, m_strip);
					m_blocks.emplace_back(Block(pick, m_strips[pick].top(), sf::Color::Green));
					m_blocks.back().speed(m_strips[pick].speed());

					if(m_current_state.effect == BlackOut || m_current_state.effect == HueShift || m_current_state.effect == Shadow)
						m_blocks.back().preview(false);
				}

				m_gen_count = sf::Time::Zero;
			}
			else
			{
				m_gen_count += m_delta;
			}

			for(auto &block: m_blocks)
			{
				block.speed(m_strips[block.column()].speed());
				block.update(m_delta);
			}

			for(auto &strip : m_strips)
				strip.update(m_delta);

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_player.bounding_box().left > 0)
				m_player.move(Player::Left, sf::Keyboard::isKeyPressed(sf::Keyboard::Space), m_delta);
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_player.bounding_box().left < 472)
				m_player.move(Player::Right, sf::Keyboard::isKeyPressed(sf::Keyboard::Space), m_delta);

			bool beam = false;

			for(auto &block: m_blocks)
			{
				if(block.bounding_box().intersects(m_player.bounding_box()))
				{
					//std::cout << "DEAD" << std::endl;
					m_current_state.status = Over;

					if(m_ply_hit.getStatus() == sf::Sound::Stopped)
						m_ply_hit.play();
				}

				if(m_current_state.effect != Shadow	&& m_current_state.effect != BlackOut && m_current_state.effect != HueShift
					&& block.beam_bounding_box().intersects(m_player.bounding_box()))
				{
					if(m_ply_beam.getStatus() == sf::Sound::Stopped)
						m_ply_beam.play();
					beam = true;
				}
			}

			if(beam)
				m_current_state.beam = true;
			else
				m_current_state.beam = false;

			if(m_current_state.beam)
				m_current_state.points += 3*(m_delta.asMilliseconds()/10);
			else
				m_current_state.points += m_delta.asMilliseconds()/10;

			std::stringstream ss;
			ss << m_current_state.points;
			m_points.setString(ss.str());
			//std::cout << m_current_state.points << std::endl;

			// Difficulty
			if(m_current_state.points < 5000)
				m_speed = {250, 300};
			else if(m_current_state.points < 10000)
				m_speed = {300, 350};
			else if(m_current_state.points < 30000)
				m_speed = {350, 500};
			else if(m_current_state.points < 50000)
				m_speed = {500, 1000};
			else if(m_current_state.points < 1000000)
				m_speed = {1000, 1200};

			if(m_current_state.points < 10000)
				m_gen_cooldown = sf::seconds(1.0f);
			else if(m_current_state.points < 20000)
				m_gen_cooldown = sf::seconds(0.8f);
			else if(m_current_state.points < 25000)
				m_gen_cooldown = sf::seconds(0.7f);
			else if(m_current_state.points < 35000)
				m_gen_cooldown = sf::seconds(0.5f);
			else if(m_current_state.points < 50000)
				m_gen_cooldown = sf::seconds(0.2f);
			else if(m_current_state.points < 1000000)
				m_gen_cooldown = sf::seconds(0.1f);
		}
		break;
		default:
		break;
	}
	m_delta = m_clock.restart();
}

void PlayState::render()
{
	m_window.clear(m_background);

	for(auto &strip : m_strips)
		m_window.draw(strip, &m_tex_strip);

	for(auto &block: m_blocks)
		m_window.draw(block);

	if(m_current_state.effect == Shadow)
		m_window.draw(m_shadow);

	m_window.draw(m_player);

	if(m_current_state.beam)
		m_window.draw(m_double);

	m_window.draw(m_effect_name);

	switch(m_current_state.status)
	{
		case Over:
			m_window.draw(m_go_overlay);
			m_window.draw(m_go_text);
		break;
		case Menu:
			m_window.draw(m_menu);
			m_window.draw(m_selection);
			m_window.draw(m_resume);
			m_window.draw(m_restart);
			m_window.draw(m_exit);
		break;
		default:
		break;
	}
	m_window.draw(m_points);

	m_window.display();
}
