#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <array>
#include "Neural_net.h"

typedef std::vector<float> vector_1d;
typedef std::vector<std::vector<float>> vector_2d;
typedef std::vector<std::vector<std::vector<float>>> vector_3d;





class Bird {
private:
	sf::Sprite sprite_bird;
	sf::Texture tekstura;
	sf::Clock fall_clock;
	sf::Clock update_clock;
	float velocity;
	bool dead;
	bool kontakt;
	bool spada;
public:
	Bird();
	bool boolisDead();
	bool isSpada();
	bool getkontakt();
	void draw(sf::RenderWindow& okno);
	void update(sf::Event& event);
	void deaded(float pipes_position_x, float pipes_position_y);
	void falling();
	float getPositionx();
	float getPositiony();
	float getVelocity();
	void res();
	void updateAI(float neural_out);
	void updateAIfall();
};

Bird::Bird() {
	dead = false;
	kontakt = false;
	spada = false;
	velocity = 0;
	tekstura.loadFromFile("flappy.png");
	sprite_bird.setTexture(tekstura);
	sprite_bird.setScale(3, 3);
	sprite_bird.setPosition(sf::Vector2f(100, 270));

}

bool Bird::isSpada() {
	return spada;
}

bool Bird::getkontakt() {
	return kontakt;
}

float Bird::getPositionx() {
	return sprite_bird.getPosition().x;
}

float Bird::getPositiony() {
	return sprite_bird.getPosition().y;
}

float Bird::getVelocity() {
	return velocity;
}

bool Bird::boolisDead() {
	return dead;
}

void Bird::draw(sf::RenderWindow& okno) {
	okno.draw(sprite_bird);
}

void Bird::update(sf::Event& event) {
	if (update_clock.getElapsedTime().asSeconds() > 0.2) {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::RControl) {
				velocity = -4;
				update_clock.restart();
			}
		}
		
	}


	velocity += 0.095;
	sprite_bird.setPosition(sf::Vector2f(sprite_bird.getPosition().x, sprite_bird.getPosition().y + velocity));


}

void Bird::updateAI(float neural_out) {
	if (update_clock.getElapsedTime().asSeconds() > 0.2) {
		if (neural_out > 0.5) {
			velocity = -4;
			update_clock.restart();
		}

	}


	velocity += 0.095;
	sprite_bird.setPosition(sf::Vector2f(sprite_bird.getPosition().x, sprite_bird.getPosition().y + velocity));
}

void Bird::updateAIfall() {
	velocity += 0.095;
	sprite_bird.setPosition(sf::Vector2f(sprite_bird.getPosition().x, sprite_bird.getPosition().y + velocity));
}

void Bird::deaded(float pipes_position_x,float pipes_position_y) {
	if (sprite_bird.getPosition().y < 566 && sprite_bird.getPosition().y > 0) {
		
		if (kontakt == false) {
			
			if (sprite_bird.getPosition().x+40 - pipes_position_x > 0 && sprite_bird.getPosition().x+40 - pipes_position_x < 50) {
				if (sprite_bird.getPosition().y - pipes_position_y < 0 || pipes_position_y + 150 - sprite_bird.getPosition().y < 0) {
					
					kontakt = true;
					dead = true;
					spada = true;
					fall_clock.restart();
				}
			}
			else if (sprite_bird.getPosition().y - pipes_position_y < 0 || pipes_position_y + 150 - sprite_bird.getPosition().y < 0) {
				if (sprite_bird.getPosition().x+40 - pipes_position_x > 0) {
					kontakt = true;
					dead = true;
					spada = true;
					fall_clock.restart();
					
				}
			}

			
		}


		

	}
	else if (sprite_bird.getPosition().y<=0) {
		dead = true;
		spada = true;
		fall_clock.restart();
	}

	else {
		dead = true;
		sprite_bird.setPosition(sf::Vector2f(100, 566));
	}
}

void Bird::falling() {
	if (spada == true) {
		if (fall_clock.getElapsedTime().asSeconds() > 0.009) {

			velocity += 0.095;
			sprite_bird.setPosition(sf::Vector2f(sprite_bird.getPosition().x, sprite_bird.getPosition().y + velocity));
			if (sprite_bird.getPosition().y > 566) {
				sprite_bird.setPosition(sf::Vector2f(100, 566));
				spada = false;
			}
			fall_clock.restart();
		}
	}
}

void Bird::res() {
	dead = false;
	kontakt = false;
	spada = false;
	sprite_bird.setPosition(sf::Vector2f(100, 150));
	velocity = 0;
}

