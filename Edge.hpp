#ifndef EDGE_HPP
#define EDGE_HPP

#include <string>
#include <iostream>

using namespace std;

class Edge
{
private:
    std::string start;
    std::string finish;
    int hardness;
public:
    Edge(std::string _start, std::string _finish, int _hardness);
    void print();
    bool equals(Edge*);
    string get_start() { return start; }
    string get_finish() { return finish; }
    double get_hardness() { return hardness; }
};

#endif /* EDGE_HPP */
