#ifndef CAR_HPP
#define CAR_HPP

#include <vector>
#include "Edge.hpp"
#include <random>
#include <iostream>

using namespace std;

class Car
{
private:
    int path_number;
    int car_number;
    int pollution;
    vector<Edge*> path;
public:
    Car(vector<Edge*> _path, int _car_number, int _path_number)
    {
        path_number = _path_number;
        car_number = _car_number;
        pollution = random() % 10 + 1;
        path = _path;
    }
    void print()
    {
        cout << "car number: " << car_number << endl;
        cout << "pollution: " << pollution << endl;
        cout << "edges: " << endl;
        for(int i = 0;i < path.size();i++)
        {
            path[i]->print();
        }
    }
    vector<Edge*> get_path()
    {
        return path;
    }
    string generate_file_name()
    {
        return to_string(path_number) + "-" +  to_string(car_number);
    }
    double get_pollution() { return pollution; }
};


#endif /* CAR_HPP */
