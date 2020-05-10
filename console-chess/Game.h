//---------------------------------------------------------------
//
// Game.h
//

#pragma once

#include <glm/glm.hpp>
#include <array>
#include <vector>

namespace Chess {

//===============================================================================

enum struct PieceId : uint32_t
{
	NONE = 0,
	EMPTY,
	PAWN,
	KNIGHT,
	ROOK,
	BISHOP,
	QUEEN,
	KING
};

enum struct ColorId : uint32_t {
	NONE = 0,
	WHITE,
	BLACK
};

enum struct AffinityId : uint32_t {
	NONE,
	FRIENDLY,
	ENEMY
};

enum struct GameResolutionId : uint32_t {
	NONE = 0,
	BASIC_STALEMATE,
	CHECKMATE
};

struct PieceInfo {
	// Indicates the kind of piece.
	PieceId pieceId = PieceId::NONE;

	// Indicates which color the piece is.
	ColorId colorId = ColorId::NONE;

	// Number of points that a piece is worth.
	int32_t points = 0;

	// Max number of steps that the piece is allowed to move in a turn.
	int32_t maxNumSteps = 0;

	// List of valid directions this piece is allowed to move.
	std::vector<glm::ivec2> directions;
};

class Player {
public:
	Player(ColorId colorId)
		: m_colorId(colorId)
	{
	}

	Player()
	{
		const int maxNumPieces = 16;
		m_capturedPieces.reserve(maxNumPieces);
	}

	void OnPieceCaptured(PieceInfo const* capturedPiece)
	{
		m_capturedPieces.push_back(capturedPiece);

		std::sort(std::begin(m_capturedPieces), std::end(m_capturedPieces),
			[this](const auto& lhs, const auto& rhs)
		{
			return lhs->pieceId < rhs->pieceId;
		});
	}

	const std::vector<PieceInfo const*>& GetCapturedPieces()
	{
		return m_capturedPieces;
	}

	int32_t GetScore() { return m_score; }
	ColorId GetColor() { return m_colorId; }
	bool IsInCheck() { return m_isInCheck; }
	void SetIsInCheck(bool isInCheck) { m_isInCheck = isInCheck; }

private:
	int32_t m_score = 0;
	ColorId m_colorId = ColorId::NONE;
	std::vector<PieceInfo const*> m_capturedPieces;
	bool m_isInCheck = false;
	bool m_hasMovedKing = false;
};

using ChessBoard = std::array<std::array<PieceInfo, 8>, 8>;
class Game {

public:
	Game();

	// Returns the board data.
	const ChessBoard& GetChessBoard() { return m_board; }

	// Returns the current player.
	Player* GetCurrentPlayer() { return m_currentPlayer; }

	// Begin trying to move the piece from the source to the dest.
	bool MovePiece(const std::string& source, const std::string& dest);

	// Flips the current player's turn. All functions operate in the context of the current player.
	void TogglePlayer();

	GameResolutionId GetResolutionState();

private:
	const PieceInfo& GetPieceAt(const ChessBoard& board, const glm::ivec2& source);
	void SetPieceAt(ChessBoard& board, const glm::ivec2& dest, const PieceInfo& piece);

	// Returns a list of available moves given a piece on a given board.
	std::vector<glm::ivec2> GetValidDestList(const ChessBoard& board, const glm::ivec2& source, const PieceInfo& piece);
	void HandlePawnSpecialCase(const ChessBoard& board, std::vector<glm::ivec2>& destList,
		const glm::ivec2& source, ColorId id);
	void HandleCastleSpecialCase(const ChessBoard& board, std::vector<glm::ivec2>& destList,
		const glm::ivec2& source, ColorId id);

	// Physically place the piece.
	void DoMovePiece(ChessBoard& board, const glm::ivec2& source, const glm::ivec2& dest, const PieceInfo& piece);

	// Returns a list of piece positions that cause a check.
	std::vector<glm::ivec2> SearchForCheck(const ChessBoard& data);

	bool CurrentPlayerHasValidMove();

	// Helper for getting the current king position.
	glm::ivec2 GetKingPos(const ChessBoard& board);

	// Initializes the board.
	void SetupBoard();

	// Returns the affinity between an attacking piece and a defending piece.
	AffinityId GetAffinity(const PieceInfo& attacker, const PieceInfo& defender);

	// Returns the affinity of a given piece to the current player.
	AffinityId GetAffinity(const PieceInfo& piece);

	// Returns the affinity between two given pieces.
	AffinityId DoGetAffinity(const ColorId& lhs, const ColorId& rhs);

	// Returns whether the given coordinate is on the board.
	bool IsCoordInBounds(const ChessBoard& board, const glm::ivec2& coord);

private:
	ChessBoard m_board;
	GameResolutionId m_gameResolutionState = GameResolutionId::NONE;

	Player m_whitePlayer;
	Player m_blackPlayer;
	Player* m_currentPlayer = nullptr;
};

//===============================================================================

} // namespace Chess
