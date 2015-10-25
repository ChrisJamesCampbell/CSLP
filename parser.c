//Prelimanary CSLP Simulator 


#include <stdio.h>
#include <stdlib.h>

/*struct input_file
{
  //capacity of a minibus in terms of number of passengers
  int bus_capacity;

  //passenger boarding/disembarkation duration (in seconds)
  int boarding_time;

  //Average number of journey requests per hour
  double request_rate;

  //Average time between request and desired pick-up (in minutes)
  double pickup_interval;

  //Maximum time a user can wait beyond the desired pick-up time (in minutes)
  int max_delay;

  //Number of minibuses
  int no_buses;

  //Number of bus stops
  int no_stops;

  //Road layout and distances (in minutes) between bus stops
  int[][] map;

  //Simulation duration (in hours)
  int stop_time;
}

static void initialise_input_file(struct input_file *input_file_instance)
{
  input_file_instance->bus_capacity = 0;
  input_file_instance->boarding_time = 0;
  input_file_instance->request_rate = 0.0;
  input_file_instance->pickup_interval = 0.0;
  input_file_instance->max_delay = 0;
  input_file_instance->no_buses = 0;
  input_file_instance->no_buses = 0;
  input_file_instance->map = {0, 0, 0, 0, 0, 0},
                    (0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0};


}

*/

static void parse_input()
{
  FILE *fp;
  char line[256];


  
  //capacity of a minibus in terms of number of passengers
  int busCapacity;

  //passenger boarding/disembarkation duration (in seconds)
  int boardingTime;

  //Average number of journey requests per hour
  double requestRate;

  //Average time between request and desired pick-up (in minutes)
  double pickupInterval;

  //Maximum time a user can wait beyond the desired pick-up time (in minutes)
  int maxDelay;

  //Number of minibuses
  int noBuses;

  //Number of bus stops
  int noStops;

  //Road layout and distances (in minutes) between bus stops
  int map[noStops][noStops];

  //Simulation duration (in hours)
  int stopTime;

  int rowIndex = noStops;
  int colIndex = noStops;

  for(rowIndex = 0; rowIndex < colIndex; rowIndex++)   
      free(map[rowIndex]);
    free(map);

  /*map = (int **) malloc(colIndex * sizeof(int*));  // rows
  for(rowIndex = 0; rowIndex < colIndex; rowIndex++)   
    map[rowIndex] = (int *) malloc(colIndex * sizeof(int));  // columns
*/

  fp = fopen("/Documents/CSLP/simulator_test_input.txt", "r");


  while(fgets(line,256, fp) != NULL)
  {
    if(strncmp("busCapacity", line, 11) == 0)
    {
      sscanf(line+11,"%*[ ]%d", &busCapacity);
    }

    if(strncmp("boardingTime", line, 12) == 0)
    {
      sscanf(line+12,"%*[ ]%d", &boardingTime);
    }

    if(strncmp("requestRate", line, 11) == 0)
    {
      sscanf(line+11, "%*[ ]%lf", &requestRate);
    }

    if(strncmp("pickupInterval", line, 14) == 0)
    {
      sscanf(line+14, "%*[ ]%lf", &pickupInterval);
    }

    if(strncmp("maxDelay", line, 8) == 0)
    {
      sscanf(line+8, "%*[ ]%d", &maxDelay);
    }

    if(strncmp("noBuses", line, 7) == 0)
    {
      sscanf(line+7, "%*[ ]%d", &noBuses);
    }

    if(strncmp("noStops", line, 7) == 0)
    {
      sscanf(line+7, "%*[ ]%d", &noStops);
    }

   const char delim[2] = " ";    //used to tokenise each of the rows of map
   char mapRows[noStops][16];

   if(strncmp("map", line, 3) == 0)
   {
    for(int i = 0; i < noStops; i++)
      &map[i][j] strtok( *line, "\n");
   }

   /*
   for(int i = 0; i < noStops; i++)
    sscanf(line+3, "%*\n  
                    %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ]
                    %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ]
                    %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ]
                    %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ]
                    %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ]
                    %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d %*[ ] %d ", 
                    &map[0][0], &map[0][1], &map[0][2], &map[0][3], &map[0][4], &map[0][5], 
                    &map[1][0], &map[1][1], &map[1][2], &map[1][3], &map[1][4], &map[1][5],
                    &map[2][0], &map[2][1], &map[2][2], &map[2][3], &map[2][4], &map[2][5],
                    &map[3][0], &map[3][1], &map[3][2], &map[3][3], &map[3][4], &map[3][5],
                    &map[4][0], &map[4][1], &map[4][2], &map[4][3], &map[4][4], &map[4][5],
                    &map[5][0], &map[5][1], &map[5][2], &map[5][3], &map[5][4], &map[5][5],
                    );*/
    


   if(strncmp("stopTime", line, 8) == 0);
   {
    sscanf(line+8, "%*[ ]%d", &stopTime);
   }


  } //end of while fgets loop


}

int main()
{
  

  parse_input();




  return 0;

}
