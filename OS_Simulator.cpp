#include <iostream>
#include <algorithm>
#include <cmath>
#include <unistd.h>
#include<pthread.h>
#include<mutex>
#include <queue>
using namespace std;

struct ljfNode {
    int id;
    int burst_time;
    int arrival_time;
    int duration;
};

class ljfQueue {
public:
    int front, rear;
    int size;
    ljfNode* arr;
    ljfQueue(int s) {
        front = rear = -1;
        size = s;
        arr = new ljfNode[size];
    }

    bool isFull() {
        return (rear == size - 1);
    }
    bool isEmpty() {
        return (front == -1 || front > rear);
    }
    void enqueue(ljfNode node) {
        if (isFull()) {
            cout << "Queue is full\n";
            return;
        }
        rear++;
        node.duration = node.burst_time - node.arrival_time;
        arr[rear] = node;
        if (front == -1) {
            front = 0;
        }
    }
    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty\n";
            return;
        }
        
        front++;
    }
    ljfNode gettop()
    {
        if (!isEmpty())
        {
            ljfNode temp = arr[front];
            return temp;
        }
        else
        {
            return ljfNode();
        }
    }
    void sortAscByArrivalTime() {
        sort(arr + front, arr + rear + 1, [](ljfNode a, ljfNode b) { return a.arrival_time < b.arrival_time; });
    }
    void sortDescByDuration() {
        sort(arr + front, arr + rear + 1, [](ljfNode a, ljfNode b) { return a.duration > b.duration; });
    }
};

void ljf() {
    int n = 4;
    cout << "ENTER THE NUMBER OF PROCESSES " << endl;
    cin >> n;
    int pid;
    int ax;
    int b; 
    ljfQueue q(n);
    ljfQueue R(n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time ";
        cin >>  ax >> b;
        ljfNode node = { a+1,b+ax,ax};
        q.enqueue(node);
    }
    int time = 0;
    q.sortAscByArrivalTime();
    while (!(q.isEmpty() && R.isEmpty()))
    {
        ljfNode temp = q.gettop();
        if (time == temp.arrival_time)
        {
            cout << "THE PROCESS " << temp.id << " HAS ARRIVED AT TIME "<<time << endl;
            R.enqueue(temp);
            q.dequeue();
            R.sortDescByDuration();
        }
        temp = q.gettop();
        if (time == temp.arrival_time)
        {
            cout << "THE PROCESS " << temp.id << " HAS ARRIVED AT TIME " << time << endl;
            R.enqueue(temp);
            q.dequeue();
            R.sortDescByDuration();
        }
        if (!R.isEmpty())
        {
            R.sortDescByDuration();
            ljfNode temp = R.gettop();
            for (int rt = 0; rt < temp.duration; rt++)
            {
                ljfNode temp2 = q.gettop();
                cout << "executing :" << temp.id <<"at time"<<time<< endl;
                sleep(1);
                time++;
                if (time == temp2.arrival_time)
                {
                    cout << "THE PROCESS " << temp2.id << " HAS ARRIVED AT TIME " << time << endl;
                    R.enqueue(temp2);
                    q.dequeue();
                }
            }
            R.dequeue();
            cout << "THE PROCESS " << temp.id << " HAS FINISHED AT TIME " << time << endl;
        }
        else
        {
            sleep(1);
            time++;
        }
        
    }
    return;
}
struct lfrNode {
    int id;
    int burst_time;
    int arrival_time;
    int waiting_time;
    float ratio;
};

class lfrQueue {
public:
    int front, rear;
    int size;
    lfrNode* arr;
    lfrQueue(int s) {
        front = rear = -1;
        size = s;
        arr = new lfrNode[size];
    }

    bool isFull() {
        return (rear == size - 1);
    }
    bool isEmpty() {
        return (front == -1 || front > rear);
    }
    void enqueue(lfrNode node) {
        if (isFull()) {
            cout << "Queue is full\n";
            return;
        }
        rear++;
        node.waiting_time = 0;
        node.ratio = (node.waiting_time + node.burst_time) / node.burst_time;
        arr[rear] = node;
        if (front == -1) {
            front = 0;
        }
    }
    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty\n";
            return;
        }

        front++;
    }
    lfrNode gettop()
    {
        if (!isEmpty())
        {
            lfrNode temp = arr[front];
            return temp;
        }
        else
        {
            return lfrNode();
        }
    }
    void sortAscByArrivalTime()
    {
        sort(arr + front, arr + rear + 1, [](lfrNode a, lfrNode b) { return a.arrival_time < b.arrival_time; });
    }
    void incrementWaitingTime() {
        for (int i = front; i <= rear; i++) {
            arr[i].waiting_time++;
        }
    }

    void calculateRatio() {
        for (int i = front; i <= rear; i++) {
            arr[i].ratio = (arr[i].waiting_time + arr[i].burst_time) / arr[i].burst_time;
        }
    }
    void check() {
        for (int i = front; i <= rear; i++) {
            cout << arr[i].id << " ";
        }
        cout << endl;
    }
    void sortAscByRatio() {
        sort(arr + front, arr + rear + 1, [](lfrNode a, lfrNode b) { return a.ratio < b.ratio; });
    }

};

void lfr() {
    int n = 4;
    cout << "ENTER THE NUMBER OF PROCESSES " << endl;
    cin >> n;
    int pid;
    int ax;
    int b;
    lfrQueue q(n);
    lfrQueue R(n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time ";
        cin >> ax >> b;
        lfrNode node = { a + 1,b,ax };
        q.enqueue(node);
    }
    int turn = 0;
    int time = 0;
    q.sortAscByArrivalTime();
    while (!(q.isEmpty() && R.isEmpty()))
    {
        lfrNode temp = q.gettop();
        if (turn == 0)
        {
            if (time == temp.arrival_time)
            {
                cout << "THE PROCESS " << temp.id << " HAS ARRIVED AT TIME " << time << endl;
                R.enqueue(temp);
                q.dequeue();
                R.calculateRatio();
                R.sortAscByRatio();
                turn = 1;
            }
        }
        if (!R.isEmpty())
        {
            R.calculateRatio();
            R.sortAscByRatio();
            lfrNode temp = R.gettop();
            R.dequeue();
            for (int rt = 0; rt < temp.burst_time; rt++)
            {
                lfrNode temp2 = q.gettop();
                cout << "executing :" << temp.id << "at time" << time << endl;
                sleep(1);
                time++;
                R.incrementWaitingTime();
                if (time == temp2.arrival_time)
                {
                    cout << "THE PROCESS " << temp2.id << " HAS ARRIVED AT TIME " << time << endl;
                    R.enqueue(temp2);
                    q.dequeue();
                    R.calculateRatio();
                    R.sortAscByRatio();
                }
            }
            cout << "THE PROCESS " << temp.id << " HAS FINISHED AT TIME " << time << endl;
        }
        else
        {
            sleep(1);
            time++;
        }

    }
    return;
}

struct mlqNode {
    int id;
    int burst_time;
    int arrival_time;
    int priority;
};

