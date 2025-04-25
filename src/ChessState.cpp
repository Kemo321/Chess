#include "ChessState.hpp"


ChessState::ChessState(const std::string& stateStr)
{
    if (!checkIfStringIsValid(stateStr))
        throw std::invalid_argument("Invalid state string");

    // Fill board (first 64 characters)
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            this->state[i][j] = stateStr[i * 8 + j];

    // Flags stored in remaining characters
    this->whiteToMove = stateStr[64] == '1';
    this->whiteKingMoved = stateStr[65] == '1';
    this->blackKingMoved = stateStr[66] == '1';
    this->whiteRookAMoved = stateStr[67] == '1';
    this->whiteRookBMoved = stateStr[68] == '1';
    this->blackRookAMoved = stateStr[69] == '1';
    this->blackRookBMoved = stateStr[70] == '1';
}

bool ChessState::checkIfStringIsValid(const std::string& state)
{
    if (state.size() != 71)
        return false;

    // Check board characters
    for (int i = 0; i < 64; i++)
    {
        char c = state[i];
        if (c != '0' && c != 'P' && c != 'p' && c != 'N' && c != 'n' && c != 'B' && c != 'b' &&
            c != 'R' && c != 'r' && c != 'Q' && c != 'q' && c != 'K' && c != 'k')
            return false;
    }

    // Check flags
    for (int i = 64; i < 71; i++)
    {
        if (state[i] != '0' && state[i] != '1')
            return false;
    }

    return true;
}

