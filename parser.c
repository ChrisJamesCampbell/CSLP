//Prelimanary CSLP Simulator 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>

unsigned int global_time;
unsigned int max_time; // to be set to stopTime given by the input_file


time_t unix_time_now()
{
   time_t now;

   /* Get current time */
   time(&now);

  return now;
}

struct input_file
{
  ///capacity of a minibus in terms of number of passengers
  unsigned int busCapacity;

  //passenger boarding/disembarkation duration (in seconds)
  unsigned int boardingTime;

  //Average number of journey requests per hour
  float requestRate;

  //Average time between request and desired pick-up (in minutes)
  float pickupInterval;

  //Maximum time a user can wait beyond the desired pick-up time (in minutes)
  unsigned int maxDelay;

  //Number of minibuses
  unsigned int noBuses;

  //Number of bus stops
  unsigned int noStops;

  //Road layout and distances (in minutes) between bus stops
  //int map[noStops][noStops];
  int **map; //potentially new way to store map

  //Simulation duration (in hours)
  unsigned int stopTime;
};

struct request
{
  unsigned int time_stamp;
  unsigned int from_stop;
  unsigned int to_stop;
  unsigned int for_departure;

  //think this is inappropriate to include in the request structure
  //and should in fact be the calculated metric that's appended on in an output string
  //at output stage
  //time_t scheduled_for;


};

struct bus
{
  unsigned int occupied;
  unsigned int current_stop;
  unsigned int next_stop;
  unsigned int *path; //to hold an array of stops
};




static void initialise_input_file(struct input_file *test_input)
{
  //int row, column = 0;

  test_input->busCapacity = 0;
  test_input->boardingTime = 0;
  test_input->requestRate = 0.0;
  test_input->pickupInterval = 0.0;
  test_input->maxDelay = 0;
  test_input->noBuses = 0;
  test_input->noStops = 0;

  //test_input->map[noStops][noStops] = [0][0];
  /*for(row = 0; row < 6; row++)
  {
    for(column = 0; column < 6; column++)
    {
      test_input->map[row][column] = -1;
    }
  }

  */

  test_input->stopTime = 0;
  return;

}

static void initialise_fleet(struct input_file *test_input, struct bus fleet[test_input->noBuses])
{
  int i;
  for(i = 0; i < test_input->noBuses; i++)
  {
    fleet[i].occupied = 0;
    fleet[i].current_stop = 0;
    fleet[i].next_stop = 0;
    fleet[i].path = malloc(sizeof(int) * test_input->noStops); //temporary idea for allowed size of path
  }
}




static void parse_input(struct input_file *test_input, char* input)
{
  FILE *fp;
  char line[200];
  char *delim = " \t";
  char *tempRow;
  char *token;






  fp = fopen(input, "r");
    
  
   while(1)

   {

    fgets(line, 200, fp);

    if(feof(fp))
    {
      break;
    }
  
    if(strncmp("busCapacity", line, 11) == 0)
    {
      sscanf(line+11,"%*[ ]%d", &test_input->busCapacity);
    }

    if(strncmp("boardingTime", line, 12) == 0)
    {
      sscanf(line+12,"%*[ ]%d", &test_input->boardingTime);
    }

    if(strncmp("requestRate", line, 11) == 0)
    {
      sscanf(line+11, "%*[ ]%f", &test_input->requestRate);
    }

    if(strncmp("pickupInterval", line, 14) == 0)
    {
      sscanf(line+14, "%*[ ]%f", &test_input->pickupInterval);
    }

    if(strncmp("maxDelay", line, 8) == 0)
    {
      sscanf(line+8, "%*[ ]%d", &test_input->maxDelay);
    }

    if(strncmp("noBuses", line, 7) == 0)
    {
      sscanf(line+7, "%*[ ]%d", &test_input->noBuses);
    }

    if(strncmp("noStops", line, 7) == 0)
    {
      sscanf(line+7, "%*[ ]%d", &test_input->noStops);
    }

  int rowIndex = test_input->noStops;
  int colIndex = test_input->noStops;

  
  test_input->map = (int **) malloc(colIndex * sizeof(int*));  // rows
  for(rowIndex = 0; rowIndex < colIndex; rowIndex++)  
  { 
    test_input->map[rowIndex] = (int *) malloc(colIndex * sizeof(int));
  }  // columns

 
  
  if(strncmp("map", line, 3) == 0)
   {
    fgets(line, 200, fp);

    tempRow = strtok(line, "\n");
    //printf("The value of tempRow is: %s", tempRow);
    for(int row = 0; row < test_input->noStops; row++)
    {
      token = strtok(tempRow, delim);
      test_input->map[row][0] = atoi(token);

      for(int column = 1; column < test_input->noStops; column++)
      {
        //printf("The value of token is: %s", token);
        token = strtok(NULL, delim);
        //token = strtok(NULL, delim);
        test_input->map[row][column] = atoi(token);
      }

      fgets(line, 200, fp); 
      }
    } //closes the end of map reading
    
  
   if(strncmp("stopTime", line, 8) == 0)
   {
    sscanf(line+8, "%*[ ]%d", &test_input->stopTime);
   }

   

  } //end of while fgets loop

  fclose(fp);
}