class mlqQueue {
public:
    int front, rear;
    int size;
    mlqNode* arr;
    mlqQueue(int s) {
        front = rear = -1;
        size = s;
        arr = new mlqNode[size];
    }

    bool isFull() {
        return (rear == size - 1);
    }
    bool isEmpty() {
        return (front == -1 || front > rear);
    }
    void enqueue(mlqNode node) {
        if (isFull()) {
            cout << "Queue is full\n";
            return;
        }
        rear++;
        arr[rear] = node;
        if (front == -1) {
            front = 0;
        }
    }
    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty\n";
            return;
        }

        front++;
    }
    mlqNode gettop()
    {
        if (!isEmpty())
        {
            mlqNode temp = arr[front];
            return temp;
        }
        else
        {
            return mlqNode();
        }
    }
    void sortAscByArrivalTime() {
        sort(arr + front, arr + rear + 1, [](mlqNode a, mlqNode b) { return a.arrival_time < b.arrival_time; });
    }
    void sortAscByPriority() {
        sort(arr + front, arr + rear + 1, [](mlqNode a, mlqNode b) { return a.priority > b.priority; });
    }
    void sortAscByDuration() {
        sort(arr + front, arr + rear + 1, [](mlqNode a, mlqNode b) { return a.burst_time < b.burst_time; });
    }

};

void mlq()
{
    int n = 4;
    cout << "ENTER THE NUMBER OF PROCESSES FOR PRIORITY QUEUE " << endl;
    cin >> n;
    int pid;
    int ax;
    int b;
    int p;
    int az = 0;
    mlqQueue q1(n);
    mlqQueue R1(n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time and priority";
        cin >> ax >> b >> p;
        mlqNode node = { az + 1,b ,ax,p };
        az++;
        q1.enqueue(node);
    }
    cout << "ENTER THE NUMBER OF PROCESSES FOR ROUND ROBIN QUEUE " << endl;
    cin >> n;
    mlqQueue q2(n);
    mlqQueue R2(4 * n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time ";
        cin >> ax >> b;
        mlqNode node = { az + 1,b ,ax,0 };
        az++;
        q2.enqueue(node);
    }
    cout << "ENTER THE QUANTUM FOR ROUND ROBIN " << endl;
    int quantum;
    cin >> quantum;
    cout << "ENTER THE NUMBER OF PROCESSES FOR THE SRJF "<<endl;
    cin >> n;
    mlqQueue q3(n);
    mlqQueue R3(100 * n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time ";
        cin >> ax >> b;
        mlqNode node = { az + 1,b,ax };
        az++;
        q3.enqueue(node);
    }
    int counter = 0;
    int time = 0;
    q1.sortAscByArrivalTime();
    q2.sortAscByArrivalTime();
    q3.sortAscByArrivalTime();
    while (!(q1.isEmpty() && q2.isEmpty() && q3.isEmpty() && R1.isEmpty() && R2.isEmpty() && R3.isEmpty()))
    {
        mlqNode temp1 = q1.gettop();
        if (time == temp1.arrival_time)
        {
            cout << "THE PROCESS " << temp1.id << " HAS ARRIVED AT TIME " << time << endl;
            R1.enqueue(temp1);
            q1.dequeue();
            R1.sortAscByPriority();
        }
        mlqNode temp2 = q2.gettop();
        if (time == temp2.arrival_time)
        {
            cout << "THE PROCESS " << temp2.id << " HAS ARRIVED AT TIME " << time << endl;
            R2.enqueue(temp2);
            q2.dequeue();
        }
        mlqNode temp3 = q3.gettop();
        if (time == temp3.arrival_time)
        {
            cout << "THE PROCESS " << temp3.id << " HAS ARRIVED AT TIME " << time << endl;
            R3.enqueue(temp3);
            R3.sortAscByDuration();
            q3.dequeue();
        }
        if (!R1.isEmpty())
        {
            R1.sortAscByPriority();
            mlqNode temp = R1.gettop();
            R1.dequeue();
            for (int rt = 0; rt < temp.burst_time; rt++)
            {
                mlqNode temp11 = q1.gettop();
                mlqNode temp12 = q2.gettop();
                mlqNode temp13 = q3.gettop();
                sleep(1);
                time++;
                if (time == temp11.arrival_time)
                {
                    cout << "THE PROCESS " << temp11.id << " HAS ARRIVED AT TIME " << time << endl;
                    R1.enqueue(temp11);
                    R1.sortAscByPriority();
                    q1.dequeue();
                }
                if (time == temp12.arrival_time)
                {
                    cout << "THE PROCESS " << temp12.id << " HAS ARRIVED AT TIME " << time << endl;
                    R2.enqueue(temp12);
                    q2.dequeue();
                    R2.sortAscByArrivalTime();
                }
                if (time == temp13.arrival_time)
                {
                    cout << "THE PROCESS " << temp13.id << " HAS ARRIVED AT TIME " << time << endl;
                    R3.enqueue(temp13);
                    R3.sortAscByDuration();
                    q3.dequeue();
                }
            }
            cout << "THE PROCESS " << temp.id << " HAS FINISHED AT TIME " << time << endl;
        }
        else if ((R1.isEmpty()) && (!R2.isEmpty()))
        {
            mlqNode temp2 = R2.gettop();
            R2.dequeue();
            for (int rt = 0; rt < quantum; rt++)
            {
                mlqNode temp21 = q1.gettop();
                mlqNode temp22 = q2.gettop();
                mlqNode temp23 = q3.gettop();
                sleep(1);
                temp2.burst_time--;
                time++;
                if (time == temp21.arrival_time)
                {
                    cout << "THE PROCESS " << temp21.id << " HAS ARRIVED AT TIME " << time << endl;
                    R1.enqueue(temp21);
                    R1.sortAscByPriority();
                    q1.dequeue();
                }
                if (time == temp22.arrival_time)
                {
                    cout << "THE PROCESS " << temp22.id << " HAS ARRIVED AT TIME " << time << endl;
                    R2.enqueue(temp22);
                    q2.dequeue();
                    R2.sortAscByArrivalTime();
                }
                if (time == temp23.arrival_time)
                {
                    cout << "THE PROCESS " << temp23.id << " HAS ARRIVED AT TIME " << time << endl;
                    R3.enqueue(temp23);
                    R3.sortAscByDuration();
                    q3.dequeue();
                }
                if (temp2.burst_time <= 0)
                {
                    break;
                }
            }
            if (temp2.burst_time <= 0)
            {
                cout << "THE PROCESS " << temp2.id << " HAS FINISHED AT TIME " << time << endl;
            }
            else
            {
                R2.enqueue(temp2);
            }
        }
        else if ((R1.isEmpty()) && (R2.isEmpty()) && (!R3.isEmpty()))
        {
            mlqNode temp3 = R3.gettop();
            if (temp3.burst_time <= 0)
            {
                cout << "THE PROCESS " << temp3.id << "HAS FINISHED AT TIME " << time << endl;
                R3.dequeue();
                counter++;
            }
            R3.sortAscByDuration();
            if (!R3.isEmpty())
            {
                temp3 = R3.gettop();
            }
            sleep(1);
            time++;
            temp3.burst_time--;
            if (counter != n)
            {
                R3.dequeue();
                R3.enqueue(temp3);
                R3.sortAscByDuration();
            }
        }
        else
        {
            sleep(1);
            time++;
        }
    }
}
void ps() {
    int n = 4;
    cout << "ENTER THE NUMBER OF PROCESSES " << endl;
    cin >> n;
    int pid;
    int ax;
    int b;
    int p;
    mlqQueue q(n);
    mlqQueue R(n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time and priority";
        cin >> ax >> b>>p;
        mlqNode node = { a + 1,b ,ax,p};
        q.enqueue(node);
    }
    int time = 0;
    q.sortAscByArrivalTime();
    while (!(q.isEmpty() && R.isEmpty()))
    {
        mlqNode temp = q.gettop();
        if (time == temp.arrival_time)
        {
            cout << "THE PROCESS " << temp.id << " HAS ARRIVED AT TIME " << time << endl;
            R.enqueue(temp);
            q.dequeue();
            R.sortAscByPriority();
        }
        if (!R.isEmpty())
        {
            R.sortAscByPriority();
            mlqNode temp = R.gettop();
            R.dequeue();
            for (int rt = 0; rt < temp.burst_time; rt++)
            {
                mlqNode temp2 = q.gettop();
                cout << "executing :" << temp.id << "at time" << time << endl;
                sleep(1);
                time++;
                if (time == temp2.arrival_time)
                {
                    cout << "THE PROCESS " << temp2.id << " HAS ARRIVED AT TIME " << time << endl;
                    R.enqueue(temp2);
                    R.sortAscByPriority();
                    q.dequeue();
                }
            }
            cout << "THE PROCESS " << temp.id << " HAS FINISHED AT TIME " << time << endl;
        }
        else
        {
            sleep(1);
            time++;
        }

    }
    return;
}
void rr() {
    int n = 4;
    cout << "ENTER THE NUMBER OF PROCESSES " << endl;
    cin >> n;
    int pid;
    int ax;
    int b;
    mlqQueue q(n);
    mlqQueue R(2*n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time and priority";
        cin >> ax >> b ;
        mlqNode node = { a + 1,b ,ax,0};
        q.enqueue(node);
    }
    int time = 0;
    cout << "ENTER THE QUANTUM FOR ROUND ROBIN " << endl;
    int quantum;
    cin >> quantum;
    q.sortAscByArrivalTime();
    while (!(q.isEmpty() && R.isEmpty()))
    {
        mlqNode temp = q.gettop();
        if (time == temp.arrival_time)
        {
            cout << "THE PROCESS " << temp.id << " HAS ARRIVED AT TIME " << time << endl;
            R.enqueue(temp);
            q.dequeue();
        }
        if (!R.isEmpty())
        {
            mlqNode temp = R.gettop();
            R.dequeue();
            for (int rt = 0; rt < quantum; rt++)
            {
                mlqNode temp2 = q.gettop();
                cout << "executing :" << temp.id << "at time" << time << endl;
                sleep(1);
                temp.burst_time--;
                time++;
                if (time == temp2.arrival_time)
                {
                    cout << "THE PROCESS " << temp2.id << " HAS ARRIVED AT TIME " << time << endl;
                    R.enqueue(temp2);
                    q.dequeue();
                }
                if (temp.burst_time<=0)
                {
                    break;
                }
            }
            if (temp.burst_time <= 0)
            {
                cout << "THE PROCESS " << temp.id << " HAS FINISHED AT TIME " << time << endl;
            }
            else
            {
                R.enqueue(temp);
            }
        }

        else
        {
            sleep(1);
            time++;
        }

    }
    return;
}
void lrjf()
{
        int n = 3;
        cout<<"enter the no of processes";
        cin>>n;
        int pid;
        int ax;
        int b;
        mlqQueue q(n);
        mlqQueue R(100*n);
        for (int a = 0; a < n; a++)
        {
            cout << "enter the value arrivaltime and burst time ";
            cin >> ax >> b;
            mlqNode node = { a + 1,b,ax };
            q.enqueue(node);
        }
        int time = 0;
        int counter = 0;
        q.sortAscByArrivalTime();
        while (!(q.isEmpty() && R.isEmpty()))
        {
            mlqNode temp = q.gettop();
            if (time == temp.arrival_time)
            {
                cout << "THE PROCESS " << temp.id << " HAS ARRIVED AT TIME " << time << endl;
                R.enqueue(temp);
                R.sortAscByDuration();
                q.dequeue();
            }
            if (!R.isEmpty())
            {
                mlqNode temp2 = R.gettop();
                if (temp2.burst_time <= 0)
                {
                    cout << "THE PROCESS " << temp2.id << "HAS FINISHED AT TIME " << time << endl;
                    R.dequeue();
                    counter++;
                }
                R.sortAscByDuration();
                if (!R.isEmpty())
                {
                    temp2 = R.gettop();
                }
                sleep(1);
                time++;
                temp2.burst_time--;
                if (counter != n)
                {
                    R.dequeue();
                    R.enqueue(temp2);
                    R.sortAscByDuration();
                }
            }
            else
            {
                sleep(1);
                time++;
            }
        }
        return;
}
struct mlfqNode {
    int id;
    int burst_time;
    int arrival_time;
    int waiting_time;
};

