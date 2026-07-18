#include "../include/civ.hpp"
#include <cstdlib>
#include <cmath>

void Civilization::set_params(Position p, float a, float o)
{
    pos = p;
    params.aggression = a;
    params.optimism = o;

    alive = true;
    turns_alive = 0;
    points = 0.0;

    destroy_count = 0;
    hide_count = 0;
    transmit_count = 0;
};

void Civilization::Turn(std::vector<Signal>& signals)
{
    if (!alive)
        return;

    turns_alive++;

    float search_roll = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    if (search_roll <= 0.9)
    {
        search(signals);
    }

    if (known_civs.size() >= 1)
    {
        //roll for aggressive action
        float die_roll = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        if (die_roll <= params.aggression)
        {
            //well, time to destroy someone
            int num_civs = known_civs.size();
            int target = rand() % num_civs;

            //is this known civ already dead?
            if (!known_civs[target]->alive)
            {
                lost_civs.push_back(known_civs[target]);                   //we now know they are lost
                known_civs.erase(known_civs.begin() + target);      //we remove from known

                //already dead
                this->params.aggression += 0.025;      //increase aggression due to fear
                this->params.optimism   -= 0.05;       //decrease optimism due to fear

                if (params.aggression > 1.0)
                    params.aggression = 1.0;
                if (params.optimism < 0.0)
                    params.optimism = 0.0;

                return;
            }

            known_civs[target]->alive = false;
            known_civs[target]->transmit_destruction(signals);
            destroyed_civs.push_back(known_civs[target]);
            known_civs.erase(known_civs.begin() + target);

            params.optimism -= 0.05;
            if (params.optimism < 0.0)
                params.optimism = 0.0;

            points += 1000;
            destroy_count++;
            return;
        }
    }

    // optimism increases likelihood of transmitting
    float optimism_roll = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    if (optimism_roll <= params.optimism)
    {
        transmit(signals);
    }
    else
    {
        hide();
    }
};

void Civilization::transmit(std::vector<Signal>& signals)
{
    Signal signal;
    signal.civ = this;
    signal.origin = this->pos;
    signal.years_ago = 0;
    signal.destroy_signal = false;

    signals.push_back(signal);

    //increase points value by static amount
    points += 0.50;

    //increase transmission count
    transmit_count++;
};

void Civilization::transmit_destruction(std::vector<Signal>& signals)
{
    Signal signal;
    signal.civ = this;
    signal.origin = this->pos;
    signal.years_ago = 0;
    signal.destroy_signal = true;

    signals.push_back(signal);

    //transmit count doesn't matter for this one
}

void Civilization::hide()
{
    hide_count++;
};

void Civilization::search(std::vector<Signal>& signals)
{
    for (long long i = 0; i < signals.size(); i++)
    {
        Position origin = signals[i].origin;
        bool matched = false;

        float hd = origin.x - pos.x;
        float vd = origin.y - pos.y;

        double distance = sqrt((hd * hd) + (vd * vd));

        if ((static_cast<int>(distance) >= signals[i].years_ago - 1) && (static_cast<int>(distance) <= signals[i].years_ago + 1))
        {
            //is it our own signal, ignore it
            if (signals[i].civ->pos == pos)
                continue;

            //if the origin is already known, check for destruction signal
            for (int j = 0; j < known_civs.size(); j++)
            {
                if (known_civs[j]->pos == origin)
                {
                    // IT IS KNOWN
                    if (signals[i].destroy_signal)
                    {
                        // IT'S DESTROYED... make it lost and remove from known
                        lost_civs.push_back(signals[i].civ);
                        known_civs.erase(known_civs.begin() + j);

                        this->params.aggression += 0.025;        //increase aggression due to fear
                        this->params.optimism -= 0.05;           //decrease optimism due to fear

                        if (params.aggression > 1.0)
                            params.aggression = 1.0;
                        if (params.optimism < 0.0)
                            params.optimism = 0.0;

                        this->points += 50;
                        matched = true;
                        break;
                    }
                }
            }

            if (matched)
                continue;

            //origin not know, check for destroyed_civs
            for (int j = 0; j < destroyed_civs.size(); j++)
            {
                if (destroyed_civs[j]->pos == origin)
                {
                    matched = true;
                    break;
                }
            }

            if (matched)
                continue;

            for (int j = 0; j < lost_civs.size(); j++)
            {
                if (lost_civs[j]->pos == origin)
                {
                    matched = true;
                    break;
                }
            }

            if (matched)
                continue;

            //not known already, now it is; destruction status unknown
            known_civs.push_back(signals[i].civ);

            params.optimism -= 0.005;
            if (params.optimism < 0.0)
                params.optimism = 0.0;

            this->points += 25;
        }
    }
};