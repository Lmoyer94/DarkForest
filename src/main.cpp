#include "../include/civ.hpp"
#include <random>
#include <ctime>

#include <chrono>
#include <fstream>
#include <iostream>

#define MAJOR 1
#define MINOR 0
#define REVISION 0

int main(int argc, char** argv)
{
    int civ_size = 0;
    int rounds = 0;

    if (argc == 2)
    {
        std::string input = argv[1];
        if (input == "--version")
        {
            std::cout << "Dark Forest v" << MAJOR << "." << MINOR << "." << REVISION << "\n";
            std::cout << "Copyright (C) 2026 Cyan Thunder Software Foundation" << "\n";
            std::cout << "This is free software; see the source for copying conditions." << "\n";
            std::cout << "There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
            return EXIT_SUCCESS;
        }
        else 
        {
            std::cerr << "Incorrect usage of \"darkforest\"\n";
            std::cerr << "Please input a number of civilizations first, followed by a number of rounds (years).\n";
            std::cerr << "Format:    \'./darkforest <# civs> <# of rounds>\'\n";
            std::cerr << "Example Usage:    \'./darkforest 1000 100000\'" << std::endl;
            return EXIT_FAILURE;
        }
    }
    else if (argc < 3 || argc > 3)
    {
        std::cerr << "Incorrect usage of \"darkforest\"\n";
        std::cerr << "Please input a number of civilizations first, followed by a number of rounds (years).\n";
        std::cerr << "Format:    \'./darkforest <# civs> <# of rounds>\'\n";
        std::cerr << "Example Usage:    \'./darkforest 1000 100000\'" << std::endl;
        return EXIT_FAILURE;
    }
    
    try 
    {
        civ_size = atoi(argv[1]);
        if (civ_size < 500)
        {
            std::cerr << "Please use 500+ civilizations." << std::endl;
            return EXIT_FAILURE;
        }

        rounds = atoi(argv[2]);
        if (rounds > 120000)
        {
            std::cerr << "The Milky Way is approximately 120000 light years across." << "\n";
            std::cerr << "Any value above 120000 light years is pointless, please try again." << std::endl;
            return EXIT_FAILURE;
        }
        else if (rounds < 1000)
        {
            std::cerr << "Please enter a number of rounds equal to 1000+." << std::endl;
            return EXIT_FAILURE;
        }
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<Civilization> civs;
    civs.reserve(civ_size * 1.5);

    std::vector<Signal> signals;
    signals.reserve(5000000);       //let's save 5,000,000 spaces for signals upfront; at 3810 turns with 1000 Civs, we had 999,848 signals

    int turns_taken = 0;
    double longest_turn = 0.0;

    srand(time(0));

    Civilization humanity;
    humanity.set_params(Position(0, -26500), 0.25, 0.75);
    civs.push_back(humanity);

    for (int i = 1; i < civ_size; i++)
    {
        float a = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float o = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        o += 0.01;
        if (o > 1.0)
            o = 1.0;

        std::random_device rd;
        std::mt19937_64 gen(rd());

        int x = 0, y = 0;

        while (true)
        {
            int min     =      0;
            int max     =  60000; //max distance from center
            std::uniform_int_distribution<int> distr(min, max);
            int distance = distr(gen);

            std::uniform_real_distribution<float> angle(0.00, 360.00);
            float theta = angle(gen);

            x = distance * cos(theta);
            y = distance * sin(theta);

            bool same = false;
            for (int i = 0; i < civs.size(); i++)
            {
                if (civs[i].pos.x == x && civs[i].pos.y == y)
                {
                    same = true;
                    break;
                }
            }

            if (same)
                continue;
            else
                break;
        }

        Civilization c;
        c.set_params(Position(x, y), a, o);
        civs.push_back(c);
    }

    std::ofstream initial_state;
    initial_state.open("inputs.txt");

    initial_state << "DarkForest v" << MAJOR << "." << MINOR << "." << REVISION << "\n";
    initial_state << "Number Civs: " << civs.size() << "\n\n";

    for (int i = 0; i < civs.size(); i++)
    {
        if (i == 0)
            initial_state << "Humanity" << "\n";
        else
            initial_state << "Civilization #" << i << "\n";

        initial_state << "Location:     " << civs[i].pos.x << ", " << civs[i].pos.y << "\n";
        initial_state << "Aggression:   " << civs[i].params.aggression << "\n";
        initial_state << "Optimism:     " << civs[i].params.optimism << "\n\n";
    }

    initial_state.close();

    auto last_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> game_time = last_time - last_time;

    for (long long i = 0; i < rounds; i++)
    {
        for (int j = 0; j < civs.size(); j++)
        {
            civs[j].Turn(signals);
        }

        for (int j = 0; j < signals.size(); j++)
        {
            signals[j].years_ago++;
        }

        turns_taken++;

        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = current_time - last_time;

        if (elapsed_seconds.count() > longest_turn)
            longest_turn = elapsed_seconds.count();

        last_time = current_time;
        game_time += elapsed_seconds;
        std::cout << "Round " << i+1 << " is completed!";
        std::cout << "          Elapsed Time: " << elapsed_seconds.count();

        int dead = 0;
        for (int i = 0; i < civs.size(); i++)
        {
            if (!civs[i].alive)
                dead++;
        }

        std::cout << "          Remaining  Civs: "   << (civs.size() - dead);
        std::cout << "          Signal QTY: " << signals.size() << std::endl;

        if (dead >= civs.size() - 1)
            break;
    }

    std::ofstream results;
    results.open("results.txt");

    results << "DarkForest v" << MAJOR << "." << MINOR << "." << REVISION << "\n";
    results << "Elapsed Time For Game: " << game_time.count() << " seconds" << "\n";
    results << "Turns Taken: " << turns_taken << "\n";
    results << "Longest Turn: " << longest_turn << "\n";
    results << "Signals Sent: " << signals.size() << "\n" << "\n";

    if (civs[0].alive)
        results << "Humanity survived." << "\n";
    else
        results << "Humanity was eradicated." << "\n";

    results << "Final Score: " << civs[0].points << "\n";
    results << "Turns Alive: " << civs[0].turns_alive << "\n";
    results << "Destroyed:   " << civs[0].destroy_count << "\n";
    results << "Hide:        " << civs[0].hide_count << "\n";
    results << "Transmit:    " << civs[0].transmit_count << "\n";

    results << "Ending Aggression: " << civs[0].params.aggression << "\n";
    results << "Ending Optimism:   " << civs[0].params.optimism << "\n" << std::endl;

    for (int i = 1; i < civs.size(); i++)
    {
        if (civs[i].alive)
            results << "Civilization #" << i+1 << " survived." << " Position : " << civs[i].pos.x << ", " << civs[i].pos.y << "\n";
        else
            results << "Civilization #" << i+1 << " was eradicated." << " Position : " << civs[i].pos.x << ", " << civs[i].pos.y << "\n";

        results << "Final Score: " << civs[i].points << "\n";
        results << "Turns Alive: " << civs[i].turns_alive << "\n";
        results << "Destroyed:   " << civs[i].destroy_count << "\n";
        results << "Hide:        " << civs[i].hide_count << "\n";
        results << "Transmit:    " << civs[i].transmit_count << "\n";

        results << "Ending Aggression: " << civs[i].params.aggression << "\n";
        results << "Ending Optimism:   " << civs[i].params.optimism << "\n" << std::endl;
    }

    results.close();

    std::cout << "\n" << "The program results have been stored in \"results.txt\"\n" << std::endl;

    return EXIT_SUCCESS;
};