class mlfqQueue {
public:
    int front, rear;
    int size;
    mlfqNode* arr;
    mlfqQueue(int s) {
        front = rear = -1;
        size = s;
        arr = new mlfqNode[size];
    }

    bool isFull() {
        return (rear == size - 1);
    }
    bool isEmpty() {
        return (front == -1 || front > rear);
    }
    void enqueue(mlfqNode node) {
        if (isFull()) {
            cout << "Queue is full\n";
            return;
        }
        rear++;
        arr[rear] = node;
        if (front == -1) {
            front = 0;
        }
    }
    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty\n";
            return;
        }

        front++;
    }
    mlfqNode gettop()
    {
        if (!isEmpty())
        {
            mlfqNode temp = arr[front];
            return temp;
        }
        else
        {
            return mlfqNode();
        }
    }
    void sortAscByArrivalTime() {
        sort(arr + front, arr + rear + 1, [](mlfqNode a, mlfqNode b) { return a.arrival_time < b.arrival_time; });
    }
    void sortDescByDuration() {
        sort(arr + front, arr + rear + 1, [](mlfqNode a, mlfqNode b) { return a.burst_time < b.burst_time; });
    }
    void incrementWaitingTime() {
        for (int i = front; i <= rear; i++) {
            arr[i].waiting_time++;
        }
    }


};
void mlfq()
{
    int n = 4;
    cout << "ENTER THE NUMBER OF PROCESSES FOR FCFS QUEUE " << endl;
    cin >> n;
    int pid;
    int ax;
    int b;
    int az = 0;
    mlfqQueue q1(n);
    mlfqQueue R1(10*n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time ";
        cin >> ax >> b;
        mlfqNode node = { az + 1,b ,ax,0 };
        az++;
        q1.enqueue(node);
    }
    cout << "ENTER THE NUMBER OF PROCESSES FOR SJF " << endl;
    cin >> n;
    mlfqQueue q2(n);
    mlfqQueue R2(10*n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time ";
        cin >> ax >> b;
        mlfqNode node = { az + 1,b ,ax,0 };
        az++;
        q2.enqueue(node);
    }
    cout << "ENTER THE QUANTUM FOR ROUND ROBIN " << endl;
    int quantum;
    cin >> quantum;
    cout << "ENTER THE NUMBER OF PROCESSES FOR THE ROUND ROBIN (SRJF) " << endl;
    cin >> n;
    mlfqQueue q3(n);
    mlfqQueue R3(100 * n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time ";
        cin >> ax >> b;
        mlfqNode node = { az + 1,b,ax };
        az++;
        q3.enqueue(node);
    }
    int time = 0;
    int lthold = 5;
    int mthold = 10;
    int hthold = 15;
    q1.sortAscByArrivalTime();
    q2.sortAscByArrivalTime();
    q3.sortAscByArrivalTime();
    while (!(q1.isEmpty() && q2.isEmpty() && q3.isEmpty() && R1.isEmpty() && R2.isEmpty() && R3.isEmpty()))
    {
        mlfqNode temp1 = q1.gettop();
        if (time == temp1.arrival_time)
        {
            cout << "THE PROCESS " << temp1.id << " HAS ARRIVED AT TIME " << time << endl;
            R1.enqueue(temp1);
            q1.dequeue();
            R1.sortAscByArrivalTime();
        }
        mlfqNode temp2 = q2.gettop();
        if (time == temp2.arrival_time)
        {
            cout << "THE PROCESS " << temp2.id << " HAS ARRIVED AT TIME " << time << endl;
            R2.enqueue(temp2);
            q2.dequeue();
            R2.sortDescByDuration();
        }
        mlfqNode temp3 = q3.gettop();
        if (time == temp3.arrival_time)
        {
            cout << "THE PROCESS " << temp3.id << " HAS ARRIVED AT TIME " << time << endl;
            R3.enqueue(temp3);
            R3.sortDescByDuration();
            q3.dequeue();
        }
        if (!R1.isEmpty())
        {
                mlfqNode tempo = R1.gettop();
                R1.dequeue();
                if (tempo.waiting_time < mthold)
                {
                    tempo.waiting_time = 0;
                    R2.enqueue(tempo);
                    R2.sortDescByDuration();
                }
                else
                {
                    R1.enqueue(tempo);
                }
                R1.sortAscByArrivalTime();
        }
        if(!R2.isEmpty())
        {
            mlfqNode tempo = R2.gettop();
            R2.dequeue();
            if (tempo.waiting_time < lthold)
            {
                tempo.waiting_time = 0;
                R3.enqueue(tempo);
                R3.sortDescByDuration();
            }
            else if (tempo.waiting_time > hthold)
            {
                tempo.waiting_time = 0;
                R1.enqueue(tempo);
                R1.sortAscByArrivalTime();
            }
            else
            {
                R2.enqueue(tempo);
            }
            R2.sortDescByDuration();
        }
        if (!R3.isEmpty())
        {
            mlfqNode tempo = R3.gettop();
            R3.dequeue();
            if (tempo.waiting_time > lthold && tempo.waiting_time<mthold)
            {
                tempo.waiting_time = 0;
                R2.enqueue(tempo);
                R2.sortDescByDuration();
            }
            else if (tempo.waiting_time > hthold)
            {
                tempo.waiting_time = 0;
                R1.enqueue(tempo);
                R1.sortAscByArrivalTime();
            }
            else
            {
                R3.enqueue(tempo);
            }
            R3.sortDescByDuration();
        }
        if (!R1.isEmpty())
        {
            mlfqNode temp = R1.gettop();
            R1.dequeue();
            for (int rt = 0; rt < temp.burst_time; rt++)
            {
                mlfqNode temp11 = q1.gettop();
                mlfqNode temp12 = q2.gettop();
                mlfqNode temp13 = q3.gettop();
                if (time == temp11.arrival_time)
                {
                    cout << "THE PROCESS " << temp11.id << " HAS ARRIVED AT TIME " << time << endl;
                    R1.enqueue(temp11);
                    q1.dequeue();
                    R1.sortAscByArrivalTime();
                }
                if (time == temp12.arrival_time)
                {
                    cout << "THE PROCESS " << temp12.id << " HAS ARRIVED AT TIME " << time << endl;
                    R2.enqueue(temp12);
                    q2.dequeue();
                    R2.sortDescByDuration();
                }
                if (time == temp13.arrival_time)
                {
                    cout << "THE PROCESS " << temp13.id << " HAS ARRIVED AT TIME " << time << endl;
                    R3.enqueue(temp13);
                    R3.sortDescByDuration();
                    q3.dequeue();
                }
                sleep(1);
                time++;
                R1.incrementWaitingTime();
                R2.incrementWaitingTime();
                R3.incrementWaitingTime();

            }
            cout << "THE PROCESS " << temp.id << " HAS FINISHED AT TIME " << time << endl;
        }
        else if ((R1.isEmpty()) && (!R2.isEmpty()))
        {
            R2.sortDescByDuration();
            mlfqNode temp = R2.gettop();
            R2.dequeue();
            for (int rt = 0; rt < temp.burst_time; rt++)
            {
                mlfqNode temp11 = q1.gettop();
                mlfqNode temp12 = q2.gettop();
                mlfqNode temp13 = q3.gettop();
                if (time == temp11.arrival_time)
                {
                    cout << "THE PROCESS " << temp11.id << " HAS ARRIVED AT TIME " << time << endl;
                    R1.enqueue(temp11);
                    q1.dequeue();
                    R1.sortAscByArrivalTime();
                }
                if (time == temp12.arrival_time)
                {
                    cout << "THE PROCESS " << temp12.id << " HAS ARRIVED AT TIME " << time << endl;
                    R2.enqueue(temp12);
                    q2.dequeue();
                    R2.sortDescByDuration();
                }
                if (time == temp13.arrival_time)
                {
                    cout << "THE PROCESS " << temp13.id << " HAS ARRIVED AT TIME " << time << endl;
                    R3.enqueue(temp13);
                    R3.sortDescByDuration();
                    q3.dequeue();
                }
                sleep(1);
                time++;
                R1.incrementWaitingTime();
                R2.incrementWaitingTime();
                R3.incrementWaitingTime();
            }
            cout << "THE PROCESS " << temp.id << " HAS FINISHED AT TIME " << time << endl;
        }
        else if ((R1.isEmpty()) && (R2.isEmpty()) && (!R3.isEmpty()))
        {
            mlfqNode temp = R3.gettop();
            R3.dequeue();
            for (int rt = 0; rt < quantum; rt++)
            {
                mlfqNode temp11 = q1.gettop();
                mlfqNode temp12 = q2.gettop();
                mlfqNode temp13 = q3.gettop();
                if (time == temp11.arrival_time)
                {
                    cout << "THE PROCESS " << temp11.id << " HAS ARRIVED AT TIME " << time << endl;
                    R1.enqueue(temp11);
                    q1.dequeue();
                    R1.sortAscByArrivalTime();
                }
                if (time == temp12.arrival_time)
                {
                    cout << "THE PROCESS " << temp12.id << " HAS ARRIVED AT TIME " << time << endl;
                    R2.enqueue(temp12);
                    q2.dequeue();
                    R2.sortDescByDuration();
                }
                if (time == temp13.arrival_time)
                {
                    cout << "THE PROCESS " << temp13.id << " HAS ARRIVED AT TIME " << time << endl;
                    R3.enqueue(temp13);
                    R3.sortDescByDuration();
                    q3.dequeue();
                }
                sleep(1);
                temp.burst_time--;
                time++;
                R1.incrementWaitingTime();
                R2.incrementWaitingTime();
                R3.incrementWaitingTime();
                if (temp.burst_time <= 0)
                {
                    break;
                }
            }
            if (temp.burst_time <= 0)
            {
                cout << "THE PROCESS " << temp.id << " HAS FINISHED AT TIME " << time << endl;
            }
            else
            {
                R3.enqueue(temp);
                R3.sortDescByDuration();
            }
        }
        else
        {
            sleep(1);
            time++;
        }
    }
    return;
}
void rrsj()
{
    int n = 4;
    cout << "ENTER THE NUMBER OF PROCESSES " << endl;
    cin >> n;
    int pid;
    int ax;
    int b;
    mlfqQueue q(n);
    mlfqQueue R(10 * n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time ";
        cin >> ax >> b;
        mlfqNode node = { a + 1,b ,ax,0 };
        q.enqueue(node);
    }
    int time = 0;
    cout << "ENTER THE QUANTUM FOR ROUND ROBIN " << endl;
    int quantum;
    cin >> quantum;
    q.sortAscByArrivalTime();
    while (!(q.isEmpty() && R.isEmpty()))
    {
        mlfqNode temp = q.gettop();
        if (time == temp.arrival_time)
        {
            cout << "THE PROCESS " << temp.id << " HAS ARRIVED AT TIME " << time << endl;
            R.enqueue(temp);
            R.sortDescByDuration();
            q.dequeue();
        }
        if (!R.isEmpty())
        {
            mlfqNode temp = R.gettop();
            R.dequeue();
            for (int rt = 0; rt < quantum; rt++)
            {
                mlfqNode temp2 = q.gettop();
                cout << "executing :" << temp.id << "at time" << time << endl;
                sleep(1);
                temp.burst_time--;
                time++;
                if (time == temp2.arrival_time)
                {
                    cout << "THE PROCESS " << temp2.id << " HAS ARRIVED AT TIME " << time << endl;
                    R.enqueue(temp2);
                    R.sortDescByDuration();
                    q.dequeue();
                }
                if (temp.burst_time <= 0)
                {
                    break;
                }
            }
            if (temp.burst_time <= 0)
            {
                cout << "THE PROCESS " << temp.id << " HAS FINISHED AT TIME " << time << endl;
            }
            else
            {
                R.enqueue(temp);
                R.sortDescByDuration();
            }
        }

        else
        {
            sleep(1);
            time++;
        }

    }
    return;
}
void sjf()
{
    int n = 4;
    cout << "ENTER THE NUMBER OF PROCESSES " << endl;
    cin >> n;
    int pid;
    int ax;
    int b;
    mlfqQueue q(n);
    mlfqQueue R(n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time ";
        cin >> ax >> b;
        mlfqNode node = { a + 1,b,ax };
        q.enqueue(node);
    }
    int time = 0;
    q.sortAscByArrivalTime();
    while (!(q.isEmpty() && R.isEmpty()))
    {
        mlfqNode temp = q.gettop();
        if (time == temp.arrival_time)
        {
            cout << "THE PROCESS " << temp.id << " HAS ARRIVED AT TIME " << time << endl;
            R.enqueue(temp);
            q.dequeue();
            R.sortDescByDuration();
        }
        if (!R.isEmpty())
        {
            R.sortDescByDuration();
            mlfqNode temp = R.gettop();
            R.dequeue();
            for (int rt = 0; rt < temp.burst_time; rt++)
            {
                mlfqNode temp2 = q.gettop();
                if (time == temp2.arrival_time)
                {
                    cout << "THE PROCESS " << temp2.id << " HAS ARRIVED AT TIME " << time << endl;
                    R.enqueue(temp2);
                    R.sortDescByDuration();
                    q.dequeue();
                }
                cout << "executing :" << temp.id << "at time" << time << endl;
                sleep(1);
                time++;
            }
            cout << "THE PROCESS " << temp.id << " HAS FINISHED AT TIME " << time << endl;
        }
        else
        {
            sleep(1);
            time++;
        }

    }
    return;
}
void fcfs() {
    int n = 4;
    cout << "ENTER THE NUMBER OF PROCESSES " << endl;
    cin >> n;
    int pid;
    int ax;
    int b;
    int p;
    mlfqQueue q(n);
    mlfqQueue R(n);
    for (int a = 0; a < n; a++)
    {
        cout << "enter the value arrivaltime and burst time";
        cin >> ax >> b;
        mlfqNode node = { a + 1,b ,ax,0 };
        q.enqueue(node);
    }
    int time = 0;
    q.sortAscByArrivalTime();
    while (!(q.isEmpty() && R.isEmpty()))
    {
        mlfqNode temp = q.gettop();
        if (time == temp.arrival_time)
        {
            cout << "THE PROCESS " << temp.id << " HAS ARRIVED AT TIME " << time << endl;
            R.enqueue(temp);
            q.dequeue();
        }
        if (!R.isEmpty())
        {
            mlfqNode temp = R.gettop();
            R.dequeue();
            for (int rt = 0; rt < temp.burst_time; rt++)
            {
                mlfqNode temp2 = q.gettop();
                cout << "executing :" << temp.id << "at time" << time << endl;
                sleep(1);
                time++;
                if (time == temp2.arrival_time)
                {
                    cout << "THE PROCESS " << temp2.id << " HAS ARRIVED AT TIME " << time << endl;
                    R.enqueue(temp2);
                    q.dequeue();
                }
            }
            cout << "THE PROCESS " << temp.id << " HAS FINISHED AT TIME " << time << endl;
        }
        else
        {
            sleep(1);
            time++;
        }

    }
    return;
}
void scheduling()
{
     system("clear");
    cout<<"enter 1 for longest job first \n";
    cout<<"enter 2 for longest feedback ratio first \n";
    cout<<"enter 3 for MLQ  \n";
    cout<<"enter 4 for MLFQ \n";
    cout<<"enter 5 for priority scheduling \n";
    cout<<"enter 6 for round robin \n";
    cout<<"enter 7 for shortest remaining job first \n";
    cout<<"enter 8 for shortest remaining round robin \n";
    cout<<"enter 9 for shortest job first \n";
    cout<<"enter 10 for first come first serve \n";
    cout<<"enter anything else for return \n";
    int choice =0;
    cin>>choice;
    if(choice==1)
    {ljf();}
    else if(choice==2)
    {lfr();}
    else if(choice==3)
    {mlq();}
    else if(choice==4)
    {mlfq();}
    else if(choice==5)
    {ps();}
    else if(choice==6)
    {rr();}
    else if(choice==7)
    {lrjf();}
    else if(choice==8)
    {rrsj();}
    else if(choice==9)
    {sjf();}
    else if(choice==10)
    {fcfs();}
}

