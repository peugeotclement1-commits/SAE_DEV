#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* This function is used to handle code errors */

void handle_error_code(enum return_code code) {
    switch (code) {
        case OK:
            break;
		case OUT:
            printf("Error [OUT]: Coordinates are outside the grid.\n");
            break;
        case BUSY:
            printf("Error [BUSY]: The square is occupied or invalid.\n");
            break;
        case RULES:
            printf("Error [RULES]: The move or action does not follow the game rules.\n");
            break;
        case STAGE:
            printf("Error [STAGE]: Action not possible at this stage of the game.\n");
            break;
        default:
            printf("Unknown error (Error code : %d).\n", code);
            break;
    }
}

/* This function displays a menu before the game starts */

void menu(){
	printf("=================================\n");
    printf("       WELCOME IN THE GAME       \n");
    printf("     Press [ENTER] to play...    \n");
    printf("=================================\n");
    
    while (getchar() != '\n');
	
}

/* This function displays player stats */

void display_player_stats(board game, player current, char *player_name) {
    int nb_on_board = get_nb_pieces_on_board(game, current);
    int nb_eliminated = NB_INITIAL_PIECES - nb_on_board;

    printf("\n----------------------------------------\n");
    printf(" Statistics of %s :\n", player_name);
    printf(" - Pieces alive on the board : %d / 6\n", nb_on_board);
    printf(" - Pieces removed / in reserve : %d\n", nb_eliminated);
    printf("----------------------------------------\n");
}

/* This function displays the game. */

void print_board(board game_board){
	
    printf("\n    ");
    for (int j = 0; j < DIMENSION; j++) {
        printf(" %d  ", j);
    }
    printf("\n   -------------------------\n");

    for (int i = 0; i < DIMENSION; i++) {
        printf(" %d |", i);

        for (int j = 0; j < DIMENSION; j++) {
            int digit = get_digit(game_board, i, j);
            player holder = get_place_holder(game_board, i, j);

			if (holder == NO_PLAYER) {
                printf(" %d |", digit);
            } 
            else if (holder == NORTH) {
                char piece;
                if (is_king(game_board, i, j)) {
                    piece = 'K'; 
                } else {
                    piece = 'P'; 
                }
                printf(ANSI_COLOR_RED "%c:%d" ANSI_COLOR_RESET "|", piece, digit);
            } 
            else if (holder == SOUTH) {
                char piece;
                if (is_king(game_board, i, j)) {
                    piece = 'K'; 
                } else {
                    piece = 'P'; 
                }
                printf(ANSI_COLOR_BLUE "%c:%d" ANSI_COLOR_RESET "|", piece, digit);
            }
        }
        printf("\n   -------------------------\n");
    }
}

/* This function sets the game. */

void setup_phase(board game, char *north_name, char *south_name){
	printf("=== WELCOME INTO THE GAME (SAÉ 1.1) ===\n");
    printf("Name of north player : ");
    scanf("%49s", north_name);
    printf("Name of south player : ");
    scanf("%49s", south_name);
	
	printf("\n--- PLACEMENT STAGE ---\n");
    while (piece_to_place(game) != NONE) {
        print_board(game);

		player current = current_player(game);
		type p_type = piece_to_place(game);
		char *current_name;

		if (current == NORTH) {
			current_name = north_name;
		} else {
			current_name = south_name;
		}


		char *piece_name;

		if (p_type == KING) {
			piece_name = "KING";
		} else {
			piece_name = "PAWN";
		}

		printf("\n%s turn to place his %s.\n", current_name, piece_name);

        int line, col;
        printf("Placement line : ");
        if (scanf("%d", &line) != 1) {
			 while(getchar() != '\n'); 
			 continue; 
		}
        printf("Placement column : ");
        if (scanf("%d", &col) != 1) { 
			while(getchar() != '\n'); 
			continue; 
		}
        enum return_code res = place_piece(game, line, col);
        if (res != OK) {
            handle_error_code (res);
        } else {
            printf("Piece successfully placed !\n");
        }
    }
     printf("\n--- END OF PLACEMENT STAGE, START OF THE GAME ! ---\n");
}

/* This function checks if the current player has a playable piece on a prescribed figure. */

int has_prescribed_move(board game, player current) {

    int prescribed = get_prescribed_move(game);

    if (prescribed <= 0) {
        return 1;
    }

    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {

            if (get_place_holder(game, i, j) == current &&
                get_digit(game, i, j) == prescribed &&
                is_legal_move(game, i, j)) {

                return 1;
            }
        }
    }

    return 0;
}


/* This function displays the pieces current player can play with */

void display_legal_pieces(board game, player current) {

    int found = 0;

    printf("\n--- PIECES AVAILABLE TO PLAY ---\n");

    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {

            if (get_place_holder(game, i, j) == current && is_legal_move(game, i, j)) {

                found = 1;

                if (is_king(game, i, j)) {
                    printf("King at line %d - column %d : %d steps\n", i, j, get_digit(game, i, j));
                } else {
                    printf("Pawn at line %d - column %d : %d steps\n", i, j, get_digit(game, i, j));
                }
            }
        }
    }
	
	print_board(game);
	
    if (!found) {
        printf("No piece can currently move.\n");
    }
}

