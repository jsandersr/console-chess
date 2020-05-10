//---------------------------------------------------------------
//
// GameView.cpp
//

#include "GameView.h"
#include "Game.h"

#include <cctype>
#include <sstream>
#include <iostream>

namespace Chess {

//===============================================================================

static char GetPieceView(const PieceInfo& info)
{
	char piece = ' ';
	switch (info.pieceId)
	{
	case PieceId::PAWN:
		piece = 'p';
		break;
	case PieceId::KNIGHT:
		piece = 'n';
		break;
	case PieceId::ROOK:
		piece = 'r';
		break;
	case PieceId::BISHOP:
		piece = 'b';
		break;
	case PieceId::QUEEN:
		piece = 'q';
		break;
	case PieceId::KING:
		piece = 'k';
		break;
	case PieceId::NONE:
	[[fallthrough]];
	case PieceId::EMPTY:
	[[fallthrough]];
	default:
		piece = '.';
		break;
	}

	switch (info.colorId)
	{
	case ColorId::WHITE:
		piece = std::toupper(piece);
		break;
	case ColorId::BLACK:
	[[fallthrough]];
	case ColorId::NONE:
	[[fallthrough]];
	default:
		break;
	}

	return piece;
}


GameView::GameView(Game* game)
	: m_game(game)
{

}

void GameView::DisplayBoard()
{
	const auto& data = m_game->GetChessBoard();

	char row = '8';
	char col = 'A';

	std::stringstream ss;
	for (const auto& pieces : data)
	{
		ss << "|" << row-- << " ";
		for (const auto& piece : pieces)
		{
			ss << GetPieceView(piece);
		}
		ss << "\n";
	}

	ss << "|\n";
	ss << "|  ";
	while (col <= 'H')
	{
		ss << col++;
	}

	ss << "\n|";

	for (int i = 0; i < 24; ++i)
	{
		ss << "_";
	}
	ss << "\n";
	std::cout << ss.str();
}

void GameView::OnMoveFailed()
{
	std::cout << "Invalid move. Try a different move.";
}

void GameView::OnCheckmate()
{
	const auto& player = m_game->GetCurrentPlayer();

	std::cout << "Checkmate!! " << GetCurrentPlayerStr() + " loses!";
}

void GameView::OnStalemate()
{
	std::cout << "Stalemate.";
}

void GameView::OnTurnChange()
{
	const auto& player = m_game->GetCurrentPlayer();
	std::string playerStr = GetCurrentPlayerStr();
	std::cout << playerStr << "'s turn...\n";
	if (player->IsInCheck())
	{
		std::cout << playerStr << "is in check!\n";
	}
}

std::string GameView::GetCurrentPlayerStr()
{
	const auto& player = m_game->GetCurrentPlayer();
	switch (player->GetColor())
	{
	case ColorId::BLACK:
		return "Black player";
	case ColorId::WHITE:
		return "White player";
	case ColorId::NONE:
	[[fallthrough]];
	default:
		return std::string();
		break;
	}
}

//===============================================================================

} // namespace Chess