struct node
{
	int data;
	bool ss;
	node* next;
};
class circlelist
{
public:
	node* head;
public:
	circlelist()
	{
		head = NULL;
	}
	void insert(int a)
	{
		node* current = head;
		node* temp = new node;
		temp->data = a;
		temp->ss = 1;
		if (head == NULL)
		{
			head = temp;
			head->next = head;
		}
		else
		{
			while (current->next != head)
			{
				current = current->next;
			}
			current->next = temp;
			temp->next = head;
		}
	}
	void print()
	{
		node* current = head;
		while (current->next != head)
		{
			cout << current->data << " ";
			current = current->next;
		}
		cout << endl;
	}
	bool search(int a)
	{
		bool found = false;
		node* current = head;
		while (current->next != head)
		{
			if (current->data == a)
			{
				found = true;
				current->ss = 1;
			}
			current = current->next;
		}
		if (found)
			return true;
		else
			return false;
	}

};
void secondchance()
{
         system("clear");
	int NPF=0;
	queue <int>obj1;
	circlelist obj;
	int pmem = 0;
	int qstream = 0;
	cout << "ENTER THE NUMBER OF PAGE INPUTS";
	cin >> qstream;
	cout << "enter the max no of pages at a time in memory";
	cin >> pmem;
	int p;
	for (int a = 0; a < qstream; a++)
	{
		cout << "ENTER PAGE NO: ";
		cin >> p;
		obj1.push(p);
	}
	for (int b = 0; b < pmem; b++)
	{
		obj.insert(obj1.front());
		NPF++;
		obj1.pop();
	}
	node* tempo=obj.head;
	while (!obj1.empty())
	{
		if (obj.search(obj1.front()))
		{
			cout << ">PAGE ALREADY EXISTS<" << endl;
			obj1.pop();
		}
		else if (tempo->ss == 1)
		{
			tempo->ss = 0;
			tempo = tempo->next;
		}
		else if (tempo->ss == 0)
		{
			cout << "PAGE : " << tempo->data << " Replaced with PAGE :" << obj1.front() << endl;
			tempo->data = obj1.front();
			tempo->ss = 1;
			tempo = tempo->next;
			obj1.pop();
			NPF++;
		}
	}
	float prob = (float)NPF/qstream;
	float percentage = prob * 100;
	cout << "TOTAL NO OF PAGE FAULTS ARE " << NPF<<endl;
	cout << "PROBABILITY OF PAGE FAULT IS " <<prob ;
	cout << "PERCENTAGE OF PAGE FAULT IS" << percentage;
}
int size_of_frame_array;
int ram1;
class frame_queue {
public:
    int* arr;
    int front, capacity;
public:
    frame_queue() 
    {
       int framesize = 16;
       double log_n;
       cout << "Enter the size of ram (power of 2) : ";
       cin >> ram1;
       log_n = log2(ram1);
       while (floor(log_n) != log_n)
       {
           cout << "Invalid input. Please enter a power of two: ";
           cin >> ram1;
           log_n = log2(ram1);
       }
        size_of_frame_array = ram1 / framesize;
        capacity = size_of_frame_array;
        arr = new int[capacity];
        for (int a = 0; a < capacity; a++)
        {
            arr[a] = -99;
        }
        front = -1;
    }

