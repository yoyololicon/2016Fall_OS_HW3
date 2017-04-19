#include <unistd.h>
#include <semaphore.h>
#include <ctime>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sys/time.h>
using namespace std;

#define NUM_THREADS 15

struct thread{
    vector<int>::iterator left, right;
};

vector<int>::iterator QuickSort_Buildthread(const vector<int>::iterator, const vector<int>::iterator, int);
void *TT(void*);
void *tt(void*);

struct thread Total_thread[NUM_THREADS];
sem_t sem[NUM_THREADS+1];
int task[NUM_THREADS];

int main()
{
    vector<int> matrix_1, matrix_2 ;
    int n, rc, data_n;
    string command;
    pthread_t Thread[NUM_THREADS];
    struct timeval start, end;

    cout << "Please enter the input file" << endl;
    cin >> command;

    ifstream data(command.c_str(), fstream::in);
    ofstream MT("output1.txt", fstream::out);
    ofstream ST("output2.txt", fstream::out);

    data >> data_n;

    while(data >> n)
        matrix_1.push_back(n);

    matrix_2 = matrix_1;

    Total_thread[1].left = matrix_1.begin();
    Total_thread[1].right = matrix_1.end();

    for(int i = 0; i <= NUM_THREADS; i++)
            sem_init(&sem[i], 0, 0);

    for(int i = 0; i < NUM_THREADS; i++)
    {
        task[i] = i+1;
        if(i < 7)
            rc = pthread_create(&Thread[i], NULL, TT, (void *)&task[i]);
        else
            rc = pthread_create(&Thread[i], NULL, tt, (void *)&task[i]);

        if (rc){
            cout << "ERROR; return code from pthread_create(" << task[i] << ")" << endl;
            return 1;
        }
    }

    //MT process
    gettimeofday(&start, 0);
    sem_post(&sem[0]);
    for(int i = 8; i <= NUM_THREADS; i++)
        sem_wait(&sem[i]);
    gettimeofday(&end, 0);

    cout << "multi-thread" << endl;
    int sec = end.tv_sec - start.tv_sec;
    int usec = end.tv_usec - start.tv_usec;
    cout << "elapsed " << sec*1000+(usec/1000.0) << " ms" << endl;

    for(vector<int>::iterator i = matrix_1.begin() ; i != matrix_1.end() ; i++)
        MT << *i << " ";

    //ST process
    gettimeofday(&start, 0);
    QuickSort_Buildthread(matrix_2.begin(), matrix_2.end(), 0);
    gettimeofday(&end, 0);

    cout << "single-thread" << endl;
    sec = end.tv_sec - start.tv_sec;
    usec = end.tv_usec - start.tv_usec;
    cout << "elapsed " << sec*1000+(usec/1000.0) << " ms" << endl;

    for(vector<int>::iterator i = matrix_2.begin() ; i != matrix_2.end() ; i++)
        ST << *i << " ";

    for(int i = 0; i <= NUM_THREADS; i++)
        sem_destroy(&sem[i]);
    data.close();
    MT.close();
    ST.close();
    return 0 ;
}

void *TT(void *threadarg)
{
    int t = *(int *)threadarg;
    int sig = t/2;
    sem_wait(&sem[sig]);

    Total_thread[t*2+1].right = Total_thread[t].right;
    Total_thread[t*2].left = Total_thread[t].left;
    Total_thread[t*2+1].left = Total_thread[t*2].right = QuickSort_Buildthread(Total_thread[t].left, Total_thread[t].right, 1);

    if(sem_post(&sem[t]) < 0)
        cout << t << " sem post fail" << endl;
    if(sem_post(&sem[t]) < 0)
        cout << t << " sem post fail" << endl;
    pthread_exit(NULL);
}

void *tt(void *threadarg)
{
    int t = *(int *)threadarg;
    int sig = t/2;
    sem_wait(&sem[sig]);

    QuickSort_Buildthread(Total_thread[t].left, Total_thread[t].right, 0);

    if(sem_post(&sem[t]) < 0)
        cout << t << " sem post fail" << endl;
    pthread_exit(NULL);
}

vector<int>::iterator QuickSort_Buildthread(const vector<int>::iterator left , const vector<int>::iterator right, int build)
{
    if(left < right)
    {
        vector<int>::iterator i = left ;
        vector<int>::iterator j = right ;
        cout << j-i << endl;
        int pivot = *i ;
        do
        {
            do
                i++;
            while(*i < pivot) ;
            do
                j--;
            while(*j > pivot) ;
            if(i<j)
                swap(*i , *j) ;
        }
        while(i < j) ;
        swap(*left , *j) ;
        if(build)
            return i;

        QuickSort_Buildthread(left , j, 0);
        QuickSort_Buildthread(i , right, 0);
    }
}