double uniform_deviate( int seed )
{
   return seed * ( 1.0 / ( RAND_MAX + 1.0 ) );
}

static int generate_random( int seed, float request_rate) {

  //srand((unsigned) time(NULL));
  double r;

  while(r == 0.0)
  {
    r = uniform_deviate( rand());
    printf("The value of r is: %lf\n", r);
  }
  r = (int) (-log(r)*request_rate);

  return r;

}

void format_global_time(char *str, int t)
{
  unsigned int seconds, minutes, hours, days;

  //char str[1024]; //1024 chosen just so as to have well enough memory allocated
  //printf("The formatted time beforehand: %s\n", str);
  //deliberate truncation utilized for the # of days
  days = (t / 86400); //24*60*60, 
  hours = (t / 3600) % 24 ;
  minutes = (t/ 60) % 60;
  seconds = (t % 60) ;

  sprintf(str, "%02d:%02d:%02d:%02d", days, hours, minutes, seconds);
  //printf("The formatted time is afterwards: %s\n", str);
  return;

}

static long random_at_most(long max) {
  unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
   x = rand();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (unsigned long)x);

  // Truncated division is intentional
  return x/bin_size;
 }

 unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}




static struct request generate_request(struct input_file *test_input)
{
  //need to use the random algorithm to select randomly distributed numbers
  //to fill the fields of an instance of a request
  struct request new_request;

  new_request.time_stamp = global_time;

  //so a user can't place a request for a bus to or from the garage
  //and so they can't place a request from their start stop to their destination 

  //printf("From stop is: %d\n", new_request.from_stop);

    //srand(global_time);

    new_request.from_stop = rand_interval(1, (test_input->noStops) -1) ;
    new_request.to_stop = rand_interval(1, (test_input->noStops) -1) ;

    while(new_request.from_stop == new_request.to_stop)
    {
      
      new_request.to_stop = rand_interval(1, (test_input->noStops) -1) ;
    
  }

  //this needs to convert 
  new_request.for_departure = (global_time + (random_at_most(test_input->pickupInterval) * 60)); //time_stamp is in sscs and we need pickupInterval in secs too
  //printf("The value of new_request.for_departure is: %d\n", new_request.for_departure);

  return new_request;
}