    void enqueue(int value) 
    {
        front++;
        front = front % capacity;
        arr[front] = value;
    }
    int gettop()
    {
        return front;
    }
    void print()
    {
        for (int a = 0; a < capacity; a++)
        {
            if (arr[a] != -99)
            {
                cout << arr[a] << " ";
            }
        }
    }
};
int tlbpage;
frame_queue objxyz;
class HashTable {
private:
    int size;
    int* table; 
    int* frameloc;
    int sindex;
public:
    HashTable()
    {
        this->size = size_of_frame_array;
        table = new int[size];
        frameloc = new int[size];
        for (int i = 0; i < size; i++)
        {
            table[i] = -1; 
            frameloc[i] = -1;
        }
    }

   void insert(int psize, int id) 
   {
       int turn = 1;
        int idx = psize % size;
        while (psize > 0)
        {
            while (table[idx] != -1)
            {
                idx++;
                idx = idx % size;
            }
            if (turn == 1)
            {
                sindex = idx;
            }
            turn++;
            table[idx] = id;
            objxyz.enqueue(id);
            frameloc[idx] = objxyz.gettop();
            psize = psize - 16;
        }
    }
   void addressget(int id, int psize)
   {
       int idx = sindex;
       int size = psize;
       while (size > 0)
       {
           idx++;
           idx = idx % size_of_frame_array;
           size = size - 16;
       }
       idx--;
       int offset = psize % 16;
       int loc = frameloc[idx];
       int pmem = (loc * 16) + offset;
       if (id == objxyz.arr[loc])
       {
           tlbpage = loc;
           cout << "in page table it is at PAGE NO : " << idx << "\t and this page is in ram at loc " << loc<<"\t it maps to physcial memory :  "<<pmem << endl;
       }
       else
       {
           cout << "in page table it is at PAGE NO : " << idx << "\t but it has been replaced at RAM " << endl;
       }
   }
   void print()
   {
       for (int a = 0; a < size; a++)
       {
           if (table[a] != -1)
           {
               cout << a << " ";
           }
       }
       cout << endl;
       for (int a = 0; a < size; a++)
       {
              cout << frameloc[a] << " ";
       }
       cout << endl;
   }
};
class hashpaging 
{
public:
    int id;
    int psize;
    HashTable obj;
public:
    hashpaging()
    {
        id = 0;
        psize = 0;
    }
    void set_values(int xid, int xpsize)
    {
        id = xid;
        psize = xpsize;
        obj.insert(psize, id);
    }
    void find_address(int ladd)
    {
        obj.addressget(id,ladd);
    }
};
class tlb 
{
private:
    int* arr;
    int* sarr;
public:
    tlb(int s)
    {
        arr = new int[s];
        sarr = new int[s];
        for (int a = 0; a < s; a++)
        {
            arr[a] = -1;
            sarr[a] = -1;
        }
    }
    void tlbset(int idx,int size)
    {
        arr[idx] = tlbpage;
        int a = 0;
        while (size > 0)
        {
            size = size - 16;
            a++;
        }
        sarr[idx] = a;
    }
    bool search(int isize,int idx)
    {
        int size = isize;
        int offset = isize % 16;
        int a = 0;
        while (size > 0)
        {
            size = size - 16;
            a++;
        }
        if (a == sarr[idx])
        {
            cout << "TLB HIT" << endl;
            int loc = arr[idx];
            int pmem = (loc * 16) + offset;
            cout << "\t this page is in ram at loc " << loc << "\t it maps to physcial memory :  " << pmem << endl;
            return true;
        }
        else
        {
            cout << "TLB MISS" << endl;
            return false;
        }
    }
};

