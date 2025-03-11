# KingdomRushLite

## License
This repo adopts [GPL-3.0](https://www.gnu.org/licenses/gpl-3.0.html) License

## About
This branch is for cmake build by Emscripten, but in current commit history, you can use following instructions in cmd to build in MSVC

```
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## Dependency
|Lib|Version|
|---|---|
|[cJSON](https://github.com/DaveGamble/cJSON)|cJSON-1.7.18|
|[SDL](https://github.com/libsdl-org/SDL)|SDL2-devel-2.30.2-VC|
|[SDL_image](https://github.com/libsdl-org/SDL_image)|SDL2_image-devel-2.8.2-VC|
|[SDL_gfx](https://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/)|[SDL2_gfx-1.0.4-VC](https://github.com/giroletm/SDL2_gfx/releases/tag/release-1.0.4)|
|[SDL_ttf](https://github.com/libsdl-org/SDL_ttf)|SDL2_ttf-devel-2.22.0-VC|
|[SDL_mixer](https://github.com/libsdl-org/SDL_mixer)|SDL2_mixer-devel-2.8.0-VC|