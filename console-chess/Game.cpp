//---------------------------------------------------------------
//
// Game.cpp
//

#include "Game.h"

#include <algorithm>
#include <limits>
#include <unordered_map>

namespace Chess {

//===============================================================================

static const glm::ivec2 s_up        = { -1,  0 };
static const glm::ivec2 s_left      = {  0, -1 };
static const glm::ivec2 s_down      = {  1,  0 };
static const glm::ivec2 s_right     = {  0,  1 };
static const glm::ivec2 s_upLeft    = { -1, -1 };
static const glm::ivec2 s_upRight   = { -1,  1 };
static const glm::ivec2 s_downLeft  = {  1, -1 };
static const glm::ivec2 s_downRight = {  1,  1 };

static const std::array<glm::ivec2, 1> s_whitePawnDirections{{ {s_up} }};
static const std::array<glm::ivec2, 1> s_blackPawnDirections{{ {s_down} }};
static const std::array<glm::ivec2, 8> s_queenDirections{{ {s_up}, {s_down}, {s_left}, {s_right}, {s_upLeft}, {s_downLeft}, {s_upRight}, {s_downRight} }};
static const std::array<glm::ivec2, 8> s_kingDirections{{ {s_up}, {s_down}, {s_left}, {s_right}, {s_upLeft}, {s_downLeft}, {s_upRight}, {s_downRight} }};
static const std::array<glm::ivec2, 4> s_rookDirections{{ {s_up}, {s_down}, {s_left}, {s_right} }};
static const std::array<glm::ivec2, 4> s_bishopDirections{{ {s_upLeft}, {s_upRight}, {s_downLeft}, {s_downRight} }};
static const std::array<glm::ivec2, 4> s_knightDirections{{ {-2, -1}, {-2, 1}, {2, -1}, {2, 1} }};

static const PieceInfo s_wP {
	PieceId::PAWN, ColorId::WHITE, 1, 1,
	{ std::cbegin(s_whitePawnDirections), std::cend(s_whitePawnDirections)}
};
static const PieceInfo s_bP {
	PieceId::PAWN, ColorId::BLACK, 1, 1,
	{ std::cend(s_blackPawnDirections), std::cend(s_blackPawnDirections) }
};
static const PieceInfo s_wN {
	PieceId::KNIGHT, ColorId::WHITE, 3, 1,
	{ std::cbegin(s_knightDirections), std::cend(s_knightDirections) }
};
static const PieceInfo s_bN {
	PieceId::KNIGHT, ColorId::BLACK, 3, 1,
	{ std::cbegin(s_knightDirections), std::cend(s_knightDirections) }
};
static const PieceInfo s_wR {
	PieceId::ROOK, ColorId::WHITE, 5, 8,
	{ std::cbegin(s_rookDirections), std::end(s_rookDirections) }
};
static const PieceInfo s_bR {
	PieceId::ROOK, ColorId::BLACK, 5, 8,
	{ std::cbegin(s_rookDirections), std::cend(s_rookDirections) }
};
static const PieceInfo s_wB {
	PieceId::BISHOP, ColorId::WHITE, 3, 8,
	{ std::cbegin(s_bishopDirections), std::cend(s_bishopDirections) }
};
static const PieceInfo s_bB {
	PieceId::BISHOP, ColorId::BLACK, 3, 8,
	{ std::cbegin(s_bishopDirections), std::cend(s_bishopDirections) }
};
static const PieceInfo s_wQ {
	PieceId::QUEEN,  ColorId::WHITE, 9, 8,
	{ std::cbegin(s_queenDirections), std::cend(s_queenDirections) }
};
static const PieceInfo s_bQ {
	PieceId::QUEEN,  ColorId::BLACK, 9, 8,
	{ std::cbegin(s_queenDirections), std::cend(s_queenDirections) }
};
static const PieceInfo s_wK { 
	PieceId::KING, ColorId::WHITE, std::numeric_limits<int32_t>::max(), 1,
	{ std::cbegin(s_kingDirections), std::cend(s_kingDirections) }
};
static const PieceInfo s_bK {
	PieceId::KING, ColorId::BLACK, std::numeric_limits<int32_t>::max(), 1,
	{ std::cbegin(s_kingDirections), std::cend(s_kingDirections) }
};
static const PieceInfo s_empty{ PieceId::EMPTY,  ColorId::NONE,  0, 0, {} };

static void ResetBoard(ChessBoard& board)
{
	board = {{
	//  rook     knight   bishop   queen    king     bishop   knight   rook
		s_bR,    s_bN,    s_bB,    s_bQ,    s_bK,    s_bB,    s_bN,    s_bR,
		s_bP,    s_bP,    s_bP,    s_bP,    s_bP,    s_bP,    s_bP,    s_bP,
		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
		s_wP,    s_wP,    s_wP,    s_wP,    s_wP,    s_wP,    s_wP,    s_wP,
	//  rook     knight   bishop   queen    king     bishop   knight   rook.
		s_wR,    s_wN,    s_wB,    s_wQ,    s_wK,    s_wB,    s_wN,    s_wR
	}};
}

static void SetTestBoard(ChessBoard& board)
{
	board = { {
	//  a        b        c        d        e        f        g        h
		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_bK,    // 8
		s_empty, s_empty, s_empty, s_empty, s_empty, s_wQ, s_empty, s_empty,    // 7
		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, // 6
		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, // 5
		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, // 4
		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, // 3
		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, // 2
		s_wR, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,    // 1
	} };
}
/*
	// Board mapping reference
		a	b	c	d	e	f	g	h
		0	1	2	3	4	5	6	7
	8   0
	7   1
	6   2
	5   3
	4   4
	3   5
	2   6
	1   7
*/
static std::unordered_map<std::string, glm::ivec2> s_coordMap = {
	{"a8", {0, 0}}, {"a7", {1, 0}}, {"a6", {2, 0}}, {"a5", {3, 0}}, {"a4", {4, 0}}, {"a3", {5, 0}}, {"a2", {6, 0}}, {"a1", {7, 0}},
	{"b8", {0, 1}}, {"b7", {1, 1}}, {"b6", {2, 1}}, {"b5", {3, 1}}, {"b4", {4, 1}}, {"b3", {5, 1}}, {"b2", {6, 1}}, {"b1", {7, 1}},
	{"c8", {0, 2}}, {"c7", {1, 2}}, {"c6", {2, 2}}, {"c5", {3, 2}}, {"c4", {4, 2}}, {"c3", {5, 2}}, {"c2", {6, 2}}, {"c1", {7, 2}},
	{"d8", {0, 3}}, {"d7", {1, 3}}, {"d6", {2, 3}}, {"d5", {3, 3}}, {"d4", {4, 3}}, {"d3", {5, 3}}, {"d2", {6, 3}}, {"d1", {7, 3}},
	{"e8", {0, 4}}, {"e7", {1, 4}}, {"e6", {2, 4}}, {"e5", {3, 4}}, {"e4", {4, 4}}, {"e3", {5, 4}}, {"e2", {6, 4}}, {"e1", {7, 4}},
	{"f8", {0, 5}}, {"f7", {1, 5}}, {"f6", {2, 5}}, {"f5", {3, 5}}, {"f4", {4, 5}}, {"f3", {5, 5}}, {"f2", {6, 5}}, {"f1", {7, 5}},
	{"g8", {0, 6}}, {"g7", {1, 6}}, {"g6", {2, 6}}, {"g5", {3, 6}}, {"g4", {4, 6}}, {"g3", {5, 6}}, {"g2", {6, 6}}, {"g1", {7, 6}},
	{"h8", {0, 7}}, {"h7", {1, 7}}, {"h6", {2, 7}}, {"h5", {3, 7}}, {"h4", {4, 7}}, {"h3", {5, 7}}, {"h2", {6, 7}}, {"h1", {7, 7}},
};

Game::Game()
	: m_blackPlayer(ColorId::BLACK)
	, m_whitePlayer(ColorId::WHITE)
	, m_currentPlayer(&m_whitePlayer)
{
	SetupBoard();
}

bool Game::MovePiece(const std::string& source, const std::string& dest)
{
	// The game is over.
	if (m_gameResolutionState != GameResolutionId::NONE)
	{
		return true;
	}

	glm::ivec2 sourceVec = s_coordMap[source];
	glm::ivec2 destVec = s_coordMap[dest];

	// Not allowed to take own piece.
	if (GetPieceAt(m_board, destVec).colorId == m_currentPlayer->GetColor())
	{
		return false;
	}
	const PieceInfo& piece = GetPieceAt(m_board, sourceVec);

	using DestList = std::vector<glm::ivec2>;
	const DestList& validDestList = GetValidDestList(m_board, sourceVec, piece);

	auto it = std::find(std::cbegin(validDestList), std::cend(validDestList), destVec);
	if (it != std::cend(validDestList))
	{
		// To check if this move would cause us to move into check, we mock the next turn.
		ChessBoard tempBoard = m_board;

		DoMovePiece(tempBoard, sourceVec, destVec, piece);

		const DestList& piecesThatCheck = SearchForCheck(tempBoard);
		if (piecesThatCheck.empty())
		{
			DoMovePiece(m_board, sourceVec, destVec, piece);
			return true;
		}
	}

	return false;
}

void Game::TogglePlayer()
{
	m_currentPlayer = m_currentPlayer->GetColor() == ColorId::BLACK ?
		&m_whitePlayer : &m_blackPlayer;

	const auto& moveList = SearchForCheck(m_board);
	m_currentPlayer->SetIsInCheck(!moveList.empty());

	bool hasValidMove = CurrentPlayerHasValidMove();
	if (m_currentPlayer->IsInCheck())
	{
		// Player was in check and has nowhere to go, checkmate.
		if (!hasValidMove)
		{
			m_gameResolutionState = GameResolutionId::CHECKMATE;
		}
	}
	else if (!hasValidMove)
	{
		m_gameResolutionState = GameResolutionId::BASIC_STALEMATE;
	}
}

Chess::GameResolutionId Game::GetResolutionState()
{
	return m_gameResolutionState;
}

const Chess::PieceInfo& Game::GetPieceAt(const ChessBoard& board, const glm::ivec2& source)
{
	if (!IsCoordInBounds(board, source))
		return s_empty;

	return board[source.x][source.y];
}

void Game::SetPieceAt(ChessBoard& board, const glm::ivec2& dest, const PieceInfo& piece)
{
	if (!IsCoordInBounds(board, dest))
		return;

	board[dest.x][dest.y] = piece;
}

std::vector<glm::ivec2> Game::GetValidDestList(const ChessBoard& board, const glm::ivec2& source, const PieceInfo& piece)
{
	std::vector<glm::ivec2> validDestList;

	// Check each direction for the piece and populate a list of its available moves.
	for (const auto& direction : piece.directions)
	{
		// Establish line of sight for direction.
		for (int numSteps = 1; numSteps <= piece.maxNumSteps; ++numSteps)
		{
			glm::ivec2 dest = source + direction * numSteps;

			// If we go off the board, we're done with this direction.
			if (!IsCoordInBounds(board, dest))
			{
				break;
			}
			
			const auto& destPiece = GetPieceAt(board, dest);

			// Any empty space is valid.
			if (destPiece.pieceId == PieceId::EMPTY)
			{
				validDestList.push_back(dest);
			}

			// A friendly piece will break LoS and is not valid.
			else if (GetAffinity(piece, destPiece) == AffinityId::FRIENDLY)
			{
				break;
			}

			// An enemy piece breaks LoS, but is still valid.
			else if (GetAffinity(piece, destPiece) == AffinityId::ENEMY)
			{
				validDestList.push_back(dest);
				break;
			}
		}
	}

	// We get a chance down here to append any other special case moves to the list.
	if (piece.pieceId == PieceId::PAWN)
	{
		HandlePawnSpecialCase(board, validDestList, source, piece.colorId);
	}

	return validDestList;
}

void Game::HandlePawnSpecialCase(const ChessBoard& board, std::vector<glm::ivec2>& destList,
	const glm::ivec2& source, ColorId colorId)
{
	glm::ivec2 direction = colorId == ColorId::BLACK ? s_down : s_up;

	// Pawns in a specific row and color may move two spaces forward.
	if ((source.x == 1 && colorId == ColorId::BLACK)
		|| (source.x == 6 && colorId == ColorId::WHITE))
	{
		glm::ivec2 dest{};
		bool canMoveTwoSpaces = true;
		for (int i = 1; i <= 2; ++i)
		{
			 dest = source + direction * i;
			 const auto& piece = GetPieceAt(board, dest);

			 // Any occupied piece along the way will make it impossible to move 2 spaces.
			if (!IsCoordInBounds(board, dest) || piece.pieceId != PieceId::EMPTY)
			{
				canMoveTwoSpaces = false;
				break;
			}
		}
		if (canMoveTwoSpaces)
		{
			destList.push_back(dest);
		}
	}

	auto handleDiagMove = [this, &source, &destList, &board](glm::ivec2& diag, ColorId colorId)
	{
		glm::ivec2 dest = source + diag;
		const auto& piece = GetPieceAt(board, dest);
		if (IsCoordInBounds(board, dest) && piece.pieceId != PieceId::EMPTY
			&& piece.colorId != m_currentPlayer->GetColor())
		{
			destList.push_back(dest);
		}
	};

	// Pawns may also move diagonally if there is an enemy piece there.
	glm::ivec2 diagLeft = colorId == ColorId::BLACK ? s_downLeft : s_upLeft;
	handleDiagMove(diagLeft, colorId);

	glm::ivec2 diagRight = colorId == ColorId::BLACK ? s_downRight : s_upRight;
	handleDiagMove(diagRight, colorId);
}

void Game::HandleCastleSpecialCase(const ChessBoard& board, std::vector<glm::ivec2>& destList,
	const glm::ivec2& source, ColorId id)
{
	// TODO: requires state tracking. Rules for check:
	// 1) King has not moved. I'll need to track if king has moved or not.
	// 2) King must not pass through check. Will need to check for this for each step.
	// 3) King must not break los with rook. (no pieces between).
	// 4) King must not currently be in check.

	// Potentially create a Piece class for tracking state. Piece will have a PieceInfo struct in addition
	// to state.

	// Alternatively, if this is the only case where I need state tracking, just track if a king has moved
	// in Player class.
}

void Game::DoMovePiece(ChessBoard& board, const glm::ivec2& source, const glm::ivec2& dest, const PieceInfo& piece)
{
	const PieceInfo& destPiece = GetPieceAt(board, dest);
	if (destPiece.pieceId != PieceId::EMPTY)
	{
		m_currentPlayer->OnPieceCaptured(&destPiece);
	}

	SetPieceAt(board, dest, piece);
	SetPieceAt(board, source, s_empty);
}

std::vector<glm::ivec2> Game::SearchForCheck(const ChessBoard& board)
{
	// Find King position
	glm::ivec2 kingPos = GetKingPos(board);

	// pos, isLoSing king.
	using Candidate = std::pair<glm::ivec2, bool>;
	using CandidateList = std::vector<Candidate>;
	CandidateList candidates;
	int32_t boardSize = static_cast<int32_t>(board.size());

	// All enemies are potential candidates. Get them first.
	for (int row = 0; row < boardSize; ++row )
	{
		for (int col = 0; col < boardSize; ++col)
		{
			glm::ivec2 pos{row, col};

			if (GetAffinity(GetPieceAt(board, kingPos), GetPieceAt(board, pos)) == AffinityId::ENEMY)
			{
				candidates.push_back({pos, false});
			}
		}
	}

	// Now we have a list of all the enemy pieces. Let's see if they are LoSing the King.
	for (Candidate& candidate : candidates)
	{
		const glm::ivec2& candidatePos = candidate.first;
		bool& isCandidateValid = candidate.second;
		const PieceInfo& piece = GetPieceAt(board, candidatePos);

		const std::vector<glm::ivec2>& destList = GetValidDestList(board, candidatePos, piece);

		auto found = std::find(std::cbegin(destList), std::cend(destList), kingPos);
		if (found != std::cend(destList))
		{
			isCandidateValid = true;
		}
	}
	
	std::vector<glm::ivec2> piecePositions;
	for (const Candidate& candidate : candidates)
	{
		const glm::ivec2& candidatePosition = candidate.first;
		if (candidate.second)
		{
			piecePositions.push_back(candidatePosition);
		}
	}

	return piecePositions;
}

bool Game::CurrentPlayerHasValidMove()
{
	// Find all the pieces we could potentially move.
	std::vector<glm::ivec2> candidates;
	int32_t boardSize = static_cast<int32_t>(m_board.size());
	for (int row = 0; row < boardSize; ++row)
	{
		for (int col = 0; col < boardSize; ++col)
		{
			glm::ivec2 pos{ row, col };
			if (GetAffinity(GetPieceAt(m_board, pos)) == AffinityId::FRIENDLY)
			{
				candidates.push_back(pos);
			}
		}
	}

	// Now we have a list of all the friendly pieces. Let's see if they have any moves.
	for (auto& candidate : candidates)
	{
		const auto& candidatePos = candidate;
		const auto& piece = GetPieceAt(m_board, candidatePos);

		const auto& destList = GetValidDestList(m_board, candidatePos, piece);

		// See if any of these moves would result in a Check state
		for (const auto& dest : destList)
		{
			ChessBoard tempBoard = m_board;
			DoMovePiece(tempBoard, candidatePos, dest, piece);
			const auto& checkList = SearchForCheck(tempBoard);
			if (checkList.empty())
			{
				return true;
			}
		}
	}

	return false;
}

glm::ivec2 Game::GetKingPos(const ChessBoard& board)
{
	int32_t boardSize = static_cast<int32_t>(board.size());
	glm::ivec2 pos{};
	for (int row = 0; row < boardSize; ++row)
	{
		const auto& col = board[row];
		auto it = std::find_if(std::cbegin(col), std::cend(col),
			[this](const auto& piece)
		{
			return piece.pieceId == PieceId::KING && GetAffinity(piece) == AffinityId::FRIENDLY;
		});

		if (it != std::cend(col))
		{
			pos = { row, std::distance(std::cbegin(col), it) };
			break;
		}
	}

	return pos;
}

void Game::SetupBoard()
{
	//ResetBoard(m_board);
	SetTestBoard(m_board);
}

Chess::AffinityId Game::GetAffinity(const PieceInfo& attacker, const PieceInfo& defender)
{
	return DoGetAffinity(attacker.colorId, defender.colorId);
}

Chess::AffinityId Game::GetAffinity(const PieceInfo& piece)
{
	return DoGetAffinity(m_currentPlayer->GetColor(), piece.colorId);
}

Chess::AffinityId Game::DoGetAffinity(const ColorId& lhs, const ColorId& rhs)
{
	if (lhs != rhs)
		return AffinityId::ENEMY;

	if (lhs== rhs)
		return AffinityId::FRIENDLY;

	return AffinityId::NONE;
}

bool Game::IsCoordInBounds(const ChessBoard& board, const glm::ivec2& coord)
{
	return coord.x < static_cast<int32_t>(board.size()) && coord.x >= 0
		&& coord.y < static_cast<int32_t>(board.size()) && coord.y >= 0;
}

//===============================================================================

} // namespace Chess
