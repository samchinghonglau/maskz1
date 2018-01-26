#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"
#include "spinlock.h"

#define MAX_ITEMS 10
const int NUM_ITERATIONS = 200;
const int NUM_CONSUMERS  = 2;
const int NUM_PRODUCERS  = 2;

int producer_wait_count;     // # of times producer had to wait
int consumer_wait_count;     // # of times consumer had to wait
int histogram [MAX_ITEMS+1]; // histogram [i] == # of times list stored i items
int items = 0;
uthread_mutex_t mx;
uthread_cond_t have_items;
uthread_cond_t need_items;

void* producer (void* v) {
	for (int i=0; i<NUM_ITERATIONS; i++) {
		uthread_mutex_lock(mx);
		// TODO
		while (items == MAX_ITEMS){
			uthread_cond_wait(need_items);
		}
		assert (0 <= items && items <= MAX_ITEMS);		
		items++;
		histogram[items]++;
		uthread_cond_signal (have_items);	
		uthread_mutex_unlock(mx);
	}
	
  return NULL;
}

void* consumer (void* v) {
	
	for (int i=0; i<NUM_ITERATIONS; i++) {
		// TODO
		uthread_mutex_lock(mx);
		while(items == 0) {
			uthread_cond_wait(have_items);
		}
		assert (0 <= items && items <= MAX_ITEMS);
		items--;
		histogram[items]++;
		uthread_cond_signal(need_items);
		uthread_mutex_unlock(mx);
	}
	
  return NULL;
}

int main (int argc, char** argv) {
  mx = uthread_mutex_create();
  need_items = uthread_cond_create(mx);
  have_items = uthread_cond_create(mx);
  uthread_t t[4];
  uthread_init (4);
  t[0] = uthread_create (consumer, 0);
  t[1] = uthread_create (consumer, 0);
  t[2] = uthread_create (producer, 0);
  t[3] = uthread_create (producer, 0);
  uthread_join (t[0],0);
  uthread_join (t[1],0);
  uthread_join (t[2],0);
  uthread_join (t[3],0);
  
  
  // TODO: Create Threads and Join
  
  printf ("producer_wait_count=%d, consumer_wait_count=%d\n", producer_wait_count, consumer_wait_count);
  printf ("items value histogram:\n");
  int sum=0;
  for (int i = 0; i <= MAX_ITEMS; i++) {
    printf ("  items=%d, %d times\n", i, histogram [i]);
    sum += histogram [i];
  }
  
  
  assert (sum == sizeof (t) / sizeof (uthread_t) * NUM_ITERATIONS);
  
}
