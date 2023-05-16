#include <usart.h>
#include <string.h>

#define PART2

#define MAX 7
#define SEVENFOLD(number) (number*7)

void printArray(int array[]) {
  printString("Content of array:\n");
  for(int i = 0; i < MAX; i++) {
    printf("\tAddress: %p has value: %d\n", &array[i], array[i]);
  }
}

void makeArray(int array[]) {
  for(int i = 0; i < MAX; i++) {
    array[i] = SEVENFOLD(i);
  }
}

// --- PART 2 ---

void printFirstLetters(char array[MAX][10]) {
  printString("First letters: ");
  for(int i = 0; i < MAX; i++) {
    printf("%c", array[i][0]);
  }
  printString("\n");
}

void printLastLetters(char array[MAX][10]) {
  printString("Last letters: ");
  for(int i = 0; i < MAX; i++) {
    int len = strnlen(array[i], 10);
    printf("%c", array[i][len-1]);
  }
  printString("\n");
}

char* searchShortestName(char array[MAX][10]) {
  char* smallest = array[0];
  for(int i = 1; i < MAX; i++) {
    if(strnlen(array[i], 10) < strnlen(smallest, 10)) smallest = array[i];
  }
  return smallest;
}

int main() {
  initUSART();

  #ifndef PART2
  int array[MAX] = {};

  printArray(array);
  makeArray(array);
  printArray(array);
  #endif

  #ifdef PART2
  char names[MAX][10] = {"Roman", "Matt", "Abel", "Daniil", "Boldi", "Jasper", "Artem"};

  printFirstLetters(names);
  printLastLetters(names);
  char* smallest = searchShortestName(names);
  printf("Shortest = %s", smallest);
  #endif
}