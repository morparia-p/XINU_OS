#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <typeinfo>
#include <vector>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#define DEFAULT_BUF_LENGTH (16 * 16384)

pthread_mutex_t lock;
int flag = 0;
std::fstream file;
std::vector <std::string> sent;
sem_t sema;
/* create thread argument struct for thr_func() */
typedef struct _thread_data_t {
  char word[100];
} thread_data_t;
 
/* thread function */
void *thr_func(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;
  std::string line;
  //std::fstream file;

  if(file.good())
  {
	while(1)
	{
		pthread_mutex_lock(&lock);
		//sem_wait(&sema);
		if(file.tellg()< 0)
		{
			pthread_mutex_unlock(&lock);
			//sem_post(&sema);
			pthread_exit(NULL);
		}
		std::getline(file,line);  
         	const char *line_arr = line.c_str();    
	        if (strstr(line_arr,data->word))
		{
			sent.push_back(line_arr);
		}
		pthread_mutex_unlock(&lock);
		//sem_post(&sema);
	}
  }
  pthread_exit(NULL);
}
 
int main(int argc, char **argv) {
  int i, rc;
  int n;
  char word[100];
  char file_name[100];
  char buf[DEFAULT_BUF_LENGTH];
  std::string line;
  size_t read;
  if (argc ==2){
	FILE* temp;
	temp  = fopen("input.txt","w");
	char c;
	if (temp!=NULL)
	{
		while (c!= EOF)
		{
			c = std::fgetc(stdin);
			fputc(c,temp);
		}
	}
	fclose (temp);
	strcpy(file_name,"input.txt");
    	strcpy(word,argv[1]);
	n =1;
	//while(std::cin>>buf) 
	//for (int i =0; i< 2; i++)
	}
  else if (argc==3){
    strcpy(word,argv[1]);
    strcpy(file_name,argv[2]);
    n = 1;
// Handling cases for piped input
/*    try
    {
	strtol( const char *str, char **str_end, int base );	
    }
    catch
    {
	
    }
*/

  }  
  else{
    n = atoi(argv[1]);
    strcpy(word,argv[2]);
        strcpy(file_name,argv[3]);
  }
  pthread_t thr[n];
  //std::fstream file;
  file.open(file_name);
  if(!file.good())
  {
	printf("Please provide a valid input file\n");
	return 1;
  }
  //file.seekg(0,file.end);
  //int len = file.tellg();
 /* create a thread_data_t argument array */
  thread_data_t thr_data[n];

  int start_line = 0;
 /* create threads */
  if (pthread_mutex_init(&lock, NULL) != 0)
   {
    printf("\n mutex init has failed\n");
        return 1;
   }
  if(sem_init(&sema, 0, 1) != 0)
  {
	printf("sem init failed\n");
	return 1;
  }
  for (i = 0; i < n; ++i) {
    strcpy(thr_data[i].word,word);
    if ((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
    }
  }
  
  for (i = 0; i <n ; ++i) {
    pthread_join(thr[i], NULL);

  }
//printf("printing op for thread i = :%d \n",i);
for (std::vector<std::string>::iterator it = sent.begin() ; it != sent.end(); ++it)
{
std::cout<< *it<<std::endl;
}

  sem_destroy(&sema); 
  pthread_exit(NULL);
  return EXIT_SUCCESS;
}


