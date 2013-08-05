#include "State.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <map>
#include <string>
#include <vector>

class MenuState : public State
{
public:
	MenuState(sf::RenderWindow& window);

	std::unique_ptr<State> run();

private:
	void update();
	void render();

private:
	enum Status
	{
		Menu,
		Play,
		Credits,
		Exit
	};

	sf::RenderTexture m_offscreen;

	sf::Font m_font;
	std::map<std::string, sf::Text> m_texts;
	std::map<std::string, sf::Text> m_shadows;

	sf::Sprite m_overlay;
	int m_selection;

	sf::Texture m_tex_highlight;
	sf::Sprite m_spr_highlight;

	sf::Texture m_tex_credits;
	sf::Sprite m_spr_credits;

	Status m_status;

};