void ChessState::makeMove(const ChessMove& move)
{
    // Save the side that is moving
    bool movingWhite = whiteToMove;

    std::pair<int, int> from = move.getFrom();
    std::pair<int, int> to = move.getTo();
    char movedPiece = state[from.first][from.second];
    char capturedPiece = state[to.first][to.second];


    MoveRecord record{from, to, movedPiece, capturedPiece};

    // Save previous flags
    record.prevWhiteKingMoved = whiteKingMoved;
    record.prevBlackKingMoved = blackKingMoved;
    record.prevWhiteRookAMoved = whiteRookAMoved;
    record.prevWhiteRookBMoved = whiteRookBMoved;
    record.prevBlackRookAMoved = blackRookAMoved;
    record.prevBlackRookBMoved = blackRookBMoved;
    record.prevEnPassantSquare = enPassantSquare;

    // --- Standard move: update board ---
    state[to.first][to.second] = movedPiece;
    state[from.first][from.second] = '0';

    // --- Pawn promotion ---
    if ((movedPiece == 'P' && to.first == 0) || (movedPiece == 'p' && to.first == 7))
    {
        int promotion = move.getPromotion();
        switch (promotion)
        {
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

    // --- Castling ---
    if (movedPiece == 'K' || movedPiece == 'k')
    {
        // King-side castling:
        if (from.second == 4 && to.second == 6)
        {
            record.isCastling = true;
            record.rookFrom = {from.first, 7};
            record.rookTo = {from.first, 5};
            state[from.first][5] = state[from.first][7];
            state[from.first][7] = '0';
        }
        // Queen-side castling:
        else if (from.second == 4 && to.second == 2)
        {
            record.isCastling = true;
            record.rookFrom = {from.first, 0};
            record.rookTo = {from.first, 3};
            state[from.first][3] = state[from.first][0];
            state[from.first][0] = '0';
        }
        // Update king moved flag:
        if (movedPiece == 'K')
            whiteKingMoved = true;
        else
            blackKingMoved = true;
    }

    // --- En passant ---
    if (movedPiece == 'P' || movedPiece == 'p')
    {
        if (std::abs(from.second - to.second) == 1 && capturedPiece == '0')
        {
            // For en passant, the captured pawn lies behind the destination square.
            int captureRow = movingWhite ? to.first + 1 : to.first - 1;
            if (isInsideBoard(captureRow, to.second) && state[captureRow][to.second] != '0')
            {
                record.isEnPassant = true;
                record.enPassantCapturedPos = {captureRow, to.second};
                capturedPiece = state[captureRow][to.second];
                state[captureRow][to.second] = '0';
            }
        }
    }

    // --- Update enPassantSquare ---
    enPassantSquare = {-1, -1}; // Reset by default
    if ((movedPiece == 'P' && from.first == 6 && to.first == 4) || (movedPiece == 'p' && from.first == 1 && to.first == 3))
    {
        enPassantSquare = {to.first + (movingWhite ? 1 : -1), to.second};
    }

    // --- Update rook moved flags ---
    if (movedPiece == 'R')
    {
        if (from == std::make_pair(7, 0))
            whiteRookAMoved = true;
        else if (from == std::make_pair(7, 7))
            whiteRookBMoved = true;
    }
    else if (movedPiece == 'r')
    {
        if (from == std::make_pair(0, 0))
            blackRookAMoved = true;
        else if (from == std::make_pair(0, 7))
            blackRookBMoved = true;
    }

    // --- Record the move ---
    moveHistory.push_back(record);

    // --- Switch turn ---
    whiteToMove = !whiteToMove;
}

// =====================================================
// unmakeMove: revert a move using data stored in the MoveRecord.
// =====================================================
void ChessState::unmakeMove(const ChessMove& move)
{
    if (moveHistory.empty())
        return;

    MoveRecord record = moveHistory.back();
    moveHistory.pop_back();

    // Revert move of piece (king/pawn, etc.)
    state[record.from.first][record.from.second] = record.movedPiece;
    state[record.to.first][record.to.second] = record.capturedPiece;

    // Undo pawn promotion if necessary:
    if ((record.movedPiece == 'P' && record.to.first == 0) || (record.movedPiece == 'p' && record.to.first == 7))
    {
        state[record.from.first][record.from.second] = record.movedPiece;
    }

    // Revert castling:
    if (record.isCastling)
    {
        // Move rook back:
        state[record.rookFrom.first][record.rookFrom.second] = state[record.rookTo.first][record.rookTo.second];
        state[record.rookTo.first][record.rookTo.second] = '0';
    }

    // Revert en passant:
    if (record.isEnPassant)
    {
        // Restore the captured pawn to its proper square.
        auto pos = record.enPassantCapturedPos;
        state[pos.first][pos.second] = (record.movedPiece == 'P') ? 'p' : 'P';
    }

    // Restore previous flags
    whiteKingMoved = record.prevWhiteKingMoved;
    blackKingMoved = record.prevBlackKingMoved;
    whiteRookAMoved = record.prevWhiteRookAMoved;
    whiteRookBMoved = record.prevWhiteRookBMoved;
    blackRookAMoved = record.prevBlackRookAMoved;
    blackRookBMoved = record.prevBlackRookBMoved;
    enPassantSquare = record.prevEnPassantSquare;

    // Switch turn back.
    whiteToMove = !whiteToMove;
}

bool ChessState::isSquareAttacked(int row, int col, bool attackedByWhite) const
{
    // Pawn attacks:
    if (attackedByWhite) {
        if (isInsideBoard(row + 1, col + 1) && state[row + 1][col + 1] == 'P') return true;
        if (isInsideBoard(row + 1, col - 1) && state[row + 1][col - 1] == 'P') return true;
    }
    else {
        if (isInsideBoard(row - 1, col - 1) && state[row - 1][col - 1] == 'p') return true;
        if (isInsideBoard(row - 1, col + 1) && state[row - 1][col + 1] == 'p') return true;
    }

    // Knight attacks:
    const std::vector<std::pair<int,int>> knightOffsets = { {-2,-1}, {-2,1}, {2,-1}, {2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2} };
    for (auto [dx, dy] : knightOffsets) {
        int r = row + dx, c = col + dy;
        if (isInsideBoard(r, c)) {
            char piece = state[r][c];
            if (attackedByWhite && piece == 'N') return true;
            if (!attackedByWhite && piece == 'n') return true;
        }
    }

    // Sliding pieces: bishops & queens (diagonals)
    const std::vector<std::pair<int,int>> diagonalOffsets = { {-1,-1}, {-1,1}, {1,-1}, {1,1} };
    for (auto [dx, dy] : diagonalOffsets) 
    {
        int r = row + dx, c = col + dy;
        while (isInsideBoard(r, c) && state[r][c] == '0') 
        {
            r += dx;
            c += dy;
        }
        if (isInsideBoard(r, c)) 
        {
            char piece = state[r][c];
            if (attackedByWhite && (piece == 'B' || piece == 'Q'))
                return true;
            if (!attackedByWhite && (piece == 'b' || piece == 'q'))
                return true;
        }
    }

    // Sliding pieces: rooks & queens (orthogonal)
    const std::vector<std::pair<int,int>> straightOffsets = { {-1,0}, {1,0}, {0,-1}, {0,1} };
    for (auto [dx, dy] : straightOffsets) 
    {
        int r = row + dx, c = col + dy;
        while (isInsideBoard(r, c) && state[r][c] == '0') 
        {
            r += dx;
            c += dy;
        }
        if (isInsideBoard(r, c)) 
        {
            char piece = state[r][c];
            if (attackedByWhite && (piece == 'R' || piece == 'Q'))
                return true;
            if (!attackedByWhite && (piece == 'r' || piece == 'q'))
                return true;
        }
    }

    // King attacks:
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if (dx == 0 && dy == 0) continue;
            int r = row + dx, c = col + dy;
            if (isInsideBoard(r, c)) 
            {
                char piece = state[r][c];
                if (attackedByWhite && piece == 'K') return true;
                if (!attackedByWhite && piece == 'k') return true;
            }
        }
    }

    return false;
}

