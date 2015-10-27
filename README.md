# GummyShips

Gummy Ships is a space Infinite-runner like game where you control 1 cube trying to avoid all the other blocks while trying to go super fast

- Current Build:

  - Game Obstacles
    - The Walls
      : The main crazy thingie in the game that are pretty big in size. They can move in 4 different patterns.
        - Straight on
        - in a Diagonal Box
        - Up and Down
        - Left to Right
        *For now, the Walls only decrease the game's speed to nothing when collided*
    - The Opposing Ships
      : These only move in a straight line and come at the player slower than the walls do
        - These shoot a bullet forward at regular intervals
        *For now, the Walls only decrease the game's speed to nothing when collided*
    - The Opposing Ship bullet thingies
      : These are only small bullets that litely damage the player's speed in the game
  - Working Powerup System
    - Minifier
      : Minifies all wall-like and ship-like entities
    - Bomb
      : Destroys all bullet, ship, and wall-like entities on the screen
    - Inverter
      : Reverses the player's controls
  - Player Action Commands
    - z (Invincibility)
      : For a set amount of time (not shown graphically), so long as key is held, player can't be damaged. Letting go of the key resets the ability to use it until the skill is loaded back up.
    - c (Speed Up)
      : For a set amount of time (not shown graphically), so long as key is held, player moves a lot faster. Letting go of the key resets the ability's timer to zero and reverts the speed back to nothing.


Controls:

  WASD -> Movement
  Z(hold) -> Invincibility
  C(hold) -> Speed Up
  
Compiled using Visual Studio C++ 2010
