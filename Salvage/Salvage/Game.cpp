#include "Game.h"

void Game::updateCameraBuffers()
{
	_graphicResources->getPerFrameData()->VP = _camera->getViewProjection();
	_graphicResources->getPerFrameData()->camPos = _camera->getPosition();
	_graphicResources->updatePerFrameCB();
}

Game::Game(Clock* clock, InputController* inputController, GraphicResources* graphicResources)
{
	//Graphics
	_graphicResources = graphicResources;
	//Camera
	_camera = new Camera(_graphicResources->getDevice(), WIDTH, HEIGHT);
	//Input
	_inputController = inputController;
	//Clock
	_clock = clock;

	_levelHandler = new LevelHandler(_graphicResources->getDevice(), _graphicResources->getDeviceContext());
}

Game::~Game()
{
	delete _levelHandler;
	delete _camera;
}

void Game::changeState(State * state)
{
	//Check if state stack has at least one state
	//Pause the current state
	//Add the new state
	//Example: pause game state, add menu state
	if (!_states.empty())
	{
		_states.back()->pause();
		_states.push_back(state);
	}
}

void Game::pushState(State * state)
{
	//Add a state ontop of current
	//Example: maybe you want to update game while you're in a menu
	_states.push_back(state);
}

void Game::popState()
{
	//Delete the current state
	//resume the paused state
	if (!_states.empty())
	{
		_states.pop_back();

		if(!_states.empty())
			_states.back()->resume();
	}
}

void Game::handleInput()
{
	//Handle input if not paused
	for(int i = 0; i < _states.size(); i++)
		if(!_states[i]->isPaused())
			_states[i]->handleInput(this);
}

void Game::update()
{
	//Update game logic if not paused
	for (int i = 0; i < _states.size(); i++)
		if (!_states[i]->isPaused())
			_states[i]->update(this);
}

void Game::draw()
{
	//Draw models if not paused
	for (int i = 0; i < _states.size(); i++)
		if (!_states[i]->isPaused())
			_states[i]->draw(this);
}

GraphicResources * Game::getGraphicResources()
{
	return _graphicResources;
}

Clock* Game::getClock()
{
	return _clock;
}

InputController* Game::getInputController()
{
	return _inputController;
}

LevelHandler * Game::getLevelHandler()
{
	return _levelHandler;
}

Camera * Game::getCamera()
{
	return _camera;
}
