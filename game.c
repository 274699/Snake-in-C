// TODO: Complete this file

#include <stdlib.h>
#include "screen.h"
#include "game.h"

typedef struct body_t body_t;

struct body_t{
	unsigned int x;
	unsigned int y;
	body_t*next;
};

struct game_t{
	int hor;
	int ver;
	unsigned int applex;
	unsigned int appley;
	body_t* head;
	unsigned int len;
	unsigned int score;
	unsigned int level;
	screen_t *scr;
};


/// Called to initialize internal game structures.
/// Your game should not use global variables, but should associate all state
/// with the pointer returned by game_init. This pointer will be passed to the
/// opther game functions.
///
/// A pointer to the screen to use for output/input (see screen.h) is passed.
/// The game does not own this pointer, i.e. it should not free it when the
/// game is destroyed.
game_t * game_init (screen_t * scr){
	if(scr==NULL)
		return NULL;
	game_t * res=(game_t *)malloc(sizeof(game_t));
	res->hor=-1;
	res->ver=0;
	unsigned int row=screen_getmaxx(scr);
	unsigned int col=screen_getmaxy(scr);
	//generate the initialization apple
	res->applex=rand()%(row-2)+1;
	res->appley=rand()%(col-2)+1;

	res->head=(body_t *)malloc(sizeof(body_t));
	//generate the initialization head
	//it can not be the same with the apple
	unsigned int x=rand()%(row-2)+1;
	unsigned int y=rand()%(col-2)+1;
	while(x==res->applex){
		x=rand()%(row-2)+1;
	}
	while(y==res->appley){
		y=rand()%(col-2)+1;
	}

	res->head->x=x;
	res->head->y=y;
	res->len=1;
	res->scr=scr;
	res->score=0;
	res->level=1;
	return res;
}

/// Called when the game is over. Free any internal state here.
void game_done (game_t * g){
	if(g==NULL)
		return;
	//free body
	body_t * temp=g->head;
	for(unsigned int i=0;i<g->len;i++){
		body_t*temp2=temp->next;
		free(temp);
		temp=temp2;
	}
	free(g);
}

/// Called when a key was pressed; hor and ver indicate which key according to
/// the table below:
///  
///   key     hor    ver
///  left     -1     0
///  right     1     0
///  up        0    -1
///  down      0     1
///
/// Return 0 if delay before calling game_tick again should remain unmodified
/// If non-zero, the return
/// value is taken as the delay before game_tick should be called again
/// (ignoring any remaining delay that remained before the key was pressed)
unsigned long game_key(game_t * g, int hor, int ver){
	if(g->hor==hor&&g->ver==ver)
		return 0;
	g->hor=hor;
	g->ver=ver;
	//refresh instantly
	return 1;
}

/// Called at regular intervals; The return code should be how long (in ms) 
/// to sleep until this function should be called again, or 0 if the game is
/// over.
unsigned long game_tick (game_t * g){
	screen_clear (g->scr);
	screen_update_score (g->scr, g->score, 0);
	//draw snake's body
	body_t *temp=g->head;
	for(unsigned int i=0;i<g->len;i++){
		screen_set (g->scr,temp->x,temp->y,SCREEN_CHAR_SNAKE);
		temp=temp->next;
	}
	//draw apple
	screen_set (g->scr,g->applex,g->appley,SCREEN_CHAR_SNAKE);
	screen_refresh (g->scr);
	
	//get the position of head
	unsigned int x=g->head->x;
	unsigned int y=g->head->y;
	
	//get the boundary
	unsigned int row=screen_getmaxx(g->scr);
	unsigned int col=screen_getmaxy(g->scr);

	//snake goes ahead
	//if it comes to the boundary, return 0
	if(x==row-1||x==0)
		return 0;
	if(y==col-1||y==0)
		return 0;
	
	//get the direction
	int hor=g->hor;
	int ver=g->ver;
	
	//decide next position
	body_t *newhead=(body_t*)malloc(sizeof(body_t));
	if(hor==-1&&ver==0){
		newhead->x=x-1;
		newhead->y=y;
	}else if(hor==1&&ver==0){
		newhead->x=x+1;
		newhead->y=y;
	}else if(hor==0&&ver==-1){
		newhead->x=x;
		newhead->y=y-1;
	}else if(hor==0&&ver==1){
		newhead->x=x;
		newhead->y=y+1;
	}
	
	//if it eats its body
	if(screen_get (g->scr, newhead->x, newhead->y)==SCREEN_CHAR_SNAKE){
		free(newhead);
		return 0;
	}
	
	//add the new head
	newhead->next=g->head;
	g->head=newhead;
	//doesn't eat apple
	if(newhead->x!=g->applex||newhead->y!=g->appley){
		//delete the last node
		//if the length is 1
		if(g->len==1){
			temp=newhead;
		}else{
			temp=newhead->next;
			for(unsigned int i=0;i<g->len-2;i++){
				temp=temp->next;
			}
		}
		free(temp->next);
		temp->next=NULL;		
	}else{
		//eat apple
		//doesn't delete the last node
		g->len++;
		//generate new apple
		g->applex=rand()%(row-2)+1;
		g->appley=rand()%(col-2)+1;
		while(screen_get (g->scr, g->applex, g->appley)==SCREEN_CHAR_SNAKE){
			g->applex=rand()%(row-2)+1;
			g->appley=rand()%(col-2)+1;
		}
		g->score++;
	}
	//get its level;
	//score 0-4: level1
	//score 5-9: level2
	//...
	g->level=(g->score)/5+1;
	unsigned long time=2000-(g->level-1)*10;
	//the fastest speed is 300
 	if(time<300)
		time=300;
	return time;
}
