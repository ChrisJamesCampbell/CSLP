//Prelimanary CSLP Simulator 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>


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
  time_t time_stamp;
  unsigned int from_stop;
  unsigned int to_stop;
  time_t for_departure;

  //think this is inappropriate to include in the request structure
  //and should in fact be the calculated metric that's appended on in an output string
  //at output stage
  //time_t scheduled_for;


};

struct bus
{
  unsigned int no_passengers;
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
    fleet[i].no_passengers = 0;
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

double uniform_deviate( int seed )
{
   return seed * ( 1.0 / ( RAND_MAX + 1.0 ) );
}


static struct request generate_request(struct input_file *test_input)
{
  //need to use the random algorithm to select randomly distributed numbers
  //to fill the fields of an instance of a request
  struct request new_request;

  new_request.time_stamp = unix_time_now();

  //so a user can't place a request for a bus to or from the garage
  //and so they can't place a request from their start stop to their destination 
  while(new_request.from_stop != 0 && new_request.to_stop != 0 && new_request.from_stop != new_request.to_stop)
  {
    new_request.from_stop = random_at_most((test_input->noStops) - 1);
    new_request.to_stop = random_at_most((test_input->noStops) - 1);
  }
  //this needs to convert 
  new_request.for_departure = (int)(new_request.time_stamp) + (random_at_most(test_input->pickupInterval) * 60); //time_stamp is in sscs and we need pickupInterval in secs too

  return new_request;
}

static void handle_request(struct request *new_request)
{


  return;
}


static struct input_file floyd_warshall(struct input_file *test_input) //temporary return idea for new map
{
  printf("I have made it into FW method\n");

  struct input_file updated_map; //map will be the 
  //initialise_input_file(&updated_map);

  int N = test_input->noStops;
  int rowIndex = N;
  int colIndex = N;
  int **dist;  // this will be the new matrix of distances



  updated_map.map = (int **) malloc(colIndex * sizeof(int*));  // rows
  for(rowIndex = 0; rowIndex < colIndex; rowIndex++)  
  { 
    updated_map.map[rowIndex] = (int *) malloc(colIndex * sizeof(int));
  }

  

  ////////////////////////////////////////////////////////////////
  //POSSIBLY SUPERFLIOUS CODE????????????????????????????????????
  
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
      updated_map.map[rowIndex][colIndex] = dist[rowIndex][colIndex];
    }

  }

  printf("The FW'd matrix was copied into the updated_map map\n");

  /*
  procedure Path(u, v)
   if next[u][v] = null then
       return []
   path = [u]
   while u ≠ v
       u ← next[u][v]
       path.append(u)
   return path
 */





  return updated_map;
}


/*
static char* output_request(struct request *new_request)
{
  char str = malloc(sizeof(char)*1024);
  char a[50], b[50], c[50], d[50];



  strcpy(str, sprintf(a, "%d", (int)new_request->time_stamp)); //need to convert this time to a string
  strcat(str, "-> new request placed from stop");
  strcat(str, sprintf(b, "%d", new_request->from_stop)); //need to convert this time to a string
  strcat(str, "to stop");
  strcat(str, sprintf(c, "%d", new_request->to_stop));
  strcat(str, "for departure at");
  strcat(str, sprintf(d, "%d", (int)new_request->for_departure));
  //strcat(str, "scheduled for");
  //strcat(str, sprintf(e, "%d", new_request->scheduled_for));

  return str;
}
*/



int main(int argc, char* argv[])
{
  
  struct input_file test_input;

  //initialise_input_file(&test_input);
  parse_input(&test_input, argv[1]);

  //time_t sec;
  //sec = time (NULL);

  struct bus fleet[test_input.noBuses];

  initialise_fleet(&test_input, fleet);

  printf("The Bus Capacity given in the test file was: %d persons.\n", test_input.busCapacity);
  printf("The Boarding Time given in the test file was: %d seconds.\n", test_input.boardingTime);
  printf("The Request Rate given in the test file was: %f seconds.\n", test_input.requestRate);
  printf("The Pick Up Interval given in the test file was: %f minutes.\n", test_input.pickupInterval);
  printf("The Max Delay given in the test file was: %d minutes.\n", test_input.maxDelay);
  printf("The Numer of Buses given in the test file was: %d buses.\n", test_input.noBuses);
  printf("The Number of Stops in the test file was: %d stops.\n", test_input.noStops);
  printf("The Stop Time given in the test file was: %d seconds.\n", test_input.stopTime);
  printf("The map given in the test file was: \n");


  int row, column;
  for(row = 0; row < test_input.noStops; row++)
  {
    for(column = 0; column < test_input.noStops; column++)
    {

      printf("%d\t", test_input.map[row][column]);
      if(column == test_input.noStops - 1)
      {
        printf("\n");
      }
    }
  }

  //free(test_input.map);

  struct input_file fwdmap = floyd_warshall(&test_input);


  printf("The Result of the Floyd-Warshall'd map is: \n");

  for(row = 0; row < test_input.noStops; row++)
  {
    for(column = 0; column < test_input.noStops; column++)
    {

      printf("%d ", fwdmap.map[row][column]);
      if(column == test_input.noStops - 1)
      {
        printf("\n");
      }
    }
  }


/*

  //loop for generating requests
  while(1)
  {
    struct request new_request = generate_request(&test_input); //make a new quest

    handle_request(&new_request); //handle that same request

    //printf("%s", output_request(&new_request)); //output the outcome of handling that request to the terminal

    //needs to be uniformly distributed or some shit
   //sleep((int)generate_random());
  sleep(5); //wait 5 seconds before creating the next request

  }

*/
  return 0;

}
