# KingdomRushLite

## License
This repo adopts [GPL-3.0](https://www.gnu.org/licenses/gpl-3.0.html) License

## About
In this tower defence game inspired by KingdomRush series, you can not only build different towers using coins earned by defeating waves of enemies, but also control a dragon with skills to battle, [click to play online](https://whythz-debug.github.io/KingdomRushLite/)

You can edit the files in `root\KingdomRushLite\Data\` to modify your own game experience after you clone this repo locally and run in Release x64 mode
- `map.csv`: Defines tilemap, each tile is denotd in `a/b/c/d` form, see `Tile.h`
- `config.json`: Defines window resolution and stats of player, towers and enemies
- `waves.json`: Defines the amount and details of multiple enemy waves
```javascript
// List of spawn waves for series of enemies
[
    // Wave 0
    {
        // Wave properties
        "rewards":	300,
        "interval":	10,
        // List of spawn events for one enemy of particular type
        "spawn_list":
        [
            // Event 0 
            {
                // Event properties
                "interval":	1,
                "spawn_point":	2,
                "enemy_type": "Slime"
            },
            // Event n
            {
            }
        ]
    },
    // Wave n
    {
    }
]
```

This game is data-driven and implemented by multiple extensible systems, short descriptions about the core functional classes are listed below, notice that all managers are derived from singleton `Manager` base class
- General Game Management
    - `GameManager`: Maintains game loop that handles all updating, rendering and inputs
    - `ProcessManager`: Maintains the in-game runtime data and offers related interfaces
- Infrastructure Classes
    - `Vector2`: Basic 2D vector with mathematical operators overloaded
    - `Timer`: Will trigger self-defined callback function when time out
    - `Animation`: Can be rendered with rotation, also offers spritesheet cutting
- Resource Assets Loading
    - `ResourceManager`: Loads assets by paths into particular resource pools including textures, sounds, musics and fonts
- Configuration Files Loading
    - `ConfigManager`: Loads json and csv files including configs for basic game properties, enemy waves info and tilemap info
- Tile-based Map
    - `Tile`: Struct of single tile that forms tilemap
    - `Map`: Stores basic tilemap and other info including home and enemy spawn points
    - `Route`: Set of indices of consecutive acyclic tiles on which is for enemies to move
- Enemies and Their Drops
    - `Enemy`: Organized by `EnemyManager` and aims at attacking home
        - `Slime`: Weak vitality and slow
        - `SlimeKing`: Medium vitality and slow
        - `Skeleton`: Medium vitality and fast
        - `Goblin`: Medium vitality and fast
        - `GoblinPriest`: Strong vitality and able to heal
    - `Drop`: Organized by `DropManager` and collides with Player with particular effects
        - `Coin`: Picked up by player to earn coins
    - `Wave`: Struct organized by `WaveManager` including multiple enemy spawn events
- Towers and Their Bullets
    - `Tower`: Organized by `TowerManager` and can be built for attacking and upgraded by coins
        - `Archer`: Cheap price and shoots with arrows 
        - `Axeman`: Medium price and shoots with axes
        - `Gunner`: Expensive price and shoots with shells
    - `Bullet`: Organized by `BulletManager` and collides with `Enemy` with damage and effects
        - `Arrow`: Attacks single enemy with high frequency
        - `Axe`: Attacks single enemy with slowdown effect
        - `Shell`: Attacks range of enemies with high damage
- User Interface
    - `StatusUI`: 
    - `TowerPanel`: Derives `TowerBuildPanel` and `TowerUpgradePanel`
- Controlable Character
    - WIP
    - WIP

## Dependency
|Lib|Version|
|---|---|
|[cJSON](https://github.com/DaveGamble/cJSON)|cJSON-1.7.18|
|[SDL](https://github.com/libsdl-org/SDL)|SDL2-devel-2.30.2-VC|
|[SDL_image](https://github.com/libsdl-org/SDL_image)|SDL2_image-devel-2.8.2-VC|
|[SDL_gfx](https://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/)|[SDL2_gfx-1.0.4-VC](https://github.com/giroletm/SDL2_gfx/releases/tag/release-1.0.4)|
|[SDL_ttf](https://github.com/libsdl-org/SDL_ttf)|SDL2_ttf-devel-2.22.0-VC|
|[SDL_mixer](https://github.com/libsdl-org/SDL_mixer)|SDL2_mixer-devel-2.8.0-VC|