void hashedpaging()
{
    int no_of_p;
    int id;
    int psize;
    cout << "ENTER HOW MANY PROCESSES YOU WANT TO ENTER" << endl;
    cin >> no_of_p;
    tlb obs(no_of_p);
    hashpaging *obj = new hashpaging[no_of_p];
    for (int a = 0; a < no_of_p; a++)
    {
        cout << "ENTER ID AND SIZE OF PROCESS" << endl;
        id = a + 1;
        cin >> psize;
        obj[a].set_values(id, psize);
    }
    int ch = 1;
    int x;
    int add;
    while (ch == 1)
    {
        cout << "enter the process you want to search range is 1 -> " << no_of_p << endl;
        cin >> x;
        x = x - 1;
        cout << "enter the logical memory address range is 0 -> " << obj[x].psize - 1<<endl;
        cin >> add;
        if (add >= obj[x].psize)
        {
            cout << "DOES NOT EXIST";
        }
        else
        {
            if (obs.search(add, x))
            {
                cout << "THIS WAS FETCHED FROM TLB instead of storage" << endl;
            }
            else
            {
                obj[x].find_address(add);
                obs.tlbset(x, add);
            }
        }
        cout << "ENTER 1 if you want to search more else enter anything else" << endl;
        cin >> ch;
    }
}
int size_of_frame_array2;
int ram2;
class l2frame_queue {
public:
    int* arr;
    int front, capacity;
public:
    l2frame_queue()
    {
        int framesize = 16;
        double log_n;
        cout << "Enter the size of ram (power of 2) : ";
        cin >> ram2;
        log_n = log2(ram2);
        while (floor(log_n) != log_n)
        {
            cout << "Invalid input. Please enter a power of two: ";
            cin >> ram2;
            log_n = log2(ram2);
        }
        size_of_frame_array2 = ram2 / framesize;
        capacity = size_of_frame_array2;
        arr = new int[capacity];
        for (int a = 0; a < capacity; a++)
        {
            arr[a] = -99;
        }
        front = -1;
    }

