//---------------------------------------------------------------
//
// GameController.cpp
//

#include "GameController.h"
#include "Game.h"
#include "GameView.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace Chess {

//===============================================================================

	static const int s_numNumberInputs = 2;
	static const int s_numCharactersPerInput = 2;

GameController::GameController()
	: m_game(std::make_unique<Game>())
	, m_view(std::make_unique<GameView>(m_game.get()))
{
}

GameController::~GameController()
{

}

void GameController::Run()
{
	m_view->DisplayBoard();

	bool done = false;
	while (!done)
	{

		const auto& input = GetMoveInput();
		const std::string& source = input.first;
		const std::string& dest = input.second;

		bool success = m_game->MovePiece(source, dest);
		while (!success)
		{
			m_view->OnMoveFailed();
			const auto& input = GetMoveInput();
			const std::string& source = input.first;
			const std::string& dest = input.second;
			success = m_game->MovePiece(source, dest);
		}
		m_view->DisplayBoard();

		m_game->TogglePlayer();
		GameResolutionId resolutionState = m_game->GetResolutionState();

		// Continue the game until resolved.
		if (resolutionState == GameResolutionId::NONE)
		{
			m_view->OnTurnChange();
		}
		else if (resolutionState == GameResolutionId::CHECKMATE)
		{
			m_view->OnCheckmate();
			done = true;
		}
		else if (resolutionState == GameResolutionId::BASIC_STALEMATE)
		{
			m_view->OnStalemate();
			done = true;
		}

	}
}

MoveInput GameController::GetMoveInput()
{
	std::vector<std::string> inputList;
	bool isValidInput = false;
	while (!isValidInput)
	{
		std::string input;
		std::getline(std::cin, input);

		inputList = TokenizeString(input);
		isValidInput = IsValidInput(inputList);

		if (!isValidInput)
		{
			std::cout << "\n" << "Invalid input. Must in in format: [a-h][1-8]\n";
		}
	}

	return { inputList[0], inputList[1] };
}

std::vector<std::string> GameController::TokenizeString(const std::string& str)
{
	std::istringstream iss(str);
	return { std::istream_iterator<std::string>{iss},
		std::istream_iterator<std::string>() };
}

bool GameController::IsValidInput(const std::vector<std::string>& inputTokens)
{
	// Any input other than a source and a destination is invalid.
	if (inputTokens.size() != s_numNumberInputs)
	{
		return false;
	}

	return std::all_of(std::cbegin(inputTokens), std::cend(inputTokens),
		[this](const std::string& inputStr)
	{
		return IsValidMoveInput(inputStr);
	});
}

bool GameController::IsValidMoveInput(const std::string& str)
{
	if (str.size() != s_numCharactersPerInput)
	{
		return false;
	}

	// input should be [A-H][1-8]
	char letterCoord = std::tolower(str[0]);
	char numberCoord = str[1];
	if (letterCoord > 'h' || letterCoord < 'a')
	{
		return false;
	}
	if (numberCoord > '8' || numberCoord < '1')
	{
		return false;
	}

	return true;
}

//===============================================================================

} // namespace Chess
