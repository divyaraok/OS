#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<sys/types.h>
#include<limits.h>

struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int* array;
};

struct Queue* createQueue(unsigned);
void enqueue(struct Queue *, int);
int dequeue(struct Queue *);
int isEmpty(struct Queue *);
int isFull(struct Queue *);
int front(struct Queue *); 
int rear(struct Queue *);

sem_t customers; 
sem_t barbers; 
sem_t cutting; 
sem_t mutex;

struct Queue* waiting_room;
int customers1 = 0; 
#define n 10

void barber() { 
  if(customers1!=0){
  wait(customers);
  wait(mutex);
  printf("Customer %d is getting haircut.\n",front(waiting_room));	  
  customers1--;
  dequeue(waiting_room);
  wait(5);
  signal(barbers);
  signal(mutex);
  cut_hair();
 }
 else sleep(10);
}
void customer() {
  wait(mutex);
  if (customers1 < n) { 
    customers1 = customers1 + 1; 
    enqueue(waiting_room,customers1);
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
  wait(cutting); 
} 

get_haircut(){
  signal(cutting); 
} 

int main(){
  pthread_t p1[n],p2;
  int op,i;

  sem_init(&mutex,0,1);
  sem_init(&customers,0,0);
  sem_init(&barbers,0,0);
  sem_init(&cutting,0,0);

 do{
        printf("Enter number of customers in waiting room : \n");
        scanf("%d",&op);
        for(i = 0; i<op; i++) {
		if(rear(waiting_room)!=n){
  			pthread_create(&p1[i],NULL,(void *)customer,NULL);
  			pthread_join(p1[i],NULL);
 		}
 		else{
  			printf("Cannot accomodate more than 10 customers\n");
  			break;
 		}
	}
	for(i = 0; i<op; i++) {
	  if(i<n){  pthread_create(&p2,NULL,(void *)barber,NULL);
 		    pthread_join(p2,NULL);
		    if(customers1 == 0) printf("Barber is starting to sleep.\n");
		 }
	}
 
            if(customers1 <= 0){
		    printf("Barber is sleeping.\n");
        }
    }while (op>=0);

  return 0;
}

struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1; 
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}
 

int isFull(struct Queue* queue)
{  return (queue->size == queue->capacity);  }
 
int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }
 
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}

int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}
 
int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}