class Pipes {
private:
	sf::Sprite sprite_pipes_up;
	sf::Sprite sprite_pipes_down;
	sf::Clock zegar_move;
	float speed;
	float v_speed;
public:
	Pipes();
	float getPositionx();
	float getPositiony();
	void draw(sf::RenderWindow&okno);
	void update();
};

Pipes::Pipes() {
	int los = rand() % 2;
	if (los == 0) {
		v_speed = 0.5;
	}
	else {
		v_speed = -0.5;
	}
	sf::Texture* tekstura = new sf::Texture;
	tekstura->loadFromFile("pipes.png");
	sprite_pipes_up.setTexture(*tekstura);
	sprite_pipes_down.setTexture(*tekstura);
	sprite_pipes_up.setTextureRect(sf::IntRect(32, 0, 16, 48));
	sprite_pipes_down.setTextureRect(sf::IntRect(0, 0, 16, 48));
	
	int y = rand()%301;
	//std::cout << y;
	sprite_pipes_down.setPosition(sf::Vector2f(450, 230 + y));
	sprite_pipes_up.setPosition(sf::Vector2f(450, -300 + y));

	sprite_pipes_down.setScale(sf::Vector2f(3.5,8));
	sprite_pipes_up.setScale(sf::Vector2f(3.5,8));

	speed = 1.2;
}

float Pipes::getPositionx() {
	return sprite_pipes_down.getPosition().x;
}

float Pipes::getPositiony() {
	return sprite_pipes_up.getPosition().y+383;
}

void Pipes::draw(sf::RenderWindow& okno) {
	okno.draw(sprite_pipes_down);
	okno.draw(sprite_pipes_up);
}

void Pipes::update() {
	sprite_pipes_down.setPosition(sf::Vector2f(sprite_pipes_down.getPosition().x - speed, sprite_pipes_down.getPosition().y+v_speed));
	sprite_pipes_up.setPosition(sf::Vector2f(sprite_pipes_up.getPosition().x - speed, sprite_pipes_up.getPosition().y+v_speed));
	if (sprite_pipes_up.getPosition().y >= 0) {
		v_speed = -v_speed;
	}
	else if (sprite_pipes_up.getPosition().y <= -300) {
		v_speed = -v_speed;
	}
}

class Pipes_Manager {
private:
	std::vector<Pipes>rury;

public:
	Pipes_Manager();
	void draw(sf::RenderWindow&okno);
	void update();
	void add_new();
	void res();
	float pos_x(float posx_bird);
	float pos_y(float posx_bird);
};

Pipes_Manager::Pipes_Manager() {
	Pipes rura;
	rury.emplace(rury.begin(),rura );
}

void Pipes_Manager::draw(sf::RenderWindow& okno) {
	for (int i = 0; i < rury.size(); i++) {
		rury[i].draw(okno);
	}
}

void Pipes_Manager::update() {
	for (int i = 0; i < rury.size(); i++) {
		rury[i].update();
	}
}

void Pipes_Manager::add_new() {
	
	if (rury[rury.size() - 1].getPositionx() < 150) {
		Pipes rurka;
		rury.push_back(rurka);	
	}
	if (rury[0].getPositionx() < -80) {
		rury.erase(rury.begin(), rury.begin() + 1);
	}

}

float Pipes_Manager::pos_x(float posx_bird) {
	if (posx_bird - rury[0].getPositionx() < 50) {
		return rury[0].getPositionx();
	}
	else {
		return rury[1].getPositionx();
	}
}

float Pipes_Manager::pos_y(float posx_bird) {

	if (posx_bird - rury[0].getPositionx() < 50) {
		return rury[0].getPositiony();
	}
	else {
		return rury[1].getPositiony();
	}
}

void Pipes_Manager::res() {
	rury.erase(rury.begin(),rury.end());
	Pipes rura;
	rury.emplace(rury.begin(), rura);
}

class Game {
private:
	Bird player;
	Pipes_Manager pipes;
	Neural_net* neural_network;
	vector_2d inputs;
	vector_2d outputs;
	vector_2d expected_outputs;
	sf::Clock zegar;
	sf::Clock points_clock;
	sf::Clock zegarAI;
	sf::Clock fly_clock;
	sf::Clock pos_check;
	sf::Sprite tlo;
	sf::Texture teksturatlo;
	sf::RenderWindow *window;
	sf::Text points_text;
	sf::Text game_over;
	sf::Font fontt;
	bool start;
	bool ifreset;
	bool flylearn;
	bool learn;
	int points;
	int counter;
	int pointscheck;
	float record;
	float act_rec;
public:
	Game(sf::RenderWindow& okno, Neural_net& neural_net);
	void play(sf::Event &event);
	void point_counter(float pipes_x, float player_x);
	void reset();
	void backpropag();
};