bool ChessState::isInsideBoard(int row, int col) const 
{
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool ChessState::isOpponentPiece(char piece) const 
{
    return (whiteToMove && piece >= 'a' && piece <= 'z') || (!whiteToMove && piece >= 'A' && piece <= 'Z');
}

bool ChessState::isOwnPiece(char piece) const 
{
    return (whiteToMove && piece >= 'A' && piece <= 'Z') || (!whiteToMove && piece >= 'a' && piece <= 'z');
}

bool ChessState::isTerminal()
{
    return isCheckmate() || isStalemate();
}

bool ChessState::isInCheck(bool byWhite) const
{
    int kingRow = byWhite ? getWhiteKingRow() : getBlackKingRow();
    int kingCol = byWhite ? getWhiteKingCol() : getBlackKingCol();
    return isSquareAttacked(kingRow, kingCol, !byWhite);
}

std::vector<ChessMove> ChessState::getLegalMoves()
{
    std::vector<ChessMove> legalMoves;
    for (int row = 0; row < 8; ++row) 
    {
        for (int col = 0; col < 8; ++col) 
        {
            if (isOwnPiece(state[row][col])) 
            {
                switch (state[row][col]) {
                    case 'P':
                    case 'p':
                        getPawnMoves(row, col, legalMoves);
                        break;
                    case 'N':
                    case 'n':
                        getKnightMoves(row, col, legalMoves);
                        break;
                    case 'B':
                    case 'b':
                        getBishopMoves(row, col, legalMoves);
                        break;
                    case 'R':
                    case 'r':
                        getRookMoves(row, col, legalMoves);
                        break;
                    case 'Q':
                    case 'q':
                        getQueenMoves(row, col, legalMoves);
                        break;
                    case 'K':
                    case 'k':
                        getKingMoves(row, col, legalMoves);
                        break;
                    default:

                        break;
                }
            }
        }
    }
    return legalMoves;
}

char ChessState::getPieceAt(int row, int col) const 
{
    return state[row][col];
}

bool ChessState::isLegalMove(const ChessMove& move)
{
    bool ret;
    makeMove(move);
    // Note: We check the king of the side that just moved.
    if (!whiteToMove) {
        int kingRow = getWhiteKingRow();
        int kingCol = getWhiteKingCol();
        ret = !isSquareAttacked(kingRow, kingCol, /*attackedByWhite=*/false);
    }
    else {
        int kingRow = getBlackKingRow();
        int kingCol = getBlackKingCol();
        ret = !isSquareAttacked(kingRow, kingCol, /*attackedByWhite=*/true);
    }
    unmakeMove(move);

    return ret;
}


///////////////////////////////////////////////////
// King positions
///////////////////////////////////////////////////


int ChessState::getWhiteKingRow() const 
{
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            if (state[i][j] == 'K') 
            {
                return i;
            }
        }
    }
    return -1;
}

int ChessState::getBlackKingRow() const 
{
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            if (state[i][j] == 'k') 
            {
                return i;
            }
        }
    }
    return -1;
}

int ChessState::getWhiteKingCol() const 
{
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            if (state[i][j] == 'K') 
            {
                return j;
            }
        }
    }
    return -1;
}

int ChessState::getBlackKingCol() const 
{
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            if (state[i][j] == 'k') 
            {
                return j;
            }
        }
    }
    return -1;
}

///////////////////////////////////////////////////
// Checkmate, Stalemate, Draw
///////////////////////////////////////////////////

bool ChessState::isCheckmate()
{
    int kingRow = whiteToMove ? getWhiteKingRow() : getBlackKingRow();
    int kingCol = whiteToMove ? getWhiteKingCol() : getBlackKingCol();
    bool inCheck = isSquareAttacked(kingRow, kingCol, !whiteToMove);
    if (!inCheck)
        return false;
    std::vector<ChessMove> legalMoves = getLegalMoves();
    return legalMoves.empty();
}

