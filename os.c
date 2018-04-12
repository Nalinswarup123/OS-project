//The program runs for n number of students for many no of times 
//as a student after clearing his doubt can return to seek help for next doubt
#include <stdio.h>
#include <string.h> 
#include <semaphore.h>
#include <pthread.h> 
#include <stdlib.h>
int c = 0;
int s = 0;
int t = 0;
int totalSeats=3;
int sl=3;
int chair[3];
pthread_mutex_t mutex;
sem_t semTA;
sem_t semS;
void randSleep(void)
{
	sleep(rand() % sl + 1);
}
void* student(void* stID)
{
	int id = *(int*)stID;
	printf("Student %d is doing programming\n",id);		
	while(1)
	{
		randSleep();
		pthread_mutex_lock(&mutex);
		printf("Student %d comes to seek help\n",id);
		if(c < totalSeats)	
		{
			chair[s] = id;
			c++;
			int p=0;
			int k;
			s = (s+1) % totalSeats;
			
			for(k=0;k<3;k++)
			{
				if(chair[k]>0)
				p++;
			}
			printf("total waiting students :%d\n",p);
			if(p==0)
			exit(0);
			pthread_mutex_unlock(&mutex);
			sem_post(&semS);
			sem_wait(&semTA);
		}
		else
		{
			pthread_mutex_unlock(&mutex);
			
			printf("\nNo chair left. student %d resumes programming\n\n",id);		
		}
	}				
}

void* teacher()
{
	while(1)
	{
		sem_wait(&semS);	
		pthread_mutex_lock(&mutex);
		printf("TA is teaching student %d\n",chair[t]);
		printf("TA completed teaching Student %d\n",chair[t]);	
		chair[t]=0;
		c--;
		int k;
		int p=0;
		t = (t + 1) % totalSeats;
		for(k=0;k<3;k++)
		{
			if(chair[k]>0)
			p++;
		}
		printf("total waiting students :%d\n",p);
		randSleep();
		if(chair[0]==0 && chair[1]==0 && chair[2]==0)
		{
			printf("TA returns to nap\n");
			exit(0);
		}
		
		pthread_mutex_unlock(&mutex);
		sem_post(&semTA);
	}	
}
int main(){
	int* sID;
	int totalStudents;
	pthread_t *students;
	pthread_t ta;
	int i;
	printf("Enter no of students: ");
	scanf("%d", &totalStudents);
	printf("TA SLEEPING\n");
	students = (pthread_t*)malloc(sizeof(pthread_t) * totalStudents);
	sID = (int*)malloc(sizeof(int) * totalStudents);
	memset(sID, 0, totalStudents);
	sem_init(&semS,0,0);
	sem_init(&semTA,0,1);
	srand(time(NULL));
	pthread_mutex_init(&mutex,NULL);
	pthread_create(&ta,NULL,teacher,NULL);
	for(i=0; i<totalStudents; i++)
	{
		sID[i] = i+1;
		pthread_create(&students[i], NULL, student, (void*) &sID[i]);
	} 
	pthread_join(ta, NULL);
	for(i=0; i<totalStudents;i++)
	{
		pthread_join(students[i],NULL);
	}	
	return 0;
}
