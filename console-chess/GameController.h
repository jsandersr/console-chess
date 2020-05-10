//---------------------------------------------------------------
//
// GameController.h
//

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Chess {

//===============================================================================

using MoveInput = std::pair<std::string, std::string>;

class Game;
class GameView;
class GameController {

public:
	GameController();
	~GameController();

	void Run();

private:

	// Input helpers. These could be just static functions.
	MoveInput GetMoveInput();
	std::vector<std::string> TokenizeString(const std::string& input);
	bool IsValidInput(const std::vector<std::string>& inputTokens);
	bool IsValidMoveInput(const std::string& str);


private:
	std::unique_ptr<Game> m_game;
	std::unique_ptr<GameView> m_view;
};


//===============================================================================

} // namespace Chess
