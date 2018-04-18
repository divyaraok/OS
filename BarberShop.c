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
int customer1 = 0; 
int n = 10;

void barber() { 
  if(customers1!=0){
  wait(customers);
  wait(mutex);
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
  if (customer1 < n) { 
    customer1 = customer1 + 1; 
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
  waiting(cutting); 
} 

get_haircut(){
  signal(cutting); 
} 

int main(){
  pthread_t p1,p2;
  int op,i;

  sem_init(&mutex,0,1);
  sem_init(&customers,0,0);
  sem_init(&barbers,0,0);
  sem_init(&cutting,0,0);

 do{
        printf("Who are you ?\n");
        printf("\n1)Customer \n2)Barber \n3)Processor \n4)Exit");
        printf("\nEnter Choice: ");
        scanf("%d",&op);
        switch (op) {
            case 1: pthread_create(&p1,NULL,(void *)customer,NULL);
 		                pthread_join(p1,NULL);
                    break;

	          case 2: pthread_create(&p2,NULL,(void *)barber,NULL);
 		                pthread_join(p2,NULL);
		                printf("Customer %d is getting haircut.\n",front(waiting_room));
		                break;
 
            case 3: if(customers1 == 0) printf("Barber is sleeping \n");
		                else printf("Barber is cutting hair of customer %d\n",front(waiting_room));
		                printf("No of customers in waiting room : %d\n",customers1);
		                break;

            default: break;
        }
    }while (op!=4);ULL,(void *)customer,NULL);

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