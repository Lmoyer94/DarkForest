#include <vector>

struct Position
{
    long long x;
    long long y;

    Position()
    {
        x = 0;
        y = 0;
    };

    Position(int x, int y)
    {
        this->x = x;
        this->y = y;
    };

    bool operator==(Position p)
    {
        if (this->x == p.x && this->y == p.y)
            return true;
        else
            return false;
    }
};

class Signal;

struct Civ_Params
{
    float aggression;   //0.0 = pacifistic                              1.0 = xenocidal
    float optimism;     //0.0 = fearful / pessimistic                   1.0 = fully optimistic
};

class Civilization
{
    std::vector<Civilization*> known_civs;       //civs they personally know about
    std::vector<Civilization*> destroyed_civs;   //which civs they have personally destroyed
    std::vector<Civilization*> lost_civs;        //which civs have been noticed to have been destroyed

public:
    Position pos;
    Civ_Params params;
    bool alive;
    double points;
    int turns_alive;

    long long transmit_count;
    long long destroy_count;
    long long hide_count;

    Civilization()
    {
        pos = Position(0, 0);
        params.aggression = 0.0;
        params.optimism = 0.0;
        points = 0;
    };

    void set_params(Position p, float agg, float optim);

    void Turn(std::vector<Signal>& signals);

    void transmit(std::vector<Signal>& signals);
    void transmit_destruction(std::vector<Signal>& signals);
    void hide();
    void search(std::vector<Signal>& signals);
};

struct Signal
{
    Civilization* civ;
    Position origin;
    int years_ago;
    bool destroy_signal;

    Signal()
    {
        origin = Position(0, 0);
        years_ago = 0;
    };
};