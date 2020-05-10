//---------------------------------------------------------------
//
// GameView.h
//

#pragma once

#include <string>

namespace Chess {

//===============================================================================
class Game;
class GameView
{
public:
	GameView(Game* game);
	void DisplayBoard();
	void OnTurnChange();
	void OnMoveFailed();
	void OnCheckmate();
	void OnStalemate();
	std::string GetCurrentPlayerStr();
private:
	Game* m_game;
};


//===============================================================================

} // namespace Chess
