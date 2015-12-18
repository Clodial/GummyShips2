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

Settings and HUD items
- Sequence Editor
      : Allows you to edit the different patterns that randomly come at the player
- HUD
  - Top Bar: The current speed of the game
  - Orange Boxes at the bottom: Player lives

things to note:
  There is a scoring system:
    It's based on what entities you add to each file in the sequence editor
      black-> 0pts
      red -> 200pts
      yellow -> 50pts
      green -> -20pts
    Scoring works that, once a sequence fully passes you, you gain points.
    
    Currently, the game does hold on to your high scores, which are displayed once the game starts
  



Controls:

  Game Mode:
  WASD -> Movement
  E -> go to Sequence Editor Mode 
  P -> Pause game
  
  Sequence Editor Mode (*default):
  WASD -> Movement around editor
  Z -> changes the current node's type
    red     -> random moving blocks that can end your game
    yellow  -> stationary ships that can shoot at the player
    green   -> powerups
    black   -> nothing
  Q -> Switches the game to Game Mode
  1, 2, 3, 4, 5 -> Cycles to that sequence for editing
  
Compiled using Visual Studio C++ 2010
