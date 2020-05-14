#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include<time.h>
#include <semaphore.h>
#define Number_of_philosopher 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define size 4
int Phil[size];
int Hands[size];
#define TRUE 1
#define FALSE 0
void Wants_to_deliver();
void release_signal(int);
void Thinking();
void wait_signal(int);
void done(int);

int number=0;
int number1=0;
int number2=0;
//int j=0;
void waiting();
#define LEFT (philosopher_num + 4) % Number_of_philosopher
#define RIGHT (philosopher_num + 1) % Number_of_philosopher
char names[5][15]={"MNC","C&F","Wholeseller","Shopkeeper","Customer"};
char names1[4][15]={"MNC","WHOLESELLER","SHOPKEPER","CUSTOMER"};
int state[Number_of_philosopher];
int phil[Number_of_philosopher]= { 0, 1, 2, 3, 4 };
sem_t mutex;
sem_t S[Number_of_philosopher];
void withSemaphore();
void withoutSemaphore();
void delay(int time_given)
{
    int s=1000*time_given;
    clock_t start_time=clock();
    while(clock()<start_time+s);
}

void test(int philosopher_num)
{
    if (state[philosopher_num]==HUNGRY && state[LEFT]!=EATING && state[RIGHT]!=EATING)
        {
        // state that eating
            state[philosopher_num]=EATING;
            delay(2);
        //int number1=0;
            if(number==philosopher_num)
            {
                if((number>=0) && (number<(Number_of_philosopher-1)))
                {

                printf("\n%s should give product to %s because of %s",names[philosopher_num],names[philosopher_num + 1],names[LEFT]);
                        delay(1);
                printf("\n%s is Currently collecting\n", names[philosopher_num]);
                waiting();
                }
                /*else if(number==(Number_of_philosopher-1))
                {
                    printf("\n%s checks %s and %s",names[number],names[LEFT],names[RIGHT]);
                     printf("\n%s has accepted product\n", names[philosopher_num]);
                }*/
               // printf("\n%s is Currently delivering\n", names[phnum]);
                 //       delay(1);
                //number++;
            }

            // sem_post(&S[phnum]) has no effect
            // during takefork
            // used to wake up hungry philosophers
            // during putfork
            sem_post(&S[philosopher_num]);
    }
}

// take up chopsticks
void take_fork(int philosopher_num)
{
    sem_wait(&mutex);
    // state that hungry
    state[philosopher_num]=HUNGRY;
    //int number=0;
    if(philosopher_num==number)
    {
        if(number==Number_of_philosopher-1)
        {
            //printf("\n%s wants to accept product from %s\n",names[philosopher_num],names[philosopher_num-1]);
            delay(1);
        }
        else if((philosopher_num>0) && (philosopher_num<(Number_of_philosopher-1)))
        {
            printf("\n%s demands for product from %s \n", names[philosopher_num],names[philosopher_num-1]);
            waiting();
            delay(1);
        }
        else if(number==Number_of_philosopher-1)
        {
            printf("\n%s wants to accept product from %s\n",names[philosopher_num],names[philosopher_num-1]);
            delay(1);
        }
        else if(philosopher_num==0)
        {
            printf("\n%s wants to deliver product\n",names[philosopher_num]);
            waiting();
            delay(1);
        }
        //number++;


    }
    // eat if neighbours are not eating
    test(philosopher_num);
    sem_post(&mutex);
    // if unable to eat wait to be signalled
    sem_wait(&S[philosopher_num]);
    delay(1);
}

// put down chopsticks
void put_fork(int philosopher_num)
{
    sem_wait(&mutex);
    // state that thinking
    state[philosopher_num]=THINKING;
    //int number2=0;
    if(number==philosopher_num)
    {
       if((philosopher_num>=0) && (philosopher_num<(Number_of_philosopher-1)))
        {

        printf("\n%s satisfied %s and %s\n",names[philosopher_num],names[philosopher_num+1],names[LEFT]);
        ++number;
        delay(1);
           // number++;
        }
        else if(philosopher_num==(Number_of_philosopher-1))
        {
            printf("\nCustomer Accepted the product");
        }
        //number++;
    }
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}