static void handle_request(struct request *request, struct input_file *test_input, struct bus *fleet)
{
  //printf("I have made it into handle_request\n");
  //printf("The value of request.time_stamp is: %d \n", request->time_stamp);
  //printf("The value of request.for_departure in handle_request is: %d\n", request->for_departure);
  //int garage_to_stop = (test_input->map[0][request->from_stop]) * 60; //distance in seconds
  //char* skidouche;
  //skidouche = format_global_time((request->time_stamp + test_input->map[0][request->from_stop] * 60));
  //int distance_to_travel = (test_input->map[request->from_stop][request->to_stop]) * 60; //distance between request stops in seconds
  //printf("The value of no buses is: %d", no_bus);

  int selected_bus;
  int i;
  i = 0;
  
  int no_bus = test_input->noBuses;
  int boarding_time = test_input->boardingTime;

  int bus_locations[test_input->noStops];
  int time_left_stop = request->time_stamp;
  int max_delay = test_input->maxDelay * 60;


  //initialise the closest bus to request as the 0th bus
  int closest_bus, closest_bus_location;
  closest_bus = 0;
  closest_bus_location = fleet[0].current_stop;


  //what will the time be when i get to the source stop of the request after travelling there
  int time_to_from = request->time_stamp + (test_input->map[fleet[closest_bus].current_stop][request->from_stop] * 60); 

  //the DISTANCE in time between source stop and destination stop
  int from_stop_to_stop_time = (test_input->map[request->from_stop][request->to_stop] * 60); 


  int left_from_stop = time_to_from + boarding_time;
  int arrival_time = left_from_stop + from_stop_to_stop_time;
  int time_unoccupied = arrival_time + boarding_time;

  char time_stamp[1024];
  format_global_time(time_stamp, request->time_stamp);

  char for_departure[1024];
  format_global_time(for_departure, request->for_departure);

  char scheduled_for[1024];
  format_global_time(scheduled_for, time_to_from);



  char arrived_at_from[1024];
  format_global_time(arrived_at_from, time_to_from);

  char left_from_stop_s[1024];
  format_global_time(left_from_stop_s, left_from_stop);

  char arrival_time_s[1024];
  format_global_time(arrival_time_s, arrival_time);

  char time_unoccupied_s[1014];
  format_global_time(time_unoccupied_s, time_unoccupied);
  



  //printf("I got out of the garage_loop\n");


  //iterate through all of the fleet to find the bus the shortest distance from the pick up stop
  //and then select the smallest accordingly
  //printf("The location of minibus %d is stop: %d\n", closest_bus, fleet[closest_bus].current_stop);
  for(i = no_bus - 1; i > 0; i--)
  {
    if(test_input->map[fleet[closest_bus].current_stop][request->from_stop] > test_input->map[fleet[i].current_stop][request->from_stop] && fleet[i].current_stop == 0)
    {
      fleet[i].current_stop = closest_bus_location;
      closest_bus_location = fleet[i].current_stop;
      closest_bus = i; //
      break;

    }



    else if(test_input->map[fleet[closest_bus].current_stop][request->from_stop] > test_input->map[fleet[i].current_stop][request->from_stop]) //closes_bus is initially set to fleet[0]
    {
      fleet[i].current_stop = closest_bus_location;
      closest_bus_location = fleet[i].current_stop;
      closest_bus = i; //
      break;





    }
  }
      


  time_to_from = request->time_stamp + (test_input->map[closest_bus_location][request->from_stop] * 60);
  left_from_stop = time_to_from + boarding_time;
  arrival_time = left_from_stop + from_stop_to_stop_time;
  time_unoccupied = arrival_time + boarding_time;

    
  if((time_to_from - request->for_departure) > max_delay) //i.e we can't accomodate this bus
  {
    printf("%s -> new request placed from stop %d to stop %d for departure at %s cannot be accomodated \n", 
    time_stamp,
    request->from_stop, 
    request->to_stop, 
    for_departure);
    return;
  }


  printf("%s -> new request placed from stop %d to stop %d for departure at %s scheduled for %s \n", 
  time_stamp,
  request->from_stop, 
  request->to_stop, 
  for_departure,
  scheduled_for);



  //printf("The location of minibus %d is stop: %d\n", closest_bus, fleet[closest_bus].current_stop);
  //printf("%s -> Minibus %d left stop %d", time_stamp, closest_bus,);
  printf("%s -> Minibus %d arrived at stop %d\n", arrived_at_from, closest_bus, request->from_stop);
  fleet[closest_bus].current_stop = request->from_stop;

  printf("%s -> Minibus %d became occupied and left stop %d\n", left_from_stop_s, closest_bus, request->from_stop);
  fleet[closest_bus].occupied++;

  //the bus has to travel from the pick up to the destination given the time it takes to get there
  printf("%s -> Minibus %d arrived at stop %d \n", arrival_time_s, closest_bus, request->to_stop);
  fleet[closest_bus].current_stop = request->to_stop;

  printf("%s -> Minibus %d disembarked passenger and became unoccupied\n\n", time_unoccupied_s, closest_bus);
  fleet[closest_bus].occupied--;


  //printf("The location of minibus %d is stop: %d\n", closest_bus, fleet[closest_bus].current_stop);
    

    

  global_time = time_unoccupied;
   
   





  return;
} //end of handle_request


