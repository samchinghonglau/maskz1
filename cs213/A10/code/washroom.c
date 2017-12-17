#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

#define MAX_OCCUPANCY      3
#define NUM_ITERATIONS     100
#define NUM_PEOPLE         20
#define FAIR_WAITING_COUNT 4

/**
 * You might find these declarations useful.
 */
enum GenderIdentity {MALE = 0, FEMALE = 1};
const static enum GenderIdentity otherGender [] = {FEMALE, MALE};

struct Washroom {
  // TODO
  uthread_mutex_t mutex;
  uthread_cond_t hasspace;
  uthread_cond_t notice;
  uthread_cond_t waittoleave;
  enum GenderIdentity g;
  int force;
  int num_people;
  int triggerwarning;
  
  
};

struct Washroom* createWashroom() {
  struct Washroom* washroom = malloc (sizeof (struct Washroom));
  washroom->mutex = uthread_mutex_create();
  washroom->notice = uthread_cond_create(washroom->mutex);
  washroom->hasspace = uthread_cond_create(washroom->mutex);
  washroom->waittoleave = uthread_cond_create(washroom->mutex);
  washroom->force = 0;
  washroom->num_people = 0;
  washroom->triggerwarning = 0;
  // TODO
  return washroom;
}

struct Washroom* washroom;

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram         [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_mutex_t waitingHistogrammutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

void enterWashroom (enum GenderIdentity g) {
  // TODO
  washroom->num_people++;
  occupancyHistogram [g] [washroom->num_people]++;
  for (int i = 0; i < NUM_PEOPLE; i++){
  uthread_mutex_unlock(washroom->mutex);
	uthread_yield();
  uthread_mutex_lock(washroom->mutex);
  }
}

void leaveWashroom() {
  // TODO
  washroom->num_people--;
  for (int i = 0; i < NUM_PEOPLE; i++){
  uthread_mutex_unlock(washroom->mutex);
	uthread_yield();
  uthread_mutex_lock(washroom->mutex);
  }
  
  
}

void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock (waitingHistogrammutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_mutex_unlock (waitingHistogrammutex);
}

//
// TODO
// You will probably need to create some additional produres etc.
//

void* go(void* v){
	enum GenderIdentity g = random() % 2;
	int counter = 0;
	int startcount = 0;
	int finishcount = 0;
	for (int i = 0; i < NUM_ITERATIONS; i++){
		
		uthread_mutex_lock(washroom->mutex);
		if (washroom->num_people != 0 && washroom->g != g){
			
			uthread_cond_wait(washroom->notice);
		}
		if (washroom->force && washroom->num_people != 0){
			
			uthread_cond_wait(washroom->waittoleave);
			washroom->force = 0;
		}
		startcount = counter;
		while (washroom->num_people >= MAX_OCCUPANCY){
			uthread_cond_wait(washroom->hasspace);
			counter++;
		}
		
		if (washroom->num_people == 0){
			washroom->g = g;
		}
		
		finishcount = counter;
		enterWashroom(g);
		leaveWashroom();
		if (washroom->num_people == 0)
			uthread_cond_broadcast(washroom->waittoleave);

		recordWaitingTime(finishcount-startcount);
		uthread_cond_signal(washroom->hasspace);
		
		washroom->triggerwarning++;
		if (washroom->triggerwarning >= FAIR_WAITING_COUNT || washroom->num_people == 0){
				uthread_cond_broadcast(washroom->notice);
				washroom->triggerwarning = 0;
				washroom->force = 1;
			}
		
		uthread_mutex_unlock(washroom->mutex);
		
		
	}
	return NULL;
}

int main (int argc, char** argv) {
  uthread_init (1);
  washroom = createWashroom();
  uthread_t pt [NUM_PEOPLE];
  waitingHistogrammutex = uthread_mutex_create ();

  // TODO
  for (int i = 0; i < NUM_PEOPLE; i++){
	  pt[i] = uthread_create(go,washroom);
	  
  }
  for (int i = 0; i < NUM_PEOPLE; i++){
	  uthread_join(pt[i],0);
  }
  
  printf ("Times with 1 person who identifies as male   %d\n", occupancyHistogram [MALE]   [1]);
  printf ("Times with 2 people who identifies as male   %d\n", occupancyHistogram [MALE]   [2]);
  printf ("Times with 3 people who identifies as male   %d\n", occupancyHistogram [MALE]   [3]);
  printf ("Times with 1 person who identifies as female %d\n", occupancyHistogram [FEMALE] [1]);
  printf ("Times with 2 people who identifies as female %d\n", occupancyHistogram [FEMALE] [2]);
  printf ("Times with 3 people who identifies as female %d\n", occupancyHistogram [FEMALE] [3]);
  printf ("Waiting Histogram\n");
  for (int i=0; i<WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Number of times people waited for %d %s to enter: %d\n", i, i==1?"person":"people", waitingHistogram [i]);
  if (waitingHistogramOverflow)
    printf ("  Number of times people waited more than %d entries: %d\n", WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}
