#include <string.h>
#include "hangman-io.h"
#include "words.h"
#include "cs136.h"

// to_upper_case(c) gets a char pointer modifies it to the
// upper_case char
// modifies: *c to uppercase
// time: O(1)
void to_upper_case(char *c){
  if(('a' <= *c && *c <= 'z')){
      *c = *c - 32;
    }
}

// start_game(game_no) gets a int pointer outputs a message to get a
// number between 1 and 1000 modifies it by assigning the input value
// requires: 1<= input value <= 1000
// modifies: *game_no to the input value between 1 and 1000
// side effects: prints messages
// time: O(n) where n is the number of times user inputed wrong value
void start_game(int *game_no){
  while(true){
    printf("Please enter a valid game number between 1 and 1000.\n");
    int x = scanf(" %d", game_no);
    if(x != 1){
      break;
    }
    if(!(1 <= *game_no && *game_no <= max_words)){
      printf("Error, the number entered was not valid.\n");
    }else{
      break;
    }
  }
  
}

// end_game() ask user to play again or not? user enters y/Y or n/N
// either uppercase or lowercase returns true if game should ended
// else returns false
// side effects: prints messages
// time: O(n) where n is the number of times user inputed wrong value
bool end_game(){
  while(true){
    printf("Do you want to play again? (Y for yes, N for no).\n");
    char c = 0;
    int x = scanf(" %c", &c);
    if(x != 1){
      return true;
    }
    if('Y' == c || 'y' == c){
      return false;
    }else if('N' == c || 'n' == c){
      return true; 
    }else{
      printf("Error, invalid response.\n");
    }
  }
}

// array_equals(a, b, len) gets two char arrays and an integer which
// is the array length if both arrays are same then it returns true
// else false
// time: O(n) where n is len
bool array_equals(const char a[], const char b[], const int len){
  for(int i = 0; i < len; i++){
    if(a[i] != b[i]){
      return false;
    }
  }
  return true;
}

// hang_man(c, print, original, len, progress, game endded, guesses,
// guess, game result) gets a char pointer 2 char arrays, and their 
// length as integer, integer pointer to the game_ended, char array 
// for previos guesses, and a integer pointer for the result of the
// game and makes the hangman played by the rules
// side effects: prints messages
// modifies: game_ended, game_result, progress, print
// time: O(n) where n is len or guess whichever is greater
void hang_man(const char *c, char print[], const char original[], int len, int *progress, bool *game_ended, const char guesses[], const int guess, int *game_result){
  // vairables to keep track of game
  bool match = false;
  bool already_in = false;
  
  // checks whether the given char is previously guessed or not
  for(int i = 0; i < guess; i++){
    if(guesses[i] == *c){
      already_in = true;
    }
  }
  if(already_in == true){
    printf("You have already guessed the letter %c. Please enter another letter.\n", *c);
  }else{
    // if not guessed checks whether the char is in the word
    for(int i = 0; i < len; i++){
      if(*c == original[i]){
        print[i] = original[i];
        match = true;
      }
    }
    if(match == false){
      printf("The letter %c is not in the word.\n", *c);
      int dummy = *progress;
      if(dummy + 1 == max_strikes){
        *game_result = 0;
        *progress = max_strikes;
        *game_ended = true;
      }else{
        *progress = dummy + 1;
        int x = max_strikes - *progress;
        printf("Watch out! You only have %d more guesses left before your man is hung!\n", x);
      }
    }else{
      if(array_equals(print, original, len)){
        *game_result = 1;
        *game_ended = true;
      }
    }
  }
}

// print_str(a, len) gets a char array and its length prints the chars
// in the same line then prints a new line
// time: O(n) where n is len
// side effects: prints chars
void print_str(const char a[], int len){
  for(int i = 0; i < len; i++){
    printf("%c", a[i]);
  }
  printf("\n");
}

// play_game(game_ended) gets a boolean pointer to indicate whether
// the game ended or not plays the hangman game by letting user input
// values
// time: O(1) till the maximum allowable attempt
// side effects: prints messages expects inputs
void play_game(bool *game_ended){
  // variables to keep track of progess in game
  // progress of the hangman
  int progress = 0;
  // word number
  int game_no = 0;
  // whether the game ended or not
  *game_ended = false;
  // -1: game is running 0: loss 1: win
  int game_result = -1;
  // starting sequence
  start_game(&game_no);
  
  // picks a word
  const char *string = get_word(game_no);
  // copies the word to a dummy char pointer
  const char *dummy = string;
  
  // char arrays to represent answer and word so far
  char original[50];
  char print[50]; // char array which will be printed
  char guesses[50]; // previous attemtps
  int guess = 0; // number of attemtps so far
  // lenght of the word
  int len = 0;
  
  // fills the char arrays
  while(*dummy){
    original[len] = *dummy;
    print[len] = '*';
    len++;
    dummy++;
  }
  
  
  while(true){
    // checks whether the game ended or not
    if(game_ended){
      // checks whether it's a win or not
      if(game_result == 1){
        printf("You win! The correct word was %s.\n", string);
        break;
      }else if(game_result == 0){
        print_hangman(progress);
        printf("Game over. The correct word was %s.\n", string);
        break;
      }
    }
    
    print_hangman(progress);
    printf("Please enter a letter.\n");
    print_str(print, len);
    
    char c;
    int x = scanf(" %c", &c);
    // if the input ends breaks the loop
    if(x != 1){
      break;
    }
    // change the character to upper case
    if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')){
      to_upper_case(&c);
      // calls a funtion to modify the print char array
      hang_man(&c, print, original, len, &progress, game_ended, guesses, guess, &game_result);
      // adds the input char to the previous guesses
      guesses[guess] = c;
      guess++;
    }else{
      printf("The character %c is not a letter.\n", c);
    }
  }
}

int main(void) {
  bool game_ended = false;
  while(1){
    while(!game_ended){
      play_game(&game_ended);
    }
    bool x = end_game();
    if(x == false){
      game_ended = false;
    }else{
      game_ended = true;
      break;
    }
  }
}
