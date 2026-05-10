#include <iostream>
#include <windows.h>
#include <string>
#include <cctype>

using namespace std;

const int width = 11;
const int height = 5;

class piece
{
private:

    char colour;
    char id;

public:
    piece()
    {
        colour = ' ';
        id = ' ';
    }

    void set_piece(char c, char i)
    {
        colour = c;
        id = i;
    }

    char get_colour()
    {
        return colour;
    }

    char get_id()
    {
        return id;
    }

    int absolute(int value)
    {
        if (value < 0)
        {
            return -value;
        }

        return value;
    }

    bool pawn_move(int sr, int sc, int dr, int dc)
    {
        int d;

        if (colour == 'W')
        {
            d = -1;
        }

        else
        {
            d = 1;
        }

        if (sc == dc && dr == sr + d)
        {
            return true;
        }

        if
        (
            sc == dc && absolute(dr - sr) == 2
        )
        {
            if (colour == 'W' && sr == 6)
            {
                return true;
            }

            if (colour == 'B' && sr == 1)
            {
                return true;
            }
        }

        return false;
    }

    bool rook_move(int sr, int sc, int dr, int dc)
    {
        return (sr == dr || sc == dc);
    }

    bool bishop_move(int sr, int sc, int dr, int dc)
    {
        return
        (
            absolute(dr - sr) == absolute(dc - sc)
        );
    }

    bool knight_move(int sr, int sc, int dr, int dc)
    {
        int row = absolute(dr - sr);
        int column = absolute(dc - sc);
        return
        (
            (row == 2 && column == 1) ||  (row == 1 && column == 2)
        );
    }

    bool queen_move(int sr, int sc, int dr, int dc)
    {
        return
        (
            rook_move(sr, sc, dr, dc) ||  bishop_move(sr, sc, dr, dc)
        );
    }

    bool king_move(int sr, int sc, int dr, int dc)
    {
        return
        (
            absolute(dr - sr) <= 1 &&  absolute(dc - sc) <= 1
        );
    }

    bool is_move_allowed(int sr, int sc, int dr, int dc)
    {
        if (id == 'P')
        {
            return pawn_move(sr, sc, dr, dc);
        }

        if (id == 'R')
        {
            return rook_move(sr, sc, dr, dc);
        }

        if (id == 'B')
        {
            return bishop_move(sr, sc, dr, dc);
        }

        if (id == 'N')
        {
            return knight_move(sr, sc, dr, dc);
        }

        if (id == 'Q')
        {
            return queen_move(sr, sc, dr, dc);
        }

        if (id == 'K')
        {
            return king_move(sr, sc, dr, dc);
        }

        return false;
    }
};

class board
{
private:

    piece chess_board[8][8];
    char current_turn;
    int last_pawn_double_row;
    int last_pawn_double_column;

public:

    board()
    {
        current_turn = 'W';
        last_pawn_double_row = -1;
        last_pawn_double_column = -1;

        initialize_board();
    }

    void initialize_board()
    {
        clear_board();
        place_black_pieces();
        place_white_pieces();
    }

