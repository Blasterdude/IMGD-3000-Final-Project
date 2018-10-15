SQUILL SQUISHER - final build 

Team 203


Thomas Meehan - tmeehan@wpi.edu
Paul Orvis - pworvis@wpi.edu

--------------------------------------------------------------------



To build from source:

NOTE: Sound files sent as separate folder due to size constraints.
Place the folder containing the sounds (“sounds”) in the debug directory



0) Setup development environment for Windows

See http://dragonfly.wpi.edu/ for details.



1) Build, as appropriate for the platform, in this case Visual Studio).



2) Run game by pressing F5 in Visual Studio.




--------------------------------------------------------------------

To run .exe

NOTE: Sound files sent as separate folder due to size constraints.
Place the folder containing the sounds (“sounds”) in the same level directory as the .exe file.

click and run the .exe



--------------------------------------------------------------------




Directories and files:



game.exe
A final build of the game. 

game/ - contains Visual Studio solution files if developing on Microsoft
 Windows

debug/sounds/ - contains the sound files used in the game

debug/sprites/ - contains the sprite files used in the game



--------------------------------------------------------------------



INSTRUCTIONS / DESCRIPTION:

SQUILL SQUISHER is a game similar in style to breakout, where players have to use a projectile to destroy all the enemies on screen, directly 
controlling a paddle to keep the projectile in play. Certain enemies take more hits than others, and some enemies grant powerups. When a powerup enemy is killed, a random powerup is granted to the user. Normally, 10 lives are granted for the whole game, meaning that the ball can go out of bounds 10 times before 
game over. Clearing the screen of enemies beats the level. There are currently 3 levels, each featuring increasing difficulty.



Power-ups:


Mega Paddle: Paddle is doubled in size for the rest of the match

Double Power: Power of projectile is doubled for the rest of the level

Bomb: A bomb goes off, decimating enemies in the center of the screen



CONTROLS: 

Players can control the paddle with either the A and D keys or left and right arrow keys. The projectile is released by pressing the spacebar.


Asset sources:

All music is courtesy of Square Enix’s “Final Fantasy CHIPS” album

All effects are pulled directly from Final Fantasy VII, VIII and I