    void enqueue(int value)
    {
        front++;
        front = front % capacity;
        arr[front] = value;
    }
    int gettop()
    {
        return front;
    }
    void print()
    {
        for (int a = 0; a < capacity; a++)
        {
            if (arr[a] != -99)
            {
                cout << arr[a] << " ";
            }
        }
    }
};
l2frame_queue objxyz2;
int getpowerof2(int xn)
{
    int x = 0;
    int temp = 1;
    while (temp < xn)
    {
        temp = temp * 2;
        x++;
    }
    return x;
}
bool l2_flag;
int l2_getsizei(int xn)
{
    int innersize;
    int outersize;
    int r = getpowerof2(xn);
    if (r <= 4)
    {
        innersize = 1;
        outersize = 1;
        return innersize;
    }
    r = r - 4;
    if (r % 2 == 0)
    {
        r = r / 2;
        innersize = pow(2, r);
        outersize = pow(2, r);
    }
    else
    {
        int r1 = r / 2;
        int r2 = r / 2;
        r2++;
        innersize = pow(2, r2);
        outersize = pow(2, r1);
    }
    return innersize;
}
int l2_getsizeo(int xn)
{
    int innersize;
    int outersize;
    int r = getpowerof2(xn);
    if (r <= 4)
    {
        innersize = 1;
        outersize = 1;
        return outersize;
    }
    r = r - 4;
    if (r % 2 == 0)
    {
        r = r / 2;
        innersize = pow(2, r);
        outersize = pow(2, r);
    }
    else
    {
        int r1 = r / 2;
        int r2 = r / 2;
        r2++;
        innersize = pow(2, r2);
        outersize = pow(2, r1);
    }
    return outersize;
}
class innertable
{
public:
    int itsize;
    int* iarr;
    innertable()
    {
        itsize = 0;
        iarr = new int[itsize];
        for (int a = 0; a < itsize; a++)
        {
            iarr[a] = -99;
        }
    }
    void initialize(int x)
    {
        itsize = x;
        iarr = new int[itsize];
        for (int a = 0; a < itsize; a++)
        {
            iarr[a] = -99;
        }
    }
    void insertion(int id, int& l2_psize)
    {
        for (int a = 0; a < itsize; a++)
        {
            if (l2_psize < 0)
            {
                l2_flag = false;
                break;
            }
            objxyz2.enqueue(id);
            iarr[a] = objxyz2.gettop();
            l2_psize = l2_psize - 16;
        }
    }

