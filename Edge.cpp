#include "Edge.hpp"

Edge::Edge(std::string _start, std::string _finish, int _hardness)
{
    start = _start;
    finish = _finish;
    hardness = _hardness;
}

void Edge::print()
{
    cout << start << " " << finish << " " << hardness << endl;
}

bool Edge::equals(Edge* edge)
{
    return edge->start == start && edge->finish == finish;
}