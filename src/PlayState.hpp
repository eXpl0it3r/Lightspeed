#include "State.hpp"
#include "Strip.hpp"
#include "Player.hpp"
#include "Block.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <vector>
#include <random>

class PlayState : public State
{
public:
	PlayState(sf::RenderWindow &window);

	std::unique_ptr<State> run();

private:
	void update();
	void render();

	void speed_state_machine();
	void effect_state_machine();

private:
	enum SpeedStates
	{
		Normal = 0,
		Fast = 1,
		SpeedDiff = 2,
		SpeedUp = 3,
		SlowDown = 4,
		Drift = 5,
		Join = 6
	};

	enum EffectStates
	{
		NoEffect = 0,
		HueShift = 1,
		BlackOut = 2,
		Shadow = 3
	};

	enum Status
	{
		Game,
		Over,
		Menu,
		Restart,
		Exit
	};

	struct GameState
	{
		Status status = Game;
		Status last = Exit;
		SpeedStates speed = Normal;
		bool fast = false;
		bool diff = false;
		sf::Time speed_duration = sf::seconds(2);
		sf::Time speed_count;

		EffectStates effect = NoEffect;
		sf::Time effect_duration = sf::seconds(2);
		sf::Time effect_count;

		unsigned int points = 0;
		bool beam = false;
	};

	sf::Clock m_clock;
	sf::Time m_delta;

	std::mt19937 m_generator;
	std::pair<unsigned int, unsigned int> m_speed;
	std::pair<unsigned int, unsigned int> m_color;
	std::pair<unsigned int, unsigned int> m_state;
	std::pair<unsigned int, unsigned int> m_block;
	std::pair<unsigned int, unsigned int> m_strip;
	std::pair<float, float> m_duration;
	std::pair<float, float> m_hue;

	// Assets
	sf::Texture m_tex_shadow;
	sf::Texture m_tex_strip;
	sf::SoundBuffer m_snd_beam;
	sf::SoundBuffer m_snd_hit;
	sf::Sound m_ply_beam;
	sf::Sound m_ply_hit;
	sf::Font m_font;
	sf::Sprite m_shadow;
	sf::Text m_points;
	sf::Text m_double;
	sf::Text m_effect_name;

	GameState m_current_state;
	std::vector<Strip> m_strips;
	Player m_player;
	std::vector<float> m_hues;
	std::vector<Block> m_blocks;
	sf::Time m_gen_cooldown;
	sf::Time m_gen_count;
	float m_target_speed;
	sf::Color m_background;

	// GameOver
	sf::RectangleShape m_go_overlay;
	sf::Text m_go_text;

	// Menu
	sf::RectangleShape m_menu;
	sf::CircleShape m_selection;
	sf::Text m_resume;
	sf::Text m_restart;
	sf::Text m_exit;
	int m_menu_state;
};
