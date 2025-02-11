#include "ChessState.hpp"
#include "ChessMove.hpp"
#include <vector>
#include <iostream>

ChessState::ChessState(const std::string& state)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            this->state[i][j] = state[i * 8 + j];
        }
    }

    this->whiteToMove = state[64] == '1';
    this->whiteKingMoved = state[65] == '1';
    this->blackKingMoved = state[66] == '1';
    this->whiteRookAMoved = state[67] == '1';
    this->whiteRookBMoved = state[68] == '1';
    this->blackRookAMoved = state[69] == '1';
    this->blackRookBMoved = state[70] == '1';
    this->whiteKingInCheck = state[71] == '1';
    this->blackKingInCheck = state[72] == '1';
    this->whiteKingInCheckmate = state[73] == '1';
    this->blackKingInCheckmate = state[74] == '1';
    this->whiteKingInStalemate = state[75] == '1';
    this->blackKingInStalemate = state[76] == '1';
    this->whiteKingInDraw = state[77] == '1';
    this->blackKingInDraw = state[78] == '1';
}

void ChessState::makeMove(const ChessMove& move) {
    std::pair<int, int> from = move.getFrom();
    std::pair<int, int> to = move.getTo();
    char movedPiece = state[from.first][from.second];
    char capturedPiece = state[to.first][to.second];

    // Perform move
    state[to.first][to.second] = movedPiece;
    state[from.first][from.second] = '0';

    // Pawn promotion
    if ((movedPiece == 'P' && to.first == 0) || (movedPiece == 'p' && to.first == 7)) {
        int promotion = move.getPromotion();

        switch (promotion) {
            case 0:
                state[to.first][to.second] = (movedPiece == 'P') ? 'Q' : 'q';
                break;
            case 1:
                state[to.first][to.second] = (movedPiece == 'P') ? 'R' : 'r';
                break;
            case 2:
                state[to.first][to.second] = (movedPiece == 'P') ? 'N' : 'n';
                break;
            case 3:
                state[to.first][to.second] = (movedPiece == 'P') ? 'B' : 'b';
                break;
            default:
                break;
        }
    }

    // Castling
    if (movedPiece == 'K' || movedPiece == 'k') {
        if (from.second == 4 && to.second == 6) { // King-side castling
            // Move the rook
            state[from.first][5] = state[from.first][7];
            state[from.first][7] = '0';

            // Move the king
            state[from.first][6] = movedPiece;
            state[from.first][4] = '0';

        } else if (from.second == 4 && to.second == 2) { // Queen-side castling
            // Move the rook
            state[from.first][3] = state[from.first][0];
            state[from.first][0] = '0';

            // Move the king
            state[from.first][2] = movedPiece;
            state[from.first][4] = '0';
        }
        if (movedPiece == 'K') whiteKingMoved = true;
        if (movedPiece == 'k') blackKingMoved = true;
    }

    // En passant capture
    if (movedPiece == 'P' || movedPiece == 'p') {
        if (abs(from.second - to.second) == 1 && capturedPiece == '0') {
            state[from.first][to.second] = '0';
        }
    }

    // Update move history
    moveHistory.push_back({from, to, movedPiece, capturedPiece});

    // Switch turns
    whiteToMove = !whiteToMove;

    // Update game state
    updateGameState();
}

void ChessState::unmakeMove(const ChessMove& move)
{
    if (moveHistory.empty()) return;

    MoveRecord lastMove = moveHistory.back();
    moveHistory.pop_back();

    state[lastMove.from.first][lastMove.from.second] = lastMove.movedPiece;
    state[lastMove.to.first][lastMove.to.second] = lastMove.capturedPiece;

    // Undo castling
    if (lastMove.movedPiece == 'K' || lastMove.movedPiece == 'k') {
        if (lastMove.from.second == 4 && lastMove.to.second == 6) { // King-side
            state[lastMove.from.first][7] = state[lastMove.from.first][5];
            state[lastMove.from.first][5] = '0';
        } else if (lastMove.from.second == 4 && lastMove.to.second == 2) { // Queen-side
            state[lastMove.from.first][0] = state[lastMove.from.first][3];
            state[lastMove.from.first][3] = '0';
        }
    }

    // Undo en passant
    if ((lastMove.movedPiece == 'P' || lastMove.movedPiece == 'p') && abs(lastMove.from.second - lastMove.to.second) == 1 && lastMove.capturedPiece == '0') {
        state[lastMove.from.first][lastMove.to.second] = (lastMove.movedPiece == 'P') ? 'p' : 'P';
    }

    // Switch turns back
    whiteToMove = !whiteToMove;
}

