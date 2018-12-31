# Snake-in-C
snake game in C

## Introduction
Score will be (the length of snake - 1).

Initialization: the snake will run to left at first.
Termination: if the snake runs into the boundary or its own body, the game terminates.

optional feature: speed change
	I add a member variable "level" in the game struct. 
	The level is equal to (score/5+1).
	The speed is equal to (2000-(g->level-1)*10).
	The fastest speed is 300, and the slowest speed is 2000.
  
## Implementation
I used a linkedlist to store every position (x,y) of this snake's body. 
The game_tick function will draw the snake and apple, then call refresh function in the beginning and update the score.
When an apple is eaten, the snake will increase its length by one. And a new apple will show up randomly.

## Get started
compile the snake.c, then run it!