    void clear_board()
    {
        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                chess_board[r][c].set_piece(' ', ' ');
            }
        }
    }

    void place_black_pieces()
    {
        char pieces[8] =
        {
            'R','N','B','Q',
            'K','B','N','R'
        };

        for (int c = 0; c < 8; c++)
        {
            chess_board[0][c].set_piece('B', pieces[c]);
            chess_board[1][c].set_piece('B', 'P');
        }
    }

    void place_white_pieces()
    {
        char pieces[8] =
        {
            'R','N','B','Q',
            'K','B','N','R'
        };

        for (int c = 0; c < 8; c++)
        {
            chess_board[7][c].set_piece('W', pieces[c]);
            chess_board[6][c].set_piece('W', 'P');
        }
    }

    char get_turn()
    {
        return current_turn;
    }

    void set_square_colour(bool light_square)
    {
        HANDLE h =
            GetStdHandle(STD_OUTPUT_HANDLE);

        if (light_square)
        {
            SetConsoleTextAttribute
            (
                h,

                BACKGROUND_RED |
                BACKGROUND_GREEN |
                BACKGROUND_BLUE |
                BACKGROUND_INTENSITY
            );
        }

        else
        {
            SetConsoleTextAttribute
            (
                h,

                BACKGROUND_INTENSITY
            );
        }
    }

    void reset_colour()
    {
        HANDLE h =
            GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute
        (
            h,

            FOREGROUND_RED |
            FOREGROUND_GREEN |
            FOREGROUND_BLUE
        );
    }

    void print_labels()
    {
        cout << endl;

        cout << "          ";

        for (char ch = 'a'; ch <= 'h'; ch++)
        {
            cout
                << "     "
                << ch
                << "     ";
        }

        cout << endl << endl;
    }

    void print_square(int r, int c, int l)
    {
        bool light_square = (r + c) % 2 == 0;
        set_square_colour(light_square);
        char display_piece = ' ';

        if (chess_board[r][c].get_id() != ' ')
        {
            if
            (
                chess_board[r][c].get_colour() == 'W'
            )
            {
                display_piece = tolower(chess_board[r][c].get_id());
                    
            }

            else
            {
                display_piece = chess_board[r][c].get_id();
            }
        }

        if (l == height / 2)
        {
            cout
                << "     "
                << display_piece
                << "     ";
        }

        else
        {
            for (int i = 0; i < width; i++)
            {
                cout << " ";
            }
        }

        reset_colour();
    }

    void print_board()
    {
        system("cls");

        print_labels();

        for (int r = 0; r < 8; r++)
        {
            for (int l = 0; l < height; l++)
            {
                if (l == height / 2)
                {
                    cout
                        << "   "
                        << 8 - r
                        << "   ";
                }

                else
                {
                    cout << "       ";
                }

                for (int c = 0; c < 8; c++)
                {
                    print_square(r, c, l);
                }

                cout << endl;
            }
        }

        cout << endl;
    }

    bool is_path_clear(int sr, int sc, int dr, int dc)
    {
        char piece_id = chess_board[sr][sc].get_id();

        if (piece_id == 'N')
        {
            return true;
        }

        int row_step = 0;
        int column_step = 0;

        if (dr > sr)
        {
            row_step = 1;
        }

        if (dr < sr)
        {
            row_step = -1;
        }

        if (dc > sc)
        {
            column_step = 1;
        }

        if (dc < sc)
        {
            column_step = -1;
        }

        int r = sr + row_step;
        int c = sc + column_step;

        while (r != dr || c != dc)
        {
            if (chess_board[r][c].get_id() != ' ')
            {
                return false;
            }

            r += row_step;
            c += column_step;
        }

        return true;
    }

    bool is_check(char king_colour)
    {
        int king_row = -1;
        int king_col = -1;

        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                if
                (
                    chess_board[r][c].get_id() == 'K' &&
                    chess_board[r][c].get_colour() == king_colour
                )
                {
                    king_row = r;
                    king_col = c;
                }
            }
        }

        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                if
                (
                    chess_board[r][c].get_colour() != king_colour &&
                    chess_board[r][c].get_colour() != ' '
                )
                {
                    if
                    (
                        chess_board[r][c].is_move_allowed(r,c,king_row,king_col)
                    )
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }
  bool has_valid_move(char colour)
{
    for (int sr = 0; sr < 8; sr++)
    {
        for (int sc = 0; sc < 8; sc++)
        {
            if
            (
                chess_board[sr][sc].get_colour() == colour
            )
            {
                for (int dr = 0; dr < 8; dr++)
                {
                    for (int dc = 0; dc < 8; dc++)
                    {
                    
                        if
                        (
                            chess_board[dr][dc].get_colour() != colour
                        )
                        {
                            
                            if
                            (
                                chess_board[sr][sc].is_move_allowed(sr,sc,dr,dc)
                            
                            )
                            {
                               
                                if
                                (
                                    is_path_clear(sr,sc,dr,dc)
                                   
                                )
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

  bool checkmate(char colour)
{
    if
    (
        is_check(colour) && !has_valid_move(colour)
    )
    {
        return true;
    }

    return false;
}
bool stalemate()
{
    if
    (
        !is_check(current_turn) && !has_valid_move(current_turn)
    )
    {
        return true;
    }
    return false;
}

    
    bool en_passant(int sr, int sc, int dr, int dc)
    {
        if
        (
            chess_board[sr][sc].get_id() == 'P'
        )
        {
            if
            (
                absolute(dc - sc) == 1 && chess_board[dr][dc].get_id() == ' '
            )
            {
                return true;
            }
        }

        return false;
    }

    int absolute(int value)
    {
        if (value < 0)
        {
            return -value;
        }

        return value;
    }

    void make_move(char selected_piece, string source, string destination)
    {
        int sr = 8 - (source[1] - '0');
        int sc = source[0] - 'a';
        int dr =8 - (destination[1] - '0');
        int dc =destination[0] - 'a';
        selected_piece = toupper(selected_piece);

        if (chess_board[sr][sc].get_id() == ' ')
        {
            cout << "No Piece Found!" << endl;
            return;
        }

        if (chess_board[sr][sc].get_colour() != current_turn)
        {
            cout << "Wrong Turn!" << endl;
            return;
        }

        if
        (
            chess_board[sr][sc].get_id() != selected_piece
        )
        {
            cout << "Wrong Piece!" << endl;
            return;
        }

        if
        (
            chess_board[dr][dc].get_colour() == current_turn
        )
        {
            cout << "Own Piece Blocking!" << endl;
            return;
        }

        if
        (
            !chess_board[sr][sc].is_move_allowed(sr,sc,dr,dc)
            
        )
        {
            cout << "Invalid Move" << endl;
            return;
        }

        if
        (
            !is_path_clear(sr,sc,dr,dc)
           
        )
        {
            cout << "Path Blocked!" << endl;
            return;
        }

        if (en_passant(sr, sc, dr, dc))
        {
            if (current_turn == 'W')
            {
                chess_board[dr + 1][dc].set_piece(' ', ' ');
            }

            else
            {
                chess_board[dr - 1][dc].set_piece(' ', ' ');
            }
        }

        chess_board[dr][dc] = chess_board[sr][sc];
        chess_board[sr][sc].set_piece(' ', ' ');

        if
        (
            chess_board[dr][dc].get_id() == 'P' &&
            absolute(dr - sr) == 2
        )
        {
            last_pawn_double_row = dr;
            last_pawn_double_column = dc;
        }

        else
        {
            last_pawn_double_row = -1;
            last_pawn_double_column = -1;
        }

        if (current_turn == 'W')
        {
            current_turn = 'B';
        }

        else
        {
            current_turn = 'W';
        }

        cout << endl;
        cout << "Move Successful!" << endl;

        if (is_check(current_turn))
        {
            cout << "CHECK!" << endl;
        }

        if (checkmate(current_turn))
        {
            cout << "CHECKMATE!" << endl;
        }

        if (stalemate())
        {
            cout << "STALEMATE!" << endl;
        }
    }
};

int main()
{
    board chess;
    string player1;
    string player2;
    string white_player;
    string black_player;

    char side;

    cout << endl;
    cout << "=============== WELCOME TO CHESS GAME ===============" << endl;
    cout << endl;

    cout << "Enter Player 1 Name : ";
    getline(cin, player1);

    cout << endl;

    cout << "Choose Side (White or Black0 : ";
    cin >> side;

    cin.ignore();
    cout << endl;
    cout << "Enter Player 2 Name : ";
    getline(cin, player2);

    if (side == 'W' || side == 'w')
    {
        white_player = player1;
        black_player = player2;
    }

    else
    {
        white_player = player2;
        black_player = player1;
    }

    bool game_running =
        true;

    while (game_running)
    {
        chess.print_board();
        cout << endl;

        if (chess.get_turn() == 'W')
        {
            cout
                << "Current Turn : "
                << white_player
                << " (WHITE)"
                << endl;
        }

        else
        {
            cout
                << "Current Turn : "
                << black_player
                << " (BLACK)"
                << endl;
        }

        cout << endl;

        char piece;
        string source;
        string destination;
        cout << "Enter Piece : ";
        cin >> piece;
        cout << endl;

        cout << "Enter Source Position : ";
        cin >> source;

        if (source == "Q" || source == "q")
        {
            game_running = false;
            break;
        }

        cout << endl;
        cout << "Enter Destination Position : ";
        cin >> destination;
        cout << endl;

        chess.make_move(piece,source,destination);
        cout << endl;
        system("pause");
    }
    
    system("cls");
    cout << endl;
    cout << "Game Ended. Thank  You For Playing" << endl;
    cout << endl;
    system("pause");
    return 0;
}