The program is a Space Invaders game with moving and shooting enemies, destructible obstacles with different levels of destruction, a player, and UFOs which occur during the game.

A great part of the program is the game engine, which provides a set of systems supporting development, such as AssetManager, EntityManager, SpriteRenderer, InputHandler, and others.
The game engine attempts to implement a simple version of ECS (Entity Component System), where an entity is a container of components, and each component holds transformation data, sprite data, gameplay data (score, health), and so on.
Each entity in the game is derived from the Entity class, where component functionality is implemented.
The lifespan of all entities is processed by the EntityManager class, which enables creating, managing, and destroying entities. Systems such as SpriteRenderer allow drawing each entity’s sprite.

The engine has a main GameApplication class which handles game input, setup, and starting the game.
The Game class gets references to game services structure which contains SpriteRenderer, InputHandler, EntityManager, AssetManager, and so on.
The Game class performs all game logic, specifically scene management and transitions between scenes.

There are three different types of scenes, and each scene is derived from the Scene base class, which contains pure virtual methods and a reference to GameServices that it receives from the main game class.

Each scene contains its own logic. For example, StartScene and EndScene contain text showing the welcome screen in StartScene and the player's score in EndScene. PlayScene contains all gameplay logic, specifically EnemySystem, GameHUD, ObstaclesSpawner, Player and other gameplay aspects.


Note: 
All entity positions are set as a "percent" of the screen. For example, an entity with position (50, 50) will be placed exactly at the center of the screen. This "percent" position allows the entity's position and size to be independent of the window size. Translation from "percent" position to real pixel position is provided by methods implemented in utility.h.

Possible improvements:
Currently, all game assets are preloaded and held by the AssetManager class and are stored in memory during the entire game run. AssetManager also holds all information about asset paths and tags, which logically could be placed in the class of specific scene, where each scene would preload its required assets during scene transition.

GameText should be implemented as a class derived from Entity, so it would be processed by EntityManager like other entities. However, it was not implemented this way because it would not match the CRenderer component logic, which currently expects an entity to have only one sprite.

All information about the time spent on this assignment can be found in the devlog file.