bool ChessState::isStalemate()
{
    int kingRow = whiteToMove ? getWhiteKingRow() : getBlackKingRow();
    int kingCol = whiteToMove ? getWhiteKingCol() : getBlackKingCol();
    bool inCheck = isSquareAttacked(kingRow, kingCol, !whiteToMove);
    if (inCheck)
        return false;
    std::vector<ChessMove> legalMoves = getLegalMoves();
    return legalMoves.empty();
}


///////////////////////////////////////////////////
// Print board
///////////////////////////////////////////////////

void ChessState::printBoard() const 
{
    for (int i = 0; i < 8; i++) 
    {
        std::cout << i << " ";
        for (int j = 0; j < 8; j++) 
        {
            if (state[i][j] >= 'a' && state[i][j] <= 'z') 
            {
                std::cout << "\033[1;31m" << state[i][j] << " \033[0m"; // Print black pieces in red color
            } 
            else if (state[i][j] >= 'A' && state[i][j] <= 'Z') 
            {
                std::cout << "\033[1;33m" << state[i][j] << " \033[0m"; // Print white pieces in yellow color
            } 
            else 
            {
                std::cout << state[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "  0 1 2 3 4 5 6 7" << std::endl;
}

///////////////////////////////////////////////////
// Pieces moves
///////////////////////////////////////////////////


void ChessState::getPawnMoves(int row, int col, std::vector<ChessMove>& legalMoves)
{
    int direction = whiteToMove ? -1 : 1;
    int startRow = whiteToMove ? 6 : 1;

    // Move forward
    if (isInsideBoard(row + direction, col) && state[row + direction][col] == '0')
    {
        // Promotion
        if (row + direction == 0 || row + direction == 7)
        {
            for (int i = 0; i < 4; i++) // 0: Queen, 1: Rook, 2: Knight, 3: Bishop
            {
                ChessMove move(row, col, row + direction, col, i);
                if (isLegalMove(move))
                    legalMoves.push_back(move);
            }
        }
        else
        {
            ChessMove move(row, col, row + direction, col, 0);
            if (isLegalMove(move))
                legalMoves.push_back(move);
        }
    }

    // First move: double step
    if (row == startRow && state[row + direction][col] == '0' && state[row + 2 * direction][col] == '0')
    {
        ChessMove move(row, col, row + 2 * direction, col, 0);
        if (isLegalMove(move))
            legalMoves.push_back(move);
    }

    // Capture
    for (int i = -1; i <= 1; i += 2)
    {
        if (isInsideBoard(row + direction, col + i) && isOpponentPiece(state[row + direction][col + i]))
        {
            // Promotion
            if (row + direction == 0 || row + direction == 7)
            {
                for (int p = 0; p < 4; p++)
                {
                    ChessMove move(row, col, row + direction, col + i, p);
                    if (isLegalMove(move))
                        legalMoves.push_back(move);
                }
            }
            else
            {
                ChessMove move(row, col, row + direction, col + i, 0);
                if (isLegalMove(move))
                    legalMoves.push_back(move);
            }
        }
    }

    // En passant
    if (enPassantSquare.first != -1 && enPassantSquare.second != -1)
    {
        if (row == (whiteToMove ? 3 : 4) && std::abs(col - enPassantSquare.second) == 1 &&
            enPassantSquare.first == row)
        {
            ChessMove move(row, col, row + direction, enPassantSquare.second, 0);
            if (isLegalMove(move))
                legalMoves.push_back(move);
        }
    }
}

void ChessState::getKnightMoves(int row, int col, std::vector<ChessMove>& legalMoves)
{
    for (auto [i, j] : std::vector<std::pair<int, int>>{{-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}}) 
    {
        if (!isInsideBoard(row + i, col + j)) continue;

        // Check if the move is legal
        if (!isLegalMove(ChessMove(row, col, row + i, col + j, 0))) continue;

        // Check if the move is inside the board and if it captures an opponent piece
        if (isInsideBoard(row + i, col + j) && !isOwnPiece(state[row + i][col + j])) 
        {
            legalMoves.push_back(ChessMove(row, col, row + i, col + j, 0));
        }
    }
}

void ChessState::getBishopMoves(int row, int col, std::vector<ChessMove>& legalMoves)
{
    for (int i = -1; i <= 1; i += 2) 
    {
        for (int j = -1; j <= 1; j += 2) 
        {
            int r = row + i, c = col + j;
            while (isInsideBoard(r, c) && !isOwnPiece(state[r][c])) 
            {
                if (!isLegalMove(ChessMove(row, col, r, c, 0))) break;
                legalMoves.push_back(ChessMove(row, col, r, c, 0));
                if (isOpponentPiece(state[r][c])) break;
                r += i;
                c += j;
            }
        }
    }
}

void ChessState::getRookMoves(int row, int col, std::vector<ChessMove>& legalMoves)
{
    for (int i = -1; i <= 1; i += 2) 
    {
        int r = row + i, c = col;
        while (isInsideBoard(r, c) && !isOwnPiece(state[r][c])) 
        {
            if (!isLegalMove(ChessMove(row, col, r, c, 0))) break;
            legalMoves.push_back(ChessMove(row, col, r, c, 0));
            if (isOpponentPiece(state[r][c])) break;
            r += i;
        }

        r = row, c = col + i;
        while (isInsideBoard(r, c) && !isOwnPiece(state[r][c])) 
        {
            if (!isLegalMove(ChessMove(row, col, r, c, 0))) break;
            legalMoves.push_back(ChessMove(row, col, r, c, 0));
            if (isOpponentPiece(state[r][c])) break;
            c += i;
        }
    }
}

void ChessState::getQueenMoves(int row, int col, std::vector<ChessMove>& legalMoves)
{
    for (int i = -1; i <= 1; i++) 
    {
        for (int j = -1; j <= 1; j++) 
        {
            if (i != 0 || j != 0) 
            {
                int r = row + i, c = col + j;
                while (isInsideBoard(r, c) && !isOwnPiece(state[r][c])) 
                {
                    if (!isLegalMove(ChessMove(row, col, r, c, 0))) break;
                    legalMoves.push_back(ChessMove(row, col, r, c, 0));
                    if (isOpponentPiece(state[r][c])) break;
                    r += i;
                    c += j;
                }
            }
        }
    }
}

void ChessState::getKingMoves(int row, int col, std::vector<ChessMove>& legalMoves)
{
    // Standard king moves
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i != 0 || j != 0)
            {
                int r = row + i, c = col + j;
                if (isInsideBoard(r, c) && !isOwnPiece(state[r][c]))
                {
                    ChessMove move(row, col, r, c, 0);
                    if (isLegalMove(move))
                        legalMoves.push_back(move);
                }
            }
        }
    }

    // Castling
    if (whiteToMove)
    {
        if (!whiteKingMoved)
        {
            // King-side
            if (!whiteRookBMoved && state[7][5] == '0' && state[7][6] == '0' && state[7][7] == 'R' &&
                !isSquareAttacked(7, 4, false) && !isSquareAttacked(7, 5, false) && !isSquareAttacked(7, 6, false))
            {
                ChessMove move(7, 4, 7, 6, 0);
                if (isLegalMove(move))
                    legalMoves.push_back(move);
            }
            // Queen-side
            if (!whiteRookAMoved && state[7][1] == '0' && state[7][2] == '0' && state[7][3] == '0' && state[7][0] == 'R' &&
                !isSquareAttacked(7, 4, false) && !isSquareAttacked(7, 3, false) && !isSquareAttacked(7, 2, false))
            {
                ChessMove move(7, 4, 7, 2, 0);
                if (isLegalMove(move))
                    legalMoves.push_back(move);
            }
        }
    }
    else
    {
        if (!blackKingMoved)
        {
            // King-side
            if (!blackRookBMoved && state[0][5] == '0' && state[0][6] == '0' && state[0][7] == 'r' &&
                !isSquareAttacked(0, 4, true) && !isSquareAttacked(0, 5, true) && !isSquareAttacked(0, 6, true))
            {
                ChessMove move(0, 4, 0, 6, 0);
                if (isLegalMove(move))
                    legalMoves.push_back(move);
            }
            // Queen-side
            if (!blackRookAMoved && state[0][1] == '0' && state[0][2] == '0' && state[0][3] == '0' && state[0][0] == 'r' &&
                !isSquareAttacked(0, 4, true) && !isSquareAttacked(0, 3, true) && !isSquareAttacked(0, 2, true))
            {
                ChessMove move(0, 4, 0, 2, 0);
                if (isLegalMove(move))
                    legalMoves.push_back(move);
            }
        }
    }
}

std::string ChessState::toString() const
{
    std::string stateStr;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            stateStr += state[i][j];

    stateStr += whiteToMove ? "1" : "0";
    stateStr += whiteKingMoved ? "1" : "0";
    stateStr += blackKingMoved ? "1" : "0";
    stateStr += whiteRookAMoved ? "1" : "0";
    stateStr += whiteRookBMoved ? "1" : "0";
    stateStr += blackRookAMoved ? "1" : "0";
    stateStr += blackRookBMoved ? "1" : "0";

    return stateStr;
}