/* This function displays the winner of the game */

void display_winner(board game, char *north_name, char *south_name){
	print_board(game);
	player winner = get_winner(game);
	char *winner_name;
	if(winner == NORTH){
		winner_name=north_name;
	} else if(winner == SOUTH){
		winner_name=south_name;
	}
	printf("\nEnd of game ! The opposing king was captured !\n");
	printf("THE WINNER IS : %s !!!!!!\n", winner_name);
}


/* This is the main function of the game */

void game(){
	board game = new_game();
    char north_name[50];
    char south_name[50];
	
	setup_phase(game, north_name, south_name);
    

    while (get_winner(game) == NO_PLAYER) {
        print_board(game);


        player current = current_player(game);        
		char *current_name;
		
		if (current == NORTH) {
			current_name = north_name;
		} else {
			current_name = south_name;
		}
		
		display_player_stats(game, current, current_name);
		
		int prescribed = get_prescribed_move(game);
		
        printf("\nIt's now %s 's turn.\n", current_name);
        if (prescribed > 0) {
            printf("Prescribed figure : %d\n", prescribed);
        } else {
            printf("First move: you can choose any piece..\n");
        }

        int derogation = 0;
        if (prescribed > 0 && !has_prescribed_move(game, current)) {
			derogation = 1;
			printf("\n--- SPECIAL MOVE: DEROGATION ---\n");
            printf("No piece can move from a square displaying %d.\n", prescribed);
        }

        display_legal_pieces(game, current);

        int choice = 1;

        int nb_pieces = get_nb_pieces_on_board(game, current);

        if (derogation && nb_pieces < NB_INITIAL_PIECES) {
			printf("\nYou have a captured piece that may be reintroduced.\n");
            printf("Would you like to:\n");
            printf("(1) Move one of your pieces\n");
            printf("(2) Reintroduce a captured pawn\n");
            printf("Your choice: ");

            if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {

                printf("Invalid choice. Try again.\n");
                while (getchar() != '\n');
                continue;
            }
        }

        if (choice == 2 && derogation && nb_pieces < NB_INITIAL_PIECES) {

            int line, col;

            printf("\n--- PAWN REINTRODUCTION ---\n");
            printf("The pawn must be placed on an empty square displaying %d.\n", prescribed);

            print_board(game);

            printf("Reintroduction line : ");
            if (scanf("%d", &line) != 1) {
                while (getchar() != '\n');
                continue;
            }

            printf("Reintroduction column : ");
            if (scanf("%d", &col) != 1) {
                while (getchar() != '\n');
                continue;
            }

            enum return_code ins_res = insert_pawn(game, line, col);

            if (ins_res != OK) {
                printf("Reintroduction impossible -> ");
                handle_error_code(ins_res);
                printf("Try again.\n");
                continue;
            }

            printf("Pawn successfully reintroduced !\n");
            continue;
        }

        int line, col;

        printf("\nPiece to play line : ");

        if (scanf("%d", &line) != 1) {
            while (getchar() != '\n');
            continue;
        }

        printf("Piece to play column : ");

        if (scanf("%d", &col) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (!is_legal_move(game, line, col)) {
            printf("This piece cannot make a complete legal move.\n");
            printf("Choose another piece from the available pieces.\n");
            continue;
        }

        enum return_code sel_res = select_piece(game, line, col);

        if (sel_res != OK) {
            printf("Refused selection -> ");
            handle_error_code(sel_res);
            continue;
        }

        int steps = get_digit(game, line, col);
        printf("The piece shall move of %d steps.\n", steps);

        int move_success = 1;
        for (int s = 0; s < steps; s++) {
            char dir_char;
            printf("Step %d/%d - Direction (N = Nord, S = Sud, E = Est, W = West) : ", s + 1, steps);
            scanf(" %c", &dir_char);

            direction d;
            if (dir_char == 'N' || dir_char == 'n') d = N;
            else if (dir_char == 'S' || dir_char == 's') d = S;
            else if (dir_char == 'E' || dir_char == 'e') d = E;
            else if (dir_char == 'W' || dir_char == 'w') d = W;
            else {
                printf("Unable direction.\n");
                move_success = 0;
                break;
            }

            enum return_code step_res = move_one_step(game, d);
            if (step_res != OK) {
                printf("Impossible move at step %d -> ", s + 1);
                handle_error_code(step_res);
                move_success = 0;
                break;
            }
        }


        if (!move_success) {
            cancel_move(game);
            printf("Canceled move. Try again.\n");
        } else {
            printf("Move successfully played !\n");
        }
    }
    
	display_winner(game, north_name, south_name);
	destroy_game(game);
}


int main(void){
	menu();
	game();
	return 0;
}
