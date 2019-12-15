#include "Car.hpp"
#include "Edge.hpp"
#include <sstream>
#include <vector>
#include <thread>
#include <semaphore.h>
#include <unistd.h> 
#include <ctime>
#include <chrono>

#include <fstream>

#define READING_STATE_DELIMITER "#"
#define DELIMITER '-'

#include <iostream>


using namespace std;
vector<Edge* > pathes;
vector<sem_t> locks;
vector<thread> threads;
vector<Car* > cars;
double total_emission = 0;
sem_t emission_sem;


double calculate_pollution(double car_pollution, double hardness){
    double pollution = 0;
    for(long int k = 0; k < 1e7; k++){
        pollution += floor(k / (1e6*car_pollution*hardness));
    }
    return pollution;
}

void move_car(Car* car)
{
    ofstream ofile;
    double pollution = 0;
    string file_name = car->generate_file_name();
    ofile.open(file_name, ios::trunc);
    vector<Edge*> car_path = car->get_path();
    for (int i = 0;i < car_path.size(); i++)
    {
        for (int j = 0;j < pathes.size(); j++)
        {
            if (pathes[j]->equals(car_path[i]))
            {
                sem_wait(&locks[j]);
                ofile << pathes[j]->get_start() << ", ";
                ofile << chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch()).count() << ", ";
                pollution = calculate_pollution(car->get_pollution(), pathes[j]->get_hardness());
                ofile << pathes[j]->get_finish() << ", ";
                ofile << chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch()).count() << ", ";
                ofile << pollution << ", ";
                sem_post(&locks[j]);
                sem_wait(&emission_sem);
                total_emission += pollution;
                sem_post(&emission_sem);
                ofile << total_emission;
            }
        }
        ofile << endl;
    }
    ofile.close();
}


int main()
{
    int path_number = 0;
    int max_car_number = 0;
    srand(time(NULL));
    int reading_state = 0;
    string line;
    string start = "";
    string finish = "";
    string hardness_string = "";
    int hardness = 0;
    vector<Edge*> car_path;
    vector<string> nodes;
    int cars_number = 0;

    
    while(cin >> line)
    {
        if (line == READING_STATE_DELIMITER)
        {
            reading_state = 1;
            continue;
        }
        stringstream ss(line);
        if (reading_state == 0)
        {
            getline(ss, start, DELIMITER);
            getline(ss, finish, DELIMITER);
            getline(ss, hardness_string, DELIMITER);
            hardness = stoi(hardness_string);   
            Edge* edge = new Edge(start, finish, hardness);
            pathes.push_back(edge);
        }
        else if (reading_state == 1)
        {
            reading_state = 2;
            string node_string;
            while (getline(ss, node_string, DELIMITER))
            {
                // cout << node_string;
                nodes.push_back(node_string);
            }
            // cout << endl;
            for (int i = 0;i < nodes.size() - 1; i++)
            {
                // cout << nodes[i] << nodes[i + 1] << endl;
                Edge* edge = new Edge(nodes[i], nodes[i + 1], -1);
                car_path.push_back(edge);
            }            
        }
        else if (reading_state == 2)
        {
            reading_state = 1;
            cars_number = stoi(line);
            // cout << "cars number " << cars_number << endl;
            for (int i = 0;i < cars_number; i++)
            {
                Car* car = new Car(car_path, max_car_number++, path_number);
                cars.push_back(car);
            }   
            path_number++;
            nodes.clear();
            car_path.clear();
        }
    }   
    cout << "pathes:" << endl;
    for (int i = 0;i < pathes.size(); i++)
    {
        pathes[i]->print();
    }
    for (int i = 0;i < cars.size(); i++)
    {
        cout << "car " << i << endl;
        cars[i]->print();
    }
    cout << "starting simulation" << endl;
    // cout << cars.size() << endl;
    // cout << threads.size() << endl;
    sem_init(&emission_sem, 0, 1);
    for (int i = 0 ;i < pathes.size(); i++)
    {
        sem_t sem;
        sem_init(&sem, 0, 1);
        locks.push_back(sem);
    }
    for (int i = 0 ; i < cars.size(); i++)
    {
        threads.push_back(thread(move_car, cars[i]));
    }

    for (int i = 0;i < threads.size(); i++)
    {
        if (threads[i].joinable())
            threads[i].join();
    }
    for (int i = 0;i < locks.size(); i++)
    {
        sem_destroy(&locks[i]);
    }
    cout << "all cars reach their destination" << endl;
    return 0;
}