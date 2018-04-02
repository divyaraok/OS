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
    printf("Barber : No. of customers : %d\n",customer1);
    signal(barbers);
    signal(mutex); 
    cut_hair(); 
  } 
}  
void customer() { 
  wait(mutex);  
  if (customer1 < n) { 
    customer1 = customer1 + 1; 
    printf("Customer : No. of customers : %d\n",customer1);
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
  pthread_t p1,p2;                                                          

  sem_init(&mutex,0,1);                                                     
  sem_init(&customers,0,0);                                                 
  sem_init(&barbers,0,0);                                                   
  sem_init(&cutting,0,0);                                                   
                                                                           
  pthread_create(&p1,NULL,(void *)barber,NULL);                             
  pthread_create(&p2,NULL,(void *)customer,NULL);                           
 
  pthread_join(p2,NULL);                                                    
  pthread_join(p1,NULL);
  return 0;
}