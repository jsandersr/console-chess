////---------------------------------------------------------------
////
//// ChessHelper.h
////
//
//#pragma once
//
//#include <glm/glm.hpp>
//#include <array>
//#include <vector>
//
//
//namespace Chess {
//
//enum class PieceId : uint32_t
//{
//	NONE = 0,
//	EMPTY,
//	PAWN,
//	KNIGHT,
//	ROOK,
//	BISHOP,
//	QUEEN,
//	KING
//};
//
//enum class ColorId : uint32_t {
//	NONE = 0,
//	WHITE,
//	BLACK
//};
//
//struct PieceInfo {
//	// Indicates the kind of piece.
//	PieceId pieceId = PieceId::NONE;
//
//	// Indicates which color the piece is.
//	ColorId colorId = ColorId::NONE;
//
//	// Number of points that a piece is worth.
//	int32_t points = 0;
//
//	// Max number of steps that the piece is allowed to move in a turn.
//	int32_t maxNumSteps = 0;
//
//	// List of valid directions this piece is allowed to move.
//	std::vector<glm::ivec2> directions;
//};
//
//using BoardData = std::array<std::array<PieceInfo, 8>, 8>;
//
//static const glm::ivec2 s_up = { -1,  0 };
//static const glm::ivec2 s_left = { 0, -1 };
//static const glm::ivec2 s_down = { 1,  0 };
//static const glm::ivec2 s_right = { 0,  1 };
//static const glm::ivec2 s_upLeft = { -1, -1 };
//static const glm::ivec2 s_upRight = { -1,  1 };
//static const glm::ivec2 s_downLeft = { 1, -1 };
//static const glm::ivec2 s_downRight = { 1,  1 };
//
//static const std::array<glm::ivec2, 1> s_whitePawnDirections{ { {s_up} } };
//static const std::array<glm::ivec2, 1> s_blackPawnDirections{ { {s_down} } };
//static const std::array<glm::ivec2, 8> s_queenDirections{ { {s_up}, {s_down}, {s_left}, {s_right}, {s_upLeft}, {s_downLeft}, {s_upRight}, {s_downRight} } };
//static const std::array<glm::ivec2, 8> s_kingDirections{ { {s_up}, {s_down}, {s_left}, {s_right}, {s_upLeft}, {s_downLeft}, {s_upRight}, {s_downRight} } };
//static const std::array<glm::ivec2, 4> s_rookDirections{ { {s_up}, {s_down}, {s_left}, {s_right} } };
//static const std::array<glm::ivec2, 4> s_bishopDirections{ { {s_upLeft}, {s_upRight}, {s_downLeft}, {s_downRight} } };
//static const std::array<glm::ivec2, 4> s_knightDirections{ { {-2, -1}, {-2, 1}, {2, -1}, {2, 1} } };
//
//static const PieceInfo s_whitePawn{
//	PieceId::PAWN, ColorId::WHITE, 1, 1,
//	{ std::cbegin(s_whitePawnDirections), std::cend(s_whitePawnDirections)}
//};
//static const PieceInfo s_blackPawn{
//	PieceId::PAWN, ColorId::BLACK, 1, 1,
//	{ std::cend(s_blackPawnDirections), std::cend(s_blackPawnDirections) }
//};
//static const PieceInfo s_whiteKnight{
//	PieceId::KNIGHT, ColorId::WHITE, 3, 1,
//	{ std::cbegin(s_knightDirections), std::cend(s_knightDirections) }
//};
//static const PieceInfo s_blackKnight{
//	PieceId::KNIGHT, ColorId::BLACK, 3, 1,
//	{ std::cbegin(s_knightDirections), std::cend(s_knightDirections) }
//};
//static const PieceInfo s_whiteRook{
//	PieceId::ROOK, ColorId::WHITE, 5, 8,
//	{ std::cbegin(s_rookDirections), std::end(s_rookDirections) }
//};
//static const PieceInfo s_blackRook{
//	PieceId::ROOK, ColorId::BLACK, 5, 8,
//	{ std::cbegin(s_rookDirections), std::cend(s_rookDirections) }
//};
//static const PieceInfo s_whiteBishop{
//	PieceId::BISHOP, ColorId::WHITE, 3, 8,
//	{ std::cbegin(s_bishopDirections), std::cend(s_bishopDirections) }
//};
//static const PieceInfo s_blackBishop{
//	PieceId::BISHOP, ColorId::BLACK, 3, 8,
//	{ std::cbegin(s_bishopDirections), std::cend(s_bishopDirections) }
//};
//static const PieceInfo s_whiteQueen{
//	PieceId::QUEEN,  ColorId::WHITE, 9, 8,
//	{ std::cbegin(s_queenDirections), std::cend(s_queenDirections) }
//};
//static const PieceInfo s_blackQueen{
//	PieceId::QUEEN,  ColorId::BLACK, 9, 8,
//	{ std::cbegin(s_queenDirections), std::cend(s_queenDirections) }
//};
//static const PieceInfo s_whiteKing{
//	PieceId::KING, ColorId::WHITE, std::numeric_limits<int32_t>::max(), 1,
//	{ std::cbegin(s_kingDirections), std::cend(s_kingDirections) }
//};
//static const PieceInfo s_blackKing{
//	PieceId::KING, ColorId::BLACK, std::numeric_limits<int32_t>::max(), 1,
//	{ std::cbegin(s_kingDirections), std::cend(s_kingDirections) }
//};
//static const PieceInfo s_empty{ PieceId::EMPTY,  ColorId::NONE,  0, 0, {} };
//
//static void ResetBoard(BoardData& board)
//{
//	board = { {
//		s_blackRook, s_blackKnight, s_blackBishop, s_blackQueen, s_blackKing, s_blackBishop, s_blackKnight, s_blackRook,
//		s_blackPawn, s_blackPawn, s_blackPawn, s_blackPawn, s_blackPawn, s_blackPawn, s_blackPawn, s_blackPawn,
//		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
//		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
//		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
//		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
//		s_whitePawn, s_whitePawn, s_whitePawn, s_whitePawn, s_whitePawn, s_whitePawn, s_whitePawn, s_whitePawn,
//		s_whiteRook, s_whiteKnight, s_whiteBishop, s_whiteQueen, s_whiteKing, s_whiteBishop, s_whiteKnight, s_whiteRook
//	} };
//}
//
//static void SetTestBoard(BoardData& board)
//{
//	board = { {
//		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
//		s_empty, s_empty, s_empty, s_empty, s_blackKing, s_empty, s_empty, s_empty,
//		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
//		s_empty, s_empty, s_empty, s_whitePawn, s_empty, s_empty, s_empty, s_empty,
//		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
//		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
//		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
//		s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty, s_empty,
//	} };
//}
///*
//	// Board mapping reference
//		a	b	c	d	e	f	g	h
//		0	1	2	3	4	5	6	7
//	8   0
//	7   1
//	6   2
//	5   3
//	4   4
//	3   5
//	2   6
//	1   7
//*/
//// Maps a string chess coord to a 2D array coord. Sure I could write an algorithm to do this but.. why? Its static data that never changes.
//static std::unordered_map<std::string, glm::ivec2> s_coordMap = {
//	{"a8", {0, 0}}, {"a7", {1, 0}}, {"a6", {2, 0}}, {"a5", {3, 0}}, {"a4", {4, 0}}, {"a3", {5, 0}}, {"a2", {6, 0}}, {"a1", {7, 0}},
//	{"b8", {0, 1}}, {"b7", {1, 1}}, {"b6", {2, 1}}, {"b5", {3, 1}}, {"b4", {4, 1}}, {"b3", {5, 1}}, {"b2", {6, 1}}, {"b1", {7, 1}},
//	{"c8", {0, 2}}, {"c7", {1, 2}}, {"c6", {2, 2}}, {"c5", {3, 2}}, {"c4", {4, 2}}, {"c3", {5, 2}}, {"c2", {6, 2}}, {"c1", {7, 2}},
//	{"d8", {0, 3}}, {"d7", {1, 3}}, {"d6", {2, 3}}, {"d5", {3, 3}}, {"d4", {4, 3}}, {"d3", {5, 3}}, {"d2", {6, 3}}, {"d1", {7, 3}},
//	{"e8", {0, 4}}, {"e7", {1, 4}}, {"e6", {2, 4}}, {"e5", {3, 4}}, {"e4", {4, 4}}, {"e3", {5, 4}}, {"e2", {6, 4}}, {"e1", {7, 4}},
//	{"f8", {0, 5}}, {"f7", {1, 5}}, {"f6", {2, 5}}, {"f5", {3, 5}}, {"f4", {4, 5}}, {"f3", {5, 5}}, {"f2", {6, 5}}, {"f1", {7, 5}},
//	{"g8", {0, 6}}, {"g7", {1, 6}}, {"g6", {2, 6}}, {"g5", {3, 6}}, {"g4", {4, 6}}, {"g3", {5, 6}}, {"g2", {6, 6}}, {"g1", {7, 6}},
//	{"h8", {0, 7}}, {"h7", {1, 7}}, {"h6", {2, 7}}, {"h5", {3, 7}}, {"h4", {4, 7}}, {"h3", {5, 7}}, {"h2", {6, 7}}, {"h1", {7, 7}},
//};
//
//namespace Util {
//
//const PieceInfo& GetPieceAt(const BoardData& board, const glm::ivec2& source)
//{
//	if (!IsCoordInBounds(board, source))
//		return s_empty;
//
//	return board[source.x][source.y];
//}
//
//void SetPieceAt(BoardData& board, const glm::ivec2& dest, const PieceInfo& piece)
//{
//	if (!IsCoordInBounds(board, dest))
//		return;
//
//	board[dest.x][dest.y] = piece;
//}
//
//std::vector<glm::ivec2> GetValidDestList(const BoardData& board, const glm::ivec2& source, const PieceInfo& piece)
//{
//	std::vector<glm::ivec2> validDestList;
//	for (const auto& direction : piece.directions)
//	{
//		for (int i = 1; i <= piece.maxNumSteps; ++i)
//		{
//			glm::ivec2 dest = source + direction * i;
//
//			// Any invalid destination will break our line of sight.
//			if (!IsValidDest(board, dest))
//			{
//				break;
//			}
//
//			validDestList.push_back(dest);
//		}
//	}
//
//	// We get a chance down here to append any other special case moves to the list.
//	if (piece.pieceId == PieceId::PAWN)
//	{
//		HandlePawnSpecialCase(board, validDestList, source, piece.colorId);
//	}
//
//	return validDestList;
//}
//
//void HandlePawnSpecialCase(const BoardData& board, std::vector<glm::ivec2>& destList,
//	const glm::ivec2& source, ColorId id)
//{
//	glm::ivec2 direction = colorId == ColorId::BLACK ? s_down : s_up;
//
//	// Pawns in a specific row and color may move two spaces forward.
//	if ((source.x == 1 && colorId == ColorId::BLACK)
//		|| (source.x == 6 && colorId == ColorId::WHITE))
//	{
//		glm::ivec2 dest;
//		for (int i = 1; i <= 2; ++i)
//		{
//			dest = source + direction * i;
//			if (!IsValidDest(board, dest))
//			{
//				break;
//			}
//		}
//
//		destList.push_back(dest);
//	}
//
//	auto handleDiagMove = [&source, &destList, &board](glm::ivec2& diag, ColorId colorId)
//	{
//		glm::ivec2 dest = source + diag;
//		if (IsValidDest(board, dest) && GetPieceAt(board, dest).pieceId != PieceId::EMPTY)
//		{
//			destList.push_back(dest);
//		}
//	};
//
//	// Pawns may also move diagonally if there is an enemy piece there.
//	glm::ivec2 diagLeft = colorId == ColorId::BLACK ? s_downLeft : s_upLeft;
//	handleDiagMove(diagLeft, colorId);
//
//	glm::ivec2 diagRight = colorId == ColorId::BLACK ? s_downRight : s_upRight;
//	handleDiagMove(diagRight, colorId);
//}
//
//void HandleCastleSpecialCase(const BoardData& board, std::vector<glm::ivec2>& destList,
//	const glm::ivec2& source, ColorId id)
//{
//	// TODO: requires state tracking. Rules for check:
//	// 1) King has not moved. I'll need to track if king has moved or not.
//	// 2) King must not pass through check. Will need to check for this for each step.
//	// 3) King must not los rook. (no pieces between).
//	// 4) King must not currently be in check.
//
//	// Potentially create a Piece class for tracking state. Piece will have a PieceInfo struct in addition
//	// to state.
//
//	// Alternatively, if this is the only case where I need state tracking, just track if a king has moved
//	// in Player class.
//}
//
//void DoMovePiece(BoardData& board, const glm::ivec2& source, const glm::ivec2& dest, const PieceInfo& piece)
//{
//
//}
//
//// Returns a list of piece positions that cause a check.
//std::vector<glm::ivec2> SearchForCheck(const BoardData& data)
//{
//	// Find King position
//	glm::ivec2 kingPos = GetKingPos(board);
//
//	// Positions of all the pieces that cause check. We start with all enemies,
//	// then filter the list down based on several criteria.
//	std::vector<std::pair<glm::ivec2, bool>> candidates;
//	int32_t boardSize = static_cast<int32_t>(board.size());
//	for (int row = 0; row < boardSize; ++row)
//	{
//		for (int col = 0; col < boardSize; ++col)
//		{
//			// TODO: we can trim this list down with some maths.
//			glm::ivec2 pos{ row, col };
//			if (IsEnemyPiece(GetPieceAt(board, pos)))
//			{
//				candidates.push_back({ pos, false });
//			}
//		}
//	}
//
//	// Now we have a list of all the enemy pieces. Let's validate them.
//	for (auto& candidate : candidates)
//	{
//		const auto& candidatePos = candidate.first;
//		auto& isCandidateValid = candidate.second;
//		const auto& piece = GetPieceAt(board, candidatePos);
//
//		const auto& destList = GetValidDestList(board, candidatePos, piece);
//
//		auto found = std::find(std::cbegin(destList), std::cend(destList), kingPos);
//		if (found != std::cend(destList))
//		{
//			isCandidateValid = true;
//		}
//	}
//
//	std::vector<glm::ivec2> piecePositions;
//	for (const auto candidate : candidates)
//	{
//		const auto& candidatePosition = candidate.first;
//		const auto& isCandidateValid = candidate.second;
//		if (isCandidateValid)
//		{
//			piecePositions.push_back(candidatePosition);
//		}
//	}
//
//	return piecePositions;
//}
//
//// Helper for getting the current king position.
//glm::ivec2 GetKingPos(const BoardData& board)
//{
//
//}
//
//bool IsCoordInBounds(const BoardData& board, const glm::ivec2& coord)
//{
//
//}
//
//bool IsValidDest(const BoardData& board, const glm::ivec2& dest)
//{
//
//}
//
//} // namespace Util
//
//} // namespace Chess
