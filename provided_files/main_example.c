#include <stdio.h>
#include "board.h"

/**
 * @file main_example.c
 * @brief a simple illustration of how to include and use board.h and board.o.
 * @author Dorbec
 */
int main(int args, char **argv){
  board game = new_game();
  printf("Un plateau est créé.\n");
  enum return_code result = place_piece(game, 1, 2); 
  if(result == OK){
    printf("première pièce de Nord placée.\n");
  }
  else{
    printf("Aïe, pas réussi à placer une pièce.\n");
  }
  printf("J'essaye de la bouger : \n");
  enum return_code result2 = select_piece(game, 1, 2); 
  if(result2 == STAGE){
    printf("Il est trop tôt pour la bouger.\n");
  }
  else{
    printf("Bizarre.\n");
  }
  // à vous de jouer
  // vous pouvez utiliser des fonctions intermédiaires ;-)
  destroy_game(game);
  printf("suppression du plateau et sortie\n");	
  return 0;
}