Game::Game(sf::RenderWindow& okno, Neural_net&neural_net) {
	window = new sf::RenderWindow;
	window = &okno;
	teksturatlo.loadFromFile("tlo.png");
	tlo.setTexture(teksturatlo);
	tlo.setScale(4, 4);
	points = 0;
	pointscheck = 0;
	counter = 0;
	act_rec = 0;
	record = 0;
	start = false;
	ifreset = false;
	flylearn = true;
	learn = true;
	fontt.loadFromFile("Arialn.ttf");
	
	points_text.setFont(fontt);
	points_text.setFillColor(sf::Color::Black);
	points_text.setOutlineColor(sf::Color::White);
	points_text.setOutlineThickness(1);
	points_text.setPosition(sf::Vector2f(550, 40));
	points_text.setCharacterSize(50);
	points_text.setString("Punkty : " + std::to_string(points));

	game_over.setFont(fontt);
	game_over.setFillColor(sf::Color::Red);
	game_over.setOutlineColor(sf::Color::White);
	game_over.setOutlineThickness(2);
	game_over.setPosition(sf::Vector2f(150, 200));
	game_over.setCharacterSize(80);
	game_over.setString("GAME OVER");
	
	neural_network = &neural_net;
	inputs.resize(10);
	outputs.resize(10);
	expected_outputs.resize(10);
	for (int i = 0; i < 10; i++) {
		inputs[i].resize(4);
		outputs[i].resize(1);
		expected_outputs[i].resize(1);
	}
	
}

void Game::play(sf::Event &event) {
	window->draw(tlo);

	
	pipes.draw(*window);
	player.draw(*window);
	window->draw(points_text);
	if (player.boolisDead() == true) {
		window->draw(game_over);
	}
	if (ifreset == true) {
		reset();
	}

	

	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::RControl) {
			start = true;
		}
		else if (event.key.code == sf::Keyboard::LControl) {
			ifreset = true;
		}
	}


	if (start == true) {

		pipes.add_new();
		player.falling();

		if (player.boolisDead() == false) {
			player.deaded(pipes.pos_x(player.getPositionx()), pipes.pos_y(player.getPositionx()));

			if (player.boolisDead() == true) {
				if (points == 0) {
					record = pipes.pos_x(player.getPositionx()) - player.getPositionx();
					
				}
			}
			if (learn == true) {
				if (points > 20) {
					learn = false;
				}
				else if (points > 2) {
					flylearn = false;
				}
			}
		}
		point_counter(pipes.pos_x(player.getPositionx()), player.getPositionx());

		if (player.boolisDead() == false) {
			if (zegar.getElapsedTime().asSeconds() > 0.009) {
				//player.update(event);
				player.updateAIfall();
				pipes.update();
				zegar.restart();
			}
			if (zegarAI.getElapsedTime().asSeconds() > 0.009) {
				inputs[counter][0] = player.getVelocity();
				inputs[counter][1] = pipes.pos_y(player.getPositionx())- player.getPositiony();
				inputs[counter][2] = pipes.pos_y(player.getPositionx())+150- player.getPositiony();
				inputs[counter][3] = player.getPositionx() - pipes.pos_x(player.getPositionx());
				//inputs[counter][4] = player.getVelocity();
				//std::cout << "PLAYER Y - " << player.getPositiony() << " pipes gora " << pipes.pos_y(player.getPositionx()) << " pipes dol " << pipes.pos_y(player.getPositionx()) + 150 << " pipes x " << pipes.pos_x(player.getPositionx()) << "\n";
				neural_network->set_inputs(inputs[counter]);
				outputs[counter] = neural_network->forward_propagation_out();
				player.updateAI(outputs[counter][0]);
				counter++;
				if (counter == 10) {
					counter = 0;
				}

				zegarAI.restart();
			}
		}

		backpropag();


	}
}

