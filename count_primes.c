/**
 * Mangled Mutexes Lab
 * CS 241 - Spring 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

/* You should create a struct that will get passed in by reference to your
 * start_routine. */

// Globals
size_t number_of_primes; // global number of primes you need to update
/* You should create a mutex for said global variable. */

/* You should create a start routine for your threads. */

// This function parses the arguments in argv.
// An error message is printed if any of the following happens:
//  An incorrect number of arguments are passed in.
//  'start', 'end', or 'num_threads' is less than 1.
//  'end' < 'start'.
// Else an array of 3 size_t elements are returned
// in the following order: 'start', 'end', 'num_threads'.
// It is the callers job to free the memory used.
size_t *arg_parse(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "usage: %s [start] [end] [num_threads]\n", argv[0]);
    exit(1);
  }

  char *endptr;
  long tokargs[3];
  for (size_t i = 0; i < sizeof(tokargs) / sizeof(tokargs[0]); i++) {
    tokargs[i] = strtol(argv[i + 1], &endptr, 10);
    if (*endptr != '\0') {
      fprintf(stderr, "Failed to convert an arugment to a long!\n");
      exit(2);
    }
    if (tokargs[i] < 1) {
      fprintf(stderr,
              "Please have all arguments be greater than or equal to 1!\n");
      exit(3);
    }
  }
  if (tokargs[1] < tokargs[0]) {
    fprintf(stderr, "Please give a valid range!\n");
    exit(4);
  }
  size_t *args = malloc(3 * sizeof(size_t));
  for (size_t i = 0; i < sizeof(tokargs) / sizeof(tokargs[0]); i++) {
    args[i] = (size_t)tokargs[i];
  }
  return args;
}

// Call on this function when you are ready to print the result.
void print_result(size_t number_of_primes, size_t start, size_t end) {
  printf("There are %zu primes between %zu and %zu inclusive\n",
         number_of_primes, start, end);
}


typedef struct {	
size_t start;
size_t end;
} ag;


ag* create(size_t a, size_t b)
{
     ag* p = malloc (sizeof (ag));
	p->start = a;
	p->end = b;
	return p;
} 

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void* prime(void*  p){
     ag* q= (ag*) p;
	for(size_t i = q->start ; i <= q->end; i++){
	    double k = sqrt( (double) i );
	    for(size_t j=2; j<k+1; j++) {
                        
			if( i % j == 0) {
		   		break;
			}
               if(i % j != 0 && j>= k ){
               	pthread_mutex_lock(&m);
				number_of_primes++;
		     	pthread_mutex_unlock(&m);  
			}
		}
	}
	return NULL;
}

int main(int argc, char *argv[]) {
 
    if (argc != 4) {
    fprintf(stderr, "usage: %s [start] [end] [num_threads]\n", argv[0]);
    exit(1);
  }

	size_t start =(size_t)atoi(argv[1]);
	size_t end = (size_t )atoi(argv[2]);
	size_t num = (size_t )atoi(argv[3]);
	size_t dis = end-start;
   
     size_t  unit = dis/num;
     if(  num >= (dis+1)/2){
		num = (dis+1)/2;
	}
     if(num == 0 )
	num =1;
     ag** tuple= (ag**)malloc(num * sizeof(ag*));
     
	
	if( num == 1)
		tuple[0] = create(start, end);
	else{
			tuple[0] = create(start, start+unit);
			size_t i = 1;
			for(i=1; i < num-1 ; i++)
			{
				tuple[i]= create(start+i*unit+1, start+(i+1)* unit);
			}   
				tuple[i] = create(start+i*unit+1, end); 
		}

	pthread_t*array = (pthread_t*)malloc(num * sizeof(pthread_t));

	for (size_t i = 0; i < num; i++)
	{
		pthread_create (&array[i], NULL, prime, tuple[i]);
	}
	for (size_t i = 0; i < num ; i++)
	{
		pthread_join(array[i], NULL);
	}

	if(start == 2 || (start == 1 && end != start))
     number_of_primes ++;

     for(size_t i =0 ; i< num ; i++)
	{
		free(tuple [i]);
	}

	free(tuple);
	free (array);

	print_result(number_of_primes,start, end); 
	

	
	
	return 0;




}