void* philospher(void* num)
{
    int j=0;
    while(j<5)
    {
        int* i=num;
        delay(1);
        take_fork(*i);
        delay(0);
        put_fork(*i);
        j++;
    }
    exit(0);
}

int main()
{
    int choose;
    char secureBit;
    printf("\n1.Without Semaphonre\n2.With Semaphore\n3.Exit\n");
    printf("Enter your choice: ");
    scanf("%d",&choose);

    switch(choose)
    {
        case 1:withoutSemaphore();
                break;
        case 2:withSemaphore();
                break;
        case 3:printf("Do you really want to exit(y/n):");
                scanf("%s",&secureBit);
                if(secureBit=='y' || secureBit=='Y')
                {
                    return 0;
                }
                else
                {
                    main();
                }
                break;
        default :   printf("Check input");
                    main();

    }

    /*pthread_t thread_id[Number_of_philosopher];
    // initialize the semaphores
    sem_init(&mutex,0,1);
    for (i=0;i<Number_of_philosopher;i++)
        sem_init(&S[i], 0, 0);

    for (i=0;i<Number_of_philosopher;i++)
    {
        // create philosopher processes
        pthread_create(&thread_id[i],NULL,philospher,&phil[i]);
        delay(0.5);
        printf(" %s is thinking\n",names[i]);
        //delay(0.5);
    }
    printf("\n");

    for (i=0;i<Number_of_philosopher;i++)
       {
            pthread_join(thread_id[i],NULL);
       }*/
}
void withSemaphore()
{
    system("cls");
    int i;
    pthread_t thread_id[Number_of_philosopher];
    // initialize the semaphores
    sem_init(&mutex,0,1);
    for (i=0;i<Number_of_philosopher;i++)
        sem_init(&S[i], 0, 0);

    for (i=0;i<Number_of_philosopher;i++)
    {
        // create philosopher processes
        pthread_create(&thread_id[i],NULL,philospher,&phil[i]);
        delay(0.5);
        printf(" %s is thinking\n",names[i]);
        //delay(0.5);
    }
    printf("\n");

    for (i=0;i<Number_of_philosopher;i++)
       {
            pthread_join(thread_id[i],NULL);
       }
}
void waiting()
{
    int i;
    for(i=number;i<4;i++)
    {
        delay(1);
        printf("%s is waiting\n",names[i+1]);

    }
}
void withoutSemaphore()
{
    system("cls");
        int i=0;

    /*To normally initialize Hands Semaphore to 1
    and even Phil to 1.
    and display first message as waiting.*/
    Thinking();

    //Observer here carefully, while loop will run until
    //all level employee deliver their product to customer.
    while(i<size)
    {
        printf("\n");
        wait_signal(i);     //This is wait_signal function called to check Hands semaphore.

        Wants_to_deliver(i);        //To check if the level is ready to deliver product.
        i++;
    }

}
void Thinking()
{
    int i;
    for(i=0;i<size;i++)
    {
        Phil[i]=TRUE;
        Hands[i]=TRUE;
        printf("%s is waiting\n",names1[i]);
        delay(1);
    }
}
void wait_signal(int i)
{
    Phil[i]=FALSE;
    Hands[i]=FALSE;
    Hands[(i+1)%size]=FALSE;
}
void Wants_to_deliver(int i)
{
        int l;
        for(l=0;l<size;l++)
        {
            if(Phil[l]==TRUE)
            {
                printf("%s is waiting\n",names1[l]);
                delay(1);
            }


  else if(Phil[l]==FALSE && Hands[l]==FALSE && Hands[(l+1)%size]==FALSE && l!=(size-1))
            {
                printf("%s wants to deliver some goods to %s\n",names1[l],names1[l+1]);
                release_signal(i);
                delay(1);
            }
            else if(l!=(size-1))
            {
                done(l);
            }
            else
            {
                printf("%s has accepted the goods delevered using %d and %d\n",names1[i],l,(l+1)%size);
                delay(1);
            }
        }
        printf("\n\t\t number of levels been delivered=%d\n\n",i);
        delay(1);
}

//To check for everyone except last level.
void done(int i)
{
    printf("%s have finished delivery using hands %d and %d\n",names1[i],i,(i+1)%size);
    delay(1);
}
void release_signal(int i)
{
    Hands[i]=TRUE;
    Hands[(i+1)%size]=TRUE;
}