static void floyd_warshall(struct input_file *test_input) //temporary return idea for new map
{
  printf("I have made it into FW method\n");

  //initialise_input_file(&updated_map);

  int N = test_input->noStops;
  int rowIndex = N;
  int colIndex = N;
  int **dist;  // this will be the new matrix of distances


  dist = (int **) malloc(colIndex * sizeof(int*));  // rows
  for(rowIndex = 0; rowIndex < colIndex; rowIndex++)
  {   
    dist[rowIndex] = (int *) malloc(colIndex * sizeof(int));
  }

  rowIndex = 0;
  colIndex = 0;
  for(rowIndex = 0; rowIndex < N; rowIndex++)
  {
    for(colIndex = 0; colIndex < N; colIndex++)
    {
      //printf("chris\n");
      dist[rowIndex][colIndex] = test_input->map[rowIndex][colIndex];
    }

  }

  printf("The dist copy was made\n");

  /////////////////////////////////////////////////////////////////

  int i, j, k;
  //char route[] = "";
  // Input data into dist, where dist[i][j] is the distance from i to j.
   
     for ( k = 0; k < N; k++ )
     {
        for ( i = 0; i < N; i++ )
        {
           for ( j = 0; j < N; j++ )
           {
            if(dist[i][k] == -1 || dist[k][j] == -1)
            {
              //do nothing, since route does not exist
            }

              else if(dist[i][j] > (dist[i][k] + dist[k][j]) || dist[i][j] == -1)
              {
                dist[i][j] = (dist[i][k] + dist[k][j]);
                //str_append(route, k);
              }
                            
           }
        }
     }

     printf("The FW'd matrix was made\n");

  rowIndex = 0;
  colIndex = 0;
  for(rowIndex = 0; rowIndex < N; rowIndex++)
  {
    for(colIndex = 0; colIndex < N; colIndex++)
    {
      test_input->map[rowIndex][colIndex] = dist[rowIndex][colIndex];
    }

  }

  printf("The FW'd matrix was copied into the updated_map map\n");

  return;
}


int main(int argc, char* argv[])
{
  
  struct input_file test_input;

  //initialise_input_file(&test_input);
  parse_input(&test_input, argv[1]);

 

  struct bus fleet[test_input.noBuses];

  initialise_fleet(&test_input, fleet);

  /* Intializes random number generator */
  srand((unsigned int) time(NULL));

  float request_rate = test_input.requestRate;
  float stop_time = test_input.stopTime;

  printf("The Bus Capacity given in the test file was: %d persons.\n", test_input.busCapacity);
  printf("The Boarding Time given in the test file was: %d seconds.\n", test_input.boardingTime);
  printf("The Request Rate given in the test file was: %f per hour.\n", test_input.requestRate);
  printf("The Pick Up Interval given in the test file was: %f minutes.\n", test_input.pickupInterval);
  printf("The Max Delay given in the test file was: %d minutes.\n", test_input.maxDelay);
  printf("The Numer of Buses given in the test file was: %d buses.\n", test_input.noBuses);
  printf("The Number of Stops in the test file was: %d stops.\n", test_input.noStops);
  printf("The Stop Time given in the test file was: %d hours.\n", test_input.stopTime);
  printf("The map given in the test file was: \n");


  int row, column;
  for(row = 0; row < test_input.noStops; row++)
  {
    for(column = 0; column < test_input.noStops; column++)
    {

      printf("%d\t", test_input.map[row][column]);
      
        
      
    }
    printf("\n");
  }

  //free(test_input.map);

  floyd_warshall(&test_input);

  ////////////////////////////////////////FROM HERE ON, USE test_input INSTEAD OF test_input////////////////////////////////////////////////


  printf("The Result of the Floyd-Warshall'd map is: \n");

  for(row = 0; row < test_input.noStops; row++)
  {
    for(column = 0; column < test_input.noStops; column++)
    {

      printf("%d\t", test_input.map[row][column]);
      
    }
    printf("\n");
  }

//////////////////////////////THE PROGRAM IS ACTUALLY WORKING UP UNTIL HERE////////////////////////////////////////////////
//NEED TO GENERATE REQUESTS

//global_time = generate_random(rand(), request_rate);
global_time = 0;

printf("The global_time is: %d seconds\n", global_time);

max_time = stop_time * 3600; // for conversion into seconds from hours


printf("The maximum_time for the simulation is: %d\n", max_time);

//the simulator time frame, translated from pseudocode in handout
while(global_time < max_time)
{
  //srand(time(NULL));
  struct request new_request = generate_request(&test_input);
  handle_request(&new_request, &test_input, fleet);
  /*printf("%s -> new request placed from stop %d to stop %d scheduled for %s\n", 
    format_global_time(new_request.time_stamp), 
    new_request.from_stop, 
    new_request.to_stop, 
    format_global_time(new_request.for_departure));*/


  
  //int incrementing_factor = (int) ( 60/request_rate ) * 60; //gives us the number of minutes in between requests then multiply this by 60 to give us it in seconds
  int incrementing_factor = generate_random(time(NULL), request_rate);
  int request_frequency = ((60/request_rate) * 60 ); //average frequency of requests in seconds
  //global_time = global_time + incrementing_factor;

  //handle_request(&new_request, &updated_map, &fleet);


}





  return 0;

}
