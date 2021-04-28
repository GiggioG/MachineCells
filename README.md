# MachineCells
### A ripoff version of [Sam Hogan's Cell Machine](https://samhogan.itch.io/cell-machine).
This is a console game that these people made in programming class:
* [Redy](https://github.com/Redy-1)
* [Sofi](https://github.com/SofiaStanimirova)
* [Sten1ee](https://github.com/sten1ee)
* [Me](https://github.com/GiggioG)
It's a game about arranging blocks in the right way, and then releasing them to destroy the red blocks.
## Building and Running the game
When running the game for the first time, you need to compile. This is done with the `gcc` compiler. By using the `build.bat` script, the game should compile and run automatically. Then, every time you want to play the game, you can run `run.bat`, and it will start the game. Any time you mmodify the source code, you need to `build` again. When you add or modify levels, you don't need to `build` again.
## Controls
When you start a level, you will be in `Edit Mode`, where your controls are like this:

| Key | What it does                                  |
|-----|-----------------------------------------------|
| E   | Pick up or put down a block.                  |
| W   | Move cursor up.                               |
| A   | Move cursor left.                             |
| S   | Move cursor down.                             |
| D   | Move cursor right.                            |
| Q   | Exit `Edit mode` and enter `Simulation Mode`. |

When you press `Q` and enter `Simulation Mode`, your controls will be like this:

| Key   | What it does                              |
|-------|-------------------------------------------|
| Space | Pause the simulation while holding space. |
| R     | Go back to `Edit Mode`.                   |

If your arrangement of blocks doesn't destroy all the red ones, you can press `R` to reset the level.
When you successfuly complete a level, you will go into `Menu Mode`. In this mode, you can use these keys:

| Key                  | What it does            |
|----------------------|-------------------------|
| Q                    | Exit game.              |
| Any key that isn't Q | Continue to next level. |

<!-- 
# GenadiSnowstorm
### A game about how [Genadi](#genadi) dodges dangerous snowballs.
This is a console game that these people made in programming class:
* [JustDip](https://github.com/JustDip)
* [Redy](https://github.com/Redy-1)
* [Sofi](https://github.com/SofiaStanimirova)
* [ValentinaIvanova](https://github.com/Valentina-Ivanova)
* [Me](https://github.com/GiggioG)
<br><sub>I don't know their githubs yet, so no link.</sub>

### TO-DO
- [ ] remove dumb joke comments
- [ ] order the config constants
- [X] add jumping
- [ ] display how many [Kashturas](#kashtura) [Genadi](#genadi) has next to his health and score.
- [ ] remove initial 10 [Kashturas](#kashtura)
- [ ] add condition for giving [Kashturas](#kashtura)
- [ ] comment code better

### Dictionary
###### Kashtura
Bulgarian "къщура" - an old way of saying tiny house.
###### Genadi
A common Bulgarian name - Генади. 
-->