void ChessState::updateGameState() {
    int whiteKingRow = -1, whiteKingCol = -1;
    int blackKingRow = -1, blackKingCol = -1;

    // Locate the kings
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (state[row][col] == 'K') {
                whiteKingRow = row;
                whiteKingCol = col;
            } else if (state[row][col] == 'k') {
                blackKingRow = row;
                blackKingCol = col;
            }
        }
    }

    whiteKingInCheck = isSquareAttacked(whiteKingRow, whiteKingCol, false);
    blackKingInCheck = isSquareAttacked(blackKingRow, blackKingCol, true);

    // Checkmate & Stalemate logic
    if (whiteKingInCheck && getLegalMoves().empty()) {
        whiteKingInCheckmate = true;
    }
    if (blackKingInCheck && getLegalMoves().empty()) {
        blackKingInCheckmate = true;
    }
    if (!whiteKingInCheck && getLegalMoves().empty()) {
        whiteKingInStalemate = true;
    }
    if (!blackKingInCheck && getLegalMoves().empty()) {
        blackKingInStalemate = true;
    }
}

bool ChessState::isSquareAttacked(int row, int col, bool byWhite) const {
    std::vector<ChessMove> opponentMoves;
    ChessState tempState = *this;
    tempState.whiteToMove = byWhite;
    opponentMoves = tempState.getLegalMoves();

    for (const ChessMove& move : opponentMoves) {
        if (move.getTo().first == row && move.getTo().second == col) {
            return true;
        }
    }
    return false;
}

bool ChessState::isInsideBoard(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool ChessState::isOpponentPiece(char piece) const {
    return (whiteToMove && piece >= 'a' && piece <= 'z') || (!whiteToMove && piece >= 'A' && piece <= 'Z');
}

bool ChessState::isOwnPiece(char piece) const {
    return (whiteToMove && piece >= 'A' && piece <= 'Z') || (!whiteToMove && piece >= 'a' && piece <= 'z');
}

bool ChessState::isTerminal() const {
    return (whiteKingInCheckmate || blackKingInCheckmate || whiteKingInStalemate || blackKingInStalemate || whiteKingInDraw || blackKingInDraw);
}

std::vector<ChessMove> ChessState::getLegalMoves() const {
    std::vector<ChessMove> legalMoves;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (isOwnPiece(state[row][col])) {
                switch (state[row][col]) {
                    case 'P':
                    case 'p':
                        legalMoves.push_back(ChessMove(row, col, row + (whiteToMove ? -1 : 1), col, 0));
                        break;
                    case 'N':
                    case 'n':
                        for (int i = -2; i <= 2; i++) {
                            for (int j = -2; j <= 2; j++) {
                                if (abs(i * j) == 2) {
                                    if (isInsideBoard(row + i, col + j) && !isOwnPiece(state[row + i][col + j])) {
                                        legalMoves.push_back(ChessMove(row, col, row + i, col + j, 0));
                                    }
                                }
                            }
                        }
                        break;
                    case 'B':
                    case 'b':
                        for (int i = -1; i <= 1; i += 2) {
                            for (int j = -1; j <= 1; j += 2) {
                                int r = row + i, c = col + j;
                                while (isInsideBoard(r, c) && !isOwnPiece(state[r][c])) {
                                    legalMoves.push_back(ChessMove(row, col, r, c, 0));
                                    if (isOpponentPiece(state[r][c])) break;
                                    r += i;
                                    c += j;
                                }
                            }
                        }
                        break;
                    case 'R':
                    case 'r':
                        for (int i = -1; i <= 1; i += 2) {
                            int r = row + i, c = col;
                            while (isInsideBoard(r, c) && !isOwnPiece(state[r][c])) {
                                legalMoves.push_back(ChessMove(row, col, r, c, 0));
                                if (isOpponentPiece(state[r][c])) break;
                                r += i;
                            }

                            r = row, c = col + i;
                            while (isInsideBoard(r, c) && !isOwnPiece(state[r][c])) {
                                legalMoves.push_back(ChessMove(row, col, r, c, 0));
                                if (isOpponentPiece(state[r][c])) break;
                                c += i;
                            }
                        }
                        break;
                    case 'Q':
                    case 'q':
                        for (int i = -1; i <= 1; i++) {
                            for (int j = -1; j <= 1; j++) {
                                if (i != 0 || j != 0) {
                                    int r = row + i, c = col + j;
                                    while (isInsideBoard(r, c) && !isOwnPiece(state[r][c])) {
                                        legalMoves.push_back(ChessMove(row, col, r, c, 0));
                                        if (isOpponentPiece(state[r][c])) break;
                                        r += i;
                                        c += j;
                                    }
                                }
                            }
                        }
                        break;
                    case 'K':
                    case 'k':
                        for (int i = -1; i <= 1; i++) {
                            for (int j = -1; j <= 1; j++) {
                                if (i != 0 || j != 0) {
                                    if (isInsideBoard(row + i, col + j) && !isOwnPiece(state[row + i][col + j])) {
                                        legalMoves.push_back(ChessMove(row, col, row + i, col + j, 0));
                                    }
                                }
                            }
                        }
                        break;
                    default:

                        break;
                }
            }
        }
    }
    return legalMoves;
}

char ChessState::getPieceAt(int row, int col) const {
    return state[row][col];
}

bool ChessState::isInCheck(bool byWhite) const {
    return byWhite ? whiteKingInCheck : blackKingInCheck;
}
