Dark Forest is a simulation "game" that takes a long time to complete. It is a single threaded application that
created various alien civilizations that all have different aggression and optimism traits. The more aggressive,
the more likely they are to destroy another civilization. The more optimistic they are, the more likely they are 
to transmit a signal. Each turn, they have have one option: Destroy, Hide, or Transmit. They also almost always
search for signals.

The destruction is instantaneous. Sending but 1 signal is potentially disasterous, yet in the game it is common 
to send hundreds, if not thousands for the most long-lived. This is because it takes a long time for these signals
to spread across the galaxy. Once contact is made, signal production typically reduces as optimism declines with 
each observed destruction. This is all well and good until you realize that the radio bubble is still traveling 
outward and easily detected by highly advanced civilizations.

The reason this application takes forever to process is all the logic, and the time scale being used. The
distance unit is light years, and Earth is -26,500 light years from the center. The game will be slow while lots 
of signals are being sent out, and lots of civilizations are taking their turns. It will speed up once more
when civilizations are being wiped out.

The most dangerous place to be is in the center. Yes, you can see every civilization from there faster, but every
civilization can also see you faster (if you sent a signal). The best strategy is to never send a signal! If you 
don't send a signal, then you cannot be found. This is of course highly unlikely however, as any sufficiently 
advanced civilization will have sent a signal at some point, incidentally or otherwise.

A civilization starting with 0.0 optimism will never be found, by virtue of never sending a signal.



VERSION 1:      (~2:15 hour long runtime)
    CHANGE:     Proof of concept (initial version)
    BUG:        Increases destroy_count by 10 for every single kill


VERSION 2:      (13:58 hour long runtime)
    CHANGE:     Milky Way update (larger map area)
    
    BUG:        Increases destroy_count by 10 for every single kill


VERSION 3:      (36:08 hours long runtime)
    CHANGE:     Add Paranoia mechanic
    CHANGE:     Reward revision
    CHANGE:     Results overhaul
    CHANGE:     Tracking remaining Civs by turn
    CHANGE:     Elapsed time per turn.
    
    BUG:        Remaining civs information will always display starting amount. (not tracking dead civs)
    BUG FIX:    Removes the destroy_count bug from previous versions.


VERSION 4:      (13:30 hours long runtime)
    CHANGE:         Remove paranoia mechanic. It was slowing things down massively.
    CHANGE:         Changed optimism penalty for noticing destruction of Civ
    CHANGE:         Added the optimism penalty for noticing destroyed civ during search.
    CHANGE:         Clamp parameters
    CHANGE:         Further enhanced results
    CHANGE:         Write to input.txt for initial conditions
    CHANGE:         Requires an input of # of Civs and # of rounds to play
    CHANGE:         results.txt will contain longest turn information

    BUG FIX:        Fixed in-game information relating to remaining Civs.
    BUG FIX:        My constant need to change things. Final Version... probably.
