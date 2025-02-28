# KingdomRushLite

## License
This repo adopts [GPL-3.0](https://www.gnu.org/licenses/gpl-3.0.html) License

## About
In this tower defence game prototype inspired by KingdomRush, you can not only build different towers using coins earned by defeating waves of enemies, but control a character with skills to battle, [click to play online](https://whythz-debug.github.io/KingdomRushLite/)

This prototype is designed data-driven, you can create your personal experience by editing the corresponding files below in `root\KingdomRushLite\Data\` after you clone this repo locally (Run `Main.cpp` under required IDE mode)
- `Map.csv`: Defines tilemap by denoting each tile in `a/b/c/d` form, see `Tile.h`
- `Configs.json`: Defines window resolution and stats of player, towers and enemies
- `Waves.json`: Defines the details of enemy waves spawned in game
```javascript
// List of spawn waves for series of enemies
[
    // Wave 0
    {
        // Wave properties
        "coin_rewards":     300,
        "wave_interval":    10,
        // List of spawn events for one enemy of particular type
        "spawn_list":
        [
            // Event 0 
            {
                // Event properties
                "event_interval":   1,
                "spawn_point":      2,
                "enemy_type":       "slime"
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

 Short descriptions about the core functional classes which forms multiple extensible gameplay systems are listed below, notice that all managers are derived from singleton `Manager` base class
- General Game Management
    - `GameManager`: Maintains game main loop that handles all updating, rendering and inputs
    - `ProcessManager`: Maintains the in-game runtime data and offers related interfaces
- Infrastructure Classes
    - `StateMachine`: Acts as component of entity to control its changing of states derived from `State`
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
    - `Enemy`: Organized by `EnemyManager`, aims at attacking home
        - `Slime`: Derives from `Enemy`, weak vitality and slow
        - `SlimeKing`: Derives from `Enemy`, medium vitality and slow, able to heal itself
        - `Skeleton`: Derives from `Enemy`, medium vitality and fast
        - `Goblin`: Derives from `Enemy`, medium vitality and fast
        - `GoblinPriest`: Derives from `Enemy`, strong vitality and slow, able to heal in range
    - `Drop`: Organized by `DropManager`, collides with Player with particular effects
        - `Coin`: Derives from `Drop`, picked up by player to earn coins
    - `Wave`: Struct organized by `WaveManager` including multiple enemy spawn events
- Towers and Their Bullets
    - `Tower`: Organized by `TowerManager`, can be built for attacking and upgraded by coins
        - `Archer`: Derives from `Tower`, cheap price and shoots with arrows 
        - `Axeman`: Derives from `Tower`, medium price and shoots with axes
        - `Gunner`: Derives from `Tower`, expensive price and shoots with shells
    - `Bullet`: Organized by `BulletManager` and collides with `Enemy` with damage and effects
        - `Arrow`: Derives from `Bullet`, attacks single enemy with high frequency
        - `Axe`: Derives from `Bullet`, attacks single enemy with slowdown effect
        - `Shell`: Derives from `Bullet`, attacks multiple enemies in range with high damage
- User Interface
    - `UIManager`: Offers general rendering methods and organizes all the UI components
        - `StatusUI`: Renders the real-time status of health, coin number and cooldown of player skills
        - `TowerPanel`: Derives `TowerBuildPanel` and `TowerUpgradePanel` for building or upgrading towers
- Controlable Character
    - `Player`: Organized by `PlayerManager`, controled by state machine
        - `PlayerDragon`: Derives from `Player`, can attack enemy by skills including flash and impact

## Dependency
|Lib|Version|
|---|---|
|[cJSON](https://github.com/DaveGamble/cJSON)|cJSON-1.7.18|
|[SDL](https://github.com/libsdl-org/SDL)|SDL2-devel-2.30.2-VC|
|[SDL_image](https://github.com/libsdl-org/SDL_image)|SDL2_image-devel-2.8.2-VC|
|[SDL_gfx](https://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/)|[SDL2_gfx-1.0.4-VC](https://github.com/giroletm/SDL2_gfx/releases/tag/release-1.0.4)|
|[SDL_ttf](https://github.com/libsdl-org/SDL_ttf)|SDL2_ttf-devel-2.22.0-VC|
|[SDL_mixer](https://github.com/libsdl-org/SDL_mixer)|SDL2_mixer-devel-2.8.0-VC|