    void print()
    {
        for (int a = 0; a < itsize; a++)
        {
            cout << iarr[a] << " ";
        }
        cout << endl;
    }

};
class outertable
{
public:
    int otsize;
    int isize;
    int l2_psize;
    innertable* arr;
    outertable()
    {
        otsize = 0;
        isize = 0;
        arr = new innertable[otsize];
    }
    void initialize(int x)
    {
        l2_psize = x;
        otsize = l2_getsizeo(x);
        isize = l2_getsizei(x);
        arr = new innertable[otsize];
        for (int a = 0; a < otsize; a++)
        {
            arr[a].initialize(isize);
        }
    }
    void insertion(int id)
    {
        for (int a = 0; a < otsize; a++)
        {
            if (l2_flag == false)
            {
                break;
            }
            arr[a].insertion(id, l2_psize);
        }
    }
    void print()
    {
        for (int b = 0; b < otsize; b++)
        {
            arr[b].print();
        }
    }
    void search(int x, int xsize)
    {
        int temp = isize * 16;
        int offset = xsize % 16;
        int idx1 = xsize / temp;
        int idx2 = (xsize - (idx1 * temp)) / 16;
        int loc = arr[idx1].iarr[idx2];
        int pmem = (loc * 16) + offset;
        if (objxyz2.arr[loc] == x)
        {
            cout << "IT IS PRESENT AT FRAME NO: " << loc << endl;
            cout << " ITs physical memory address is " << pmem << endl;
        }
        else
        {
            cout << "It has been either replaced or not loaded at all" << endl;
        }
    }
};
class l2process
{
public:
    int size;
    int id;
    outertable obj;
    l2process()
    {
        size = 0;
        id = 0;
    }
    void setsize(int x, int s)
    {
        size = s;
        id = x;
        obj.initialize(size);
    }
    void insert()
    {
        l2_flag = true;
        obj.insertion(id);
    }
    void print()
    {
        obj.print();
    }
    void search(int xsize)
    {
        obj.search(id, xsize);
    }
};
void l2paging()
{
    l2process* obj = new l2process;
    int n;
    cout << "enter how many processes you want to enter";
    cin >> n;
    int size;
    for (int a = 0; a < n; a++)
    {
        cout << "enter the size of process" << endl;
        cin >> size;
        while (size >= ram2)
        {
            cout << "size is too big enter again" << endl;
            cin >> size;
        }
        obj[a].setsize(a + 1, size);
        obj[a].insert();
    }
    int choice = 1;
    int xn;
    int add;
    while (choice == 1)
    {
        cout << "enter -1 to exit or the no of process you want to search range is 1 to " << n;
        cin >> xn;
        if (xn == -1)
        {
            break;
        }
        while (xn <= 0 || xn > n)
        {
            cout << "WRONG INPUT enter again" << endl;
            cin >> xn;
        }
        cout << "enter the logical address to search limit is" << obj[xn - 1].size;
        cin >> add;
        while (add < 0 || add >= obj[xn - 1].size)
        {
            cout << "WRONG INPUT enter again" << endl;
            cin >> add;
        }
        obj[xn - 1].search(add);
    }
}
void page_table_implement()
{
     system("clear");
    cout << "enter what type of paging you want to see 1 for two level 2 for hashed anything else to return to menu" << endl;
    int c;
    cin >> c;
    if (c == 1)
    {
        cout << "VIEWING 2 level paging" << endl;
        l2paging();
    }
    else if (c == 2)
    {   
        cout << "VIEWING hashed paging" << endl;
        hashedpaging();
    }
}
int n, N;
int** Max;
int** need;
int** all;
int* avl;
int* exe;
int c = 0;
int cnt;
bool flag ;
int m = 0;
int t=0;
int* req;
bool rn=true;
bool ra=true;
bool re = false;
void safetyalgo()
{
	cnt = n;
	while(cnt>0)
	{
		flag = true;
		re = false;
		for (int j = 0; j < N; j++)
		{
			for (int i = 0; i < (c-1); i++)
			{
				if (m == exe[i])
				{
					re = true;
					break;
				}
			}
			if (need[m][j] > avl[j])
			{
				flag = false;
				break;
            }
		}
		if (flag == true && re==false)
		{
			exe[c] = m;
			c++;
			for (int k = 0; k < N; k++)
			{
				avl[k] = avl[k] + all[m][k];
			}
			cnt--;
		}
		m++;
		m = m % n;
	}
	if (cnt == 0)
	{
		cout << "Safety Algo is: "<<endl;
		for (int i = 0; i < n; i++)
		{
			cout << "Process " << exe[i] << endl;
		}
	}
	else
	{
		cout << "Safety Algorithm failed to execute all processes" << endl;
		for (int i = 0; i < c; i++)
		{
			cout << "Process " << exe[i] << endl;
		}
	}
}
void reqalgo()
{
	for (int i = 0; i < N; i++)
	{
		rn = true;
		if (req[i] > need[t][i] )
		{
			rn = false;
			cout << "Request Greater than Need, Can't apply Request Algorithm"<<endl;
			break;
		}
	}
	for (int i = 0; i < N; i++)
	{
		ra = true;
		if (req[i] > avl[i] )
		{
			ra = false;
			cout << "Request Greater than Available, Can't apply Request Algorithm" << endl;
			break;
		}
	}
	if (ra == true && rn == true)
	{
		for (int i = 0; i < N; i++)
		{
			avl[i] = avl[i] - req[i];
			all[t][i] = all[t][i] + req[i];
			need[t][i] = need[t][i] - req[i];
		}
		safetyalgo();
	}
}
void input()
{
        cout << "Enter Number of Processes: ";
	cin >> n;
	cout << "Enter Number of Resources: ";
	cin >> N;
	exe = new int[n];

	Max = new int* [n];
	for (int i = 0; i < n; i++)
	{
		Max[i] = new int[N];
	}
	for (int i = 0; i < n; i++)
	{
		cout << "Enter Resource Value for Process " << i  << " for max: ";
		for (int j = 0; j < N; j++)
		{
			cin >> Max[i][j];
		}
	}
	all = new int* [n];
	for (int i = 0; i < n; i++)
	{
		all[i] = new int[N];
	}
	for (int i = 0; i < n; i++)
	{
		cout << "Enter Resource Value for Process " << i << " for allocation: ";
		for (int j = 0; j < N; j++)
		{
			cin >> all[i][j];
		}
	}

	avl = new int[N];
	cout << "Enter Available Resources: ";
	for (int i = 0; i < N; i++)
	{
		cin >> avl[i];
	}
	need = new int* [n];
	for (int i = 0; i < n; i++)
	{
		need[i] = new int[N];
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < N; j++)
		{
			need[i][j] = Max[i][j] - all[i][j];
		}
	}
	
	
}
void banker_implement()
{
         system("clear");
	int in;
	cout<<"Enter 1 for Safety Algorithm, 2 for Resource Request Algorithm: ";
	cin>>in;
	if(in==1)
	{
	        input();
	        safetyalgo();
	}
	else if(in==2)
	{
	        input();
	        req = new int[N];
	        cout << "Enter Process Number to Request: ";
	        cin >> t;
	        cout << "Enter Resources to Request: ";
	        for (int i = 0; i < N; i++)
	        {
	        	cin >> req[i];
	        }
	        reqalgo();
	}
	return;
}
const int numofph = 5;
int chopsticks[numofph];
mutex mtx;
int num;

void wait(int* s)
{
    while (*s <= 0)
    {

    }
    *s = *s - 1;

}

void signal(int* s)
{
    *s = *s + 1;
}

void philosopher(int *no)
{
    int n = *no;
    for(int i=0;i<num;i++)
    {
        mtx.lock();
        cout << "Philosopher " << n << " is thinking" << endl;
        usleep(2000000);
        mtx.unlock();
        mtx.lock();
        cout << "Philosopher " << n << " wants to eat" << endl;
        mtx.unlock();
        mtx.lock();
        wait(&chopsticks[n]);
        cout << "Philosopher " << n << " has picked up left chopstick" << endl;
        wait(&chopsticks[(n + 1) % numofph]);
        cout << "Philosopher " << n << " has picked up right chopstick" << endl;
        cout << "Philosopher " << n << " is eating now" << endl;
        usleep(2000000);
        signal(&chopsticks[n]);
        cout << "Philosopher " << n << " has released left chopstick" << endl;
        signal(&chopsticks[(n + 1) % numofph]);
        cout << "Philosopher " << n << " has released right chopstick" << endl;
        mtx.unlock();
    }

}

void dining_implement()
{
     system("clear");
    cout << "Enter the number of times a philosopher can eat: ";
    cin >> num;
    for (int i = 0; i < numofph; i++)
    {
        chopsticks[i] = 1;
    }
    pthread_t philosophers[numofph];
    int ph_num[numofph];
    for (int i = 0; i < numofph; i++)
    {
        ph_num[i]=i;
        pthread_create(&philosophers[i], NULL, (void* (*)(void*)) philosopher, &ph_num[i]);
    }

    for (int i = 0; i < numofph; i++) 
    {
        pthread_join(philosophers[i], NULL);
    }
    return;
}
int main()
{
  int choice=0;
  while(choice!=-1)
  {
    cout<<endl<<endl;
    cout<<"enter 1 for scheduling 2 for second_chance 3 for page table 4 for bankers 5 for dining philosopher -1 to exit "<<endl;
    cin>>choice;
    if(choice==1)
    {scheduling();}
    else if(choice==2)
    {secondchance();}
    else if(choice==3)
    {page_table_implement();}
    else if(choice==4)
    {banker_implement();}
    else if(choice==5)
    {dining_implement();}
    else if(choice==-1)
    {break;}
    else
    {
      cout<<"wrong input"<<endl;  
    }
  }
}