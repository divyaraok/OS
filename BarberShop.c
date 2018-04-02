#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<semaphore.h>

sem_t customers; 
sem_t barbers; 
sem_t cutting; 
sem_t mutex;

int customer1 = 0; 
int n = 10;

void barber() { 
  while(true) { 
    wait(customers);
    wait(mutex);   
    customer1 = customer1 - 1; 
    signal(barbers);
    signal(mutex); 
    cut_hair(); 
  } 
}  
void customer() { 
  wait(mutex);  
  if (customer1 < n) { 
    customer1 = customer1 + 1; 
    signal(customers); 
    signal(mutex); 
    wait(barbers); 
    get_haircut(); 
  }
  else {  
    signal(mutex); 
  } 
} 

cut_hair(){ 
  waiting(cutting); 
} 

get_haircut(){
  signal(cutting); 
} 

int main(){
  customer();
  printf("no. of customers : %d\n",customer1);
  barber();	
  printf("no. of customers : %d\n",customer1);
  customer();
  printf("no. of customers : %d\n",customer1);
  customer();
  printf("no. of customers : %d\n",customer1);
  barber();
  printf("no. of customers : %d\n",customer1);
  return 0;
}