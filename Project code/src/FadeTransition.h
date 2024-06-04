#pragma once
#include "Globals.h"
#include <raylib.h>

//fade out: gradual decrease in visibility
//fade in: gradual increase in visibility
enum class FadeState { OUT, IN };

class FadeTransition
{
public:
    FadeTransition();
    ~FadeTransition();

    //Set a fade-in effect to a target state in a number of frames
    void Set(GameState to, int frames_to, const Rectangle& rect);

    //Set a fade-out and fade-in effect from a source state to a target state over in a numbers of frames
    void Set(GameState from, int frames_from, GameState to, int frames_to, const Rectangle& rect);

    //Check if the fade effect is currently active
    bool IsActive();

    //Update the fade effect over time and return the current state after the update
    GameState Update();

    //Render the fade effect
    void Render();

private:
    //Flag indicating whether the fade effect is active
    bool is_active;

    //Source and target states of the fade effect
    GameState from, to;

    //Number of frames for fading from and to the target state
    int frames_from, frames_to;

    //Counter to track the current frame during the fade effect
    int frames_counter;

    //Rectangle defining the area where the fade effect will be applied
    Rectangle rect;

    //Current state of the fade effect (fade in or fade out)
    FadeState state;
};