void Game::backpropag() {
	if (learn == true) {

		if (flylearn == true) {

			if (points == 0) {
				if (record > act_rec) {
					for (int i = 0; i < 10; i++) {
						if (outputs[i][0] > 0.5) {
							expected_outputs[i][0] = 0.75;

						}
						else {
							expected_outputs[i][0] = 0.25;
						}
						neural_network->set_expoutputs(expected_outputs[i]);

						neural_network->back_propagation();
					}


					act_rec = record;
				}
			}

			if (fly_clock.getElapsedTime().asSeconds() > 1) {
				for (int i = 0; i < 10; i++) {
					if (outputs[i][0] > 0.5) {
						expected_outputs[i][0] = 0.8;

					}
					else {
						expected_outputs[i][0] = 0.2;
					}
					neural_network->set_expoutputs(expected_outputs[i]);

					neural_network->back_propagation();
				}
				fly_clock.restart();
			}
		}
		/*if (player.getkontakt() == true) {
			if (((player.getPositiony() - pipes.pos_y(player.getPositionx()) < 40) && (player.getPositiony() - pipes.pos_y(player.getPositionx()) > -80)) || ((player.getPositiony() - pipes.pos_y(player.getPositionx()) + 150 < 40) && (player.getPositiony() - pipes.pos_y(player.getPositionx()) + 150 > -80))) {
				for (int i = 0; i < 10; i++) {
					if (outputs[i][0] > 0.5) {
						expected_outputs[i][0] = 0.8;

					}
					else {
						expected_outputs[i][0] = 0.2;
					}
					neural_network->set_expoutputs(expected_outputs[i]);

					neural_network->back_propagation();
				}
			}
			else {
				for (int i = 0; i < 10; i++) {
					if (outputs[i][0] > 0.5) {
						expected_outputs[i][0] = 0.4;

					}
					else {
						expected_outputs[i][0] = 0.6;
					}
					neural_network->set_expoutputs(expected_outputs[i]);

					neural_network->back_propagation();
				}
			}
		}*/
		/*if(pos_check.getElapsedTime().asSeconds()>0.3){
			if (player.getPositionx() - pipes.pos_x(player.getPositionx() < 55&& player.getPositionx() - pipes.pos_x(player.getPositionx()>37))) {
				for (int i = 0; i < 10; i++) {
					if (outputs[i][0] > 0.5) {
						expected_outputs[i][0] = 0.8;

					}
					else {
						expected_outputs[i][0] = 0.2;
					}
					neural_network->set_expoutputs(expected_outputs[i]);

					neural_network->back_propagation();
				}
				
			}
			pos_check.restart();
		}*/



		if (points != pointscheck) {
			for (int i = 0; i < 10; i++) {
				if (outputs[i][0] > 0.5) {
					expected_outputs[i][0] = 0.8;

				}
				else {
					expected_outputs[i][0] = 0.2;
				}
				neural_network->set_expoutputs(expected_outputs[i]);

				neural_network->back_propagation();
			}
			pointscheck = points;

		}
		else if (player.boolisDead() == true) {
			for (int i = 0; i < 10; i++) {
				if (outputs[i][0] > 0.5) {
					expected_outputs[i][0] = 0;

				}
				else {
					expected_outputs[i][0] = 1;
				}
				neural_network->set_expoutputs(expected_outputs[i]);

				neural_network->back_propagation();
			}
			fly_clock.restart();
			ifreset = true;
			act_rec = 0;
			record -= 20;

		}
	}
}

void Game::point_counter(float pipes_x, float player_x) {

	if (player.boolisDead() == false) {
		if (player_x-40 - pipes_x  > 1) {
			if (points_clock.getElapsedTime().asSeconds() > 1) {
				points++;
				points_text.setString("Punkty : " + std::to_string(points));
				points_clock.restart();
				
			}
		}
	}
}

void Game::reset() {
	start = false;
	player.res();
	pipes.res();
	points = 0;
	points_text.setString("Punkty : " + std::to_string(points));
	ifreset = false;
}




int main() {

	srand(time(NULL));

	//INIT
	int INPUT_NEURONS = 4;
	int HIDDEN_NEURONS = 10;
	int OUTPUT_NEURONS = 1;
	int HIDDEN_LAYERS = 2;


	vector_1d expected_outputs;
	vector_1d outputs;
	vector_1d inputs;


	Neural_net neural_network(INPUT_NEURONS, HIDDEN_NEURONS, OUTPUT_NEURONS, HIDDEN_LAYERS);
	inputs.resize(INPUT_NEURONS);
	outputs.resize(OUTPUT_NEURONS);
	expected_outputs.resize(OUTPUT_NEURONS);
	

	sf::RenderWindow okno(sf::VideoMode(800, 600), "Flap Flap");
	
	
	Game gra(okno,neural_network);

	while (okno.isOpen()) {

		sf::Event event;
		while (okno.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				okno.close();

			}
		}
		okno.clear();

		
		gra.play(event);

		okno.display();

	}

}
