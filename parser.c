//Prelimanary CSLP Simulator 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>

struct input_file
{
  ///capacity of a minibus in terms of number of passengers
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
  //int map[noStops][noStops];
  int map;

  //Simulation duration (in hours)
  int stopTime;
};


static void initialise_input_file(struct input_file *test_input)
{
  test_input->busCapacity = 0;

  test_input->boardingTime = 0;

  test_input->requestRate = 0.0;

  test_input->pickupInterval = 0.0;

  test_input->maxDelay = 0;

  test_input->noBuses = 0;

  test_input->noStops = 0;

  //test_input->map[noStops][noStops] = [0][0];
  test_input->map = 0;

  test_input->stopTime = 0;

}




static void parse_input(struct input_file *test_input)
{
  FILE *fp;
  char line[1024];


  /*int rowIndex = test_input->noStops;
  int colIndex = test_input->noStops;

  for(rowIndex = 0; rowIndex < colIndex; rowIndex++)   
      free(map[rowIndex]);
    free(map);*/

  /*map = (int **) malloc(colIndex * sizeof(int*));  // rows
  for(rowIndex = 0; rowIndex < colIndex; rowIndex++)   
    map[rowIndex] = (int *) malloc(colIndex * sizeof(int));  // columns
*/

  fp = fopen("/Documents/CSLP/simulator_test_input.txt", "r");


  while(fgets(line,1024, fp) != NULL)
  {
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
      sscanf(line+11, "%*[ ]%lf", &test_input->requestRate);
    }

    if(strncmp("pickupInterval", line, 14) == 0)
    {
      sscanf(line+14, "%*[ ]%lf", &test_input->pickupInterval);
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

   /*const char delim[2] = " ";    //used to tokenise each of the rows of map
   char mapRows[noStops][16];*/

   /*if(strncmp("map", line, 3) == 0)
   {
    for(int i = 0; i < noStops; i++)
      &map[i][j] strtok( */

   /*
   for(int i = 0; i < noStops; i++)nput
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
    sscanf(line+8, "%*[ ]%d", &test_input->stopTime);
   }


  } //end of while fgets loop


}

int main()
{
  
  struct input_file test_input;
  initialise_input_file(&test_input);
  parse_input(&test_input);

  printf("The Bus Capacity given in the test file was: %d", test_input.busCapacity);




  return 0;

}
