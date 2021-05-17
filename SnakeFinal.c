#include <ncurses.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <time.h>

#define MAXWIDTH COLS-1
#define MAXHEIGHT LINES-1

int TrophyNumber;
int speed;
int count;

// By: Mariah Lopez-----------------------------------------------------------------------------------------------------------
int kbhit (void) {
  /* checks if key has been hit or not */
  struct timeval tv; //If the timeout argument is not a NULL pointer, it points to an object of type struct timeval that specifies a maximum interval to wait for the selection to complete.
  fd_set read_fd; //If the readfds argument is not NULL, it points to an object of type fd_set 
                  //that on input specifies the file descriptors to be checked for being ready to read, and on output indicates which file descriptors are ready to read.
  tv.tv_sec=0;
  tv.tv_usec=0;
  FD_ZERO(&read_fd);//Initializes the file descriptor set fdset to have zero bits for all file descriptors.
  FD_SET(0,&read_fd);// // FD_SET:Sets the bit for the file descriptor fd in the file descriptor set fdset.
  if(select(1, &read_fd, NULL, NULL, &tv) == -1)
    return 0;
  if(FD_ISSET(0,&read_fd)) return 1;
  return 0;
}
// ----------------------------------------------------------------------------------------------------------------------------
// By:India Trotman------------------------------------------------------------------------------------------------------------
// Method that checks if snake touches the border
int wallcheck(int *snakeArr, int snakeSize) {
 
  int x = *snakeArr;  // head of snake
  int y = *(snakeArr+1); // body of snake

  if (x < 0 || x > MAXWIDTH) {
    return 1;
  }
  else if (y < 0 || y > MAXHEIGHT) {
    return 1;
  }
  if (snakeSize== ((MAXWIDTH + MAXHEIGHT)/2)){
       return 1;
  }

   return 0;
}
// ----------------------------------------------------------------------------------------------------------------------------
// By: Kaiser -----------------------------------------------------------------------------------------------------------------
// method in charge of the movement of the snake inside the border as well as its speed 
void SnakeMovement(int *snakeArr, int directionX, int directionY, int speed, int snakeSize) {

  int prevX = 0; // initial position
  int prevY = 0; //
  int currentX = *snakeArr;
  int currentY = *(snakeArr+1);
  int newX = currentX + directionX;
  int newY = currentY + directionY;

  *(snakeArr) = newX;
  *(snakeArr+1) = newY;
  mvprintw(newY, newX, "S");

  for (size_t i = 2; i < snakeSize*2; i += 2) {
    prevX = currentX;
    prevY = currentY;
    currentX = *(snakeArr+i);
    currentY = *(snakeArr+i+1);
    newX = prevX;
    newY = prevY;
    *(snakeArr+i) = newX;
    *(snakeArr+i+1) = newY;

    mvprintw(newY, newX, "S");
  }

  if (directionY != 0) {
    usleep(500000/speed);
  }

  usleep(1000000/speed); // default speed of snake in border when game commences
}
// -------------------------------------------------------------------------------------------------------------------------------
// By:Sebastian-------------------------------------------------------------------------------------------------------------------
// this method will create a trophy in a random coordinate inside the borders of the game
void Trophy(int* TrophyX, int* TrophyY, int *TrophyEaten) { 
    int snumber;
    time_t t;
    srand((unsigned) time(&t)); // used for the random number generator
        if (*TrophyEaten) { // if the trophy is not eaten then the trophy will appear in a random coordinate
            *TrophyX = (rand() % MAXWIDTH)+3;
            *TrophyY = (rand() % MAXHEIGHT)+3;
            *TrophyEaten = 0;
        }
                snumber = (rand()%10)+1; // generates a number between 1 -10 which will be the trophy value
                 
  TrophyNumber= snumber; // used for the snake size increment

    for(int expire=(rand()%10); expire>0; expire--){   // makes trophy disappear after random time
       mvprintw(*TrophyY, *TrophyX-1," %d ", snumber); // prints the trophy in the random coordinates 
       TrophyEaten=0;    
       }
 
}
// ----------------------------------------------------------------------------------------------------------------------------------
// By:India Trotman------------------------------------------------------------------------------------------------------------------
void trophyReached(int *snakeArr, int TrophyX, int TrophyY,
              int *TrophyEaten, int *snakeSize) {
  int x = *snakeArr;
  int y = *(snakeArr+1);
  if (x == TrophyX && y == TrophyY) {
    *TrophyEaten = 1;    
    *snakeSize += TrophyNumber;
    int snakeLen = *snakeSize;
  }

  mvprintw(0,0,"snakeSize: %d", *snakeSize);
}
// ------------------------------------------------------------------------------------------------------------------------------------
int main() {
  
// By: Kaiser--------------------------------------------------------------------------------------------------------------------------
  int snumber;
  int number;
  int snakeArr[100][2];
  for (size_t i = 0; i < 100; i++) {
    for (size_t j = 0; j < 2; j++) {
      snakeArr[i][j] = 0;
    }
  }
   time_t t; 
  srand((unsigned) time(&t)); 
  int r = (rand() % 30)+3; /*By:Mariah Lopez - allows the snake to appear in random spots on the left side of the game window*/  
  snakeArr[0][0] = r;
  snakeArr[0][1] = r;
  int keyPressed = 0;   /* which key user pressed */
  int directionX = 1;        /* direction xy */
  int directionY = 0;
  int speed = 20;       /* controls speed of the snake */
  int TrophyX = 0;      /* current Trophy xy position */
  int TrophyY = 0;
  int TrophyEaten = 1;   /* is Trophy eaten? */
  int snakeSize = 3;  /* player snakeSize */
  initscr();			/* Start curses mode */
  curs_set(false);
  noecho();
// ----------------------------------------------------------------------------------------------------------------------------------------
// By: Sebastian --------------------------------------------------------------------------------------------------------------------------
  while (!wallcheck(&snakeArr[0][0], snakeSize)) {  // this while loop is responsible for creating the border of the game
// the border will stop once the snake hits the wall 
  erase();
   
        for (int i=0; i<LINES-1; i++) // creates a verticle line starting from the left-upper hand corner ending in the lower-left hand corner
            {
                move(i,0);
                addstr("|");

            }

        for (int i=0; i<COLS-1; i++) //creates a horizantle line starting from the top left-upper hand corner and ending int right upper hand corner
            {
                move(0,i);
                addstr("-");
                
            }

        for (int i=2; i<COLS-1; i++)  // creates a horizantle line starting from the lower-left hand corner and ending in the lower right hand corner
            {
                move(LINES-1,i);
                addstr("-");
                
            }

        for (int i=0; i<LINES-1; i++) // creates a verticle line starting from the top right hand corner and ending in the lower right hand corner
            {
                move(i, COLS-1);
                addstr("|");
                
            }
  //By:India Trotman---------------------------------------------------------------------------        
    Trophy(&TrophyX, &TrophyY, &TrophyEaten); // runs the trophy method
    trophyReached(&snakeArr[0][0], TrophyX, TrophyY, &TrophyEaten, &snakeSize); // runs the method if trophy is eaten 
    if (TrophyEaten==1){ // if the snake eats a trophy
        speed+=2; // increase the speed by 2 everytime
         count ++; // increases the count of the amount of trohies eaten
    }
    mvprintw(0,20,"Trophies eaten: %d", count); // prints out the trophy eaten count
    SnakeMovement(&snakeArr[0][0], directionX, directionY, speed, snakeSize); // runs the snake method
// -----------------------------------------------------------------------------------------------------------
//By: Mariah Lopez-Gets arrows key inputs from the user & stops the snake from going in reverse ---------------
    refresh();
    keypad(stdscr, TRUE);
    if (kbhit()) {
      keyPressed = getch();

        if (keyPressed == KEY_UP  && !(directionY == 0 && directionX == 1)) {
                directionY--;
                directionX = 0;
            }
            if (keyPressed ==KEY_DOWN   && !(directionY == 0 && directionX == -1)) {
                directionY++;
                directionX = 0;
            }
            if (keyPressed == KEY_LEFT  && !(directionY == 0 && directionX == 1)) {
                directionY = 0;
                directionX = -1;
            }
            if (keyPressed == KEY_RIGHT && !(directionY == 0 && directionX == -1)) {
                directionY = 0;
                directionX = 1;
      }
    
    }
  }
  clear();
  mvprintw((MAXHEIGHT+1)/2,(MAXWIDTH+1)/2,"Game Over!! Your Snake Size is: %d , Trophies Eaten: %d", snakeSize,count);
  refresh();
	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */

	return 0;
}
// ----------------------------------------------------------------------------------