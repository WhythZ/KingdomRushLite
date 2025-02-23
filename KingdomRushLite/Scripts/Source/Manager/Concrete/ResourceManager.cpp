#include "../../../Header/Manager/Concrete/ResourceManager.h"

bool ResourceManager::LoadResource(SDL_Renderer* _renderer)
{
	if (!LoadTextureRes(_renderer)) return false;
	if (!LoadMusicRes()) return false;
	if (!LoadSoundRes()) return false;
	if (!LoadFontRes()) return false;

	return true;
}

const TexturePool& ResourceManager::GetTexturePool()
{
	return texturePool;
}

const MusicPool& ResourceManager::GetMusicPool()
{
	return musicPool;
}

const SoundPool& ResourceManager::GetSoundPool()
{
	return soundPool;
}

const FontPool& ResourceManager::GetFontPool()
{
	return fontPool;
}

bool ResourceManager::LoadTextureRes(SDL_Renderer* _renderer)
{
	//因为此处我们无需用到SDL_Surface*对象，所以可以直接使用IMG_LoadTexture()方法省略将两个步骤合一
	//SDL_Surface* _imgSurface = IMG_Load("Assets/xx.jpg");                             //从外存加载到内存（内存结构体）
	//SDL_Texture* _imgTexture = SDL_CreateTextureFromSurface(_renderer, _imgSurface);  //从内存加载到显存（GPU纹理数据）

	//将图片资源加载存储在图像池（unordered_map类型的容器）
	texturePool[TextureResID::PlayerDragon] = IMG_LoadTexture(_renderer, "Assets/Sprite/Player/player.png");
	texturePool[TextureResID::VFX_Player_Flash_Up] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/flash_up.png");
	texturePool[TextureResID::VFX_Player_Flash_Down] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/flash_down.png");
	texturePool[TextureResID::VFX_Player_Flash_Left] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/flash_left.png");
	texturePool[TextureResID::VFX_Player_Flash_Right] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/flash_right.png");
	texturePool[TextureResID::VFX_Player_Impact_Up] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/impact_up.png");
	texturePool[TextureResID::VFX_Player_Impact_Down] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/impact_down.png");
	texturePool[TextureResID::VFX_Player_Impact_Left] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/impact_left.png");
	texturePool[TextureResID::VFX_Player_Impact_Right] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/impact_right.png");
	texturePool[TextureResID::Bullet_Shell_Explode] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/explode.png");

	texturePool[TextureResID::Tile_TileSet] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tile/tileset.png");
	texturePool[TextureResID::Tile_Home] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tile/home.png");
	texturePool[TextureResID::Tile_Spawner] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tile/spawner.png");
	texturePool[TextureResID::Tile_DirectionArrow] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tile/direction_arrow.png");

	texturePool[TextureResID::Tower_Archer] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tower/tower_archer.png");
	texturePool[TextureResID::Tower_Axeman] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tower/tower_axeman.png");
	texturePool[TextureResID::Tower_Gunner] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tower/tower_gunner.png");
	texturePool[TextureResID::Bullet_Arrow] = IMG_LoadTexture(_renderer, "Assets/Sprite/Bullet/arrow.png");
	texturePool[TextureResID::Bullet_Axe] = IMG_LoadTexture(_renderer, "Assets/Sprite/Bullet/axe.png");
	texturePool[TextureResID::Bullet_Shell] = IMG_LoadTexture(_renderer, "Assets/Sprite/Bullet/shell.png");

	texturePool[TextureResID::Enemy_Slime] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/slime.png");
	texturePool[TextureResID::Enemy_Slime_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/slime_sketch.png");
	texturePool[TextureResID::Enemy_SlimeKing] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/king_slime.png");
	texturePool[TextureResID::Enemy_SlimeKing_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/king_slime_sketch.png");
	texturePool[TextureResID::Enemy_Skeleton] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/skeleton.png");
	texturePool[TextureResID::Enemy_Skeleton_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/skeleton_sketch.png");
	texturePool[TextureResID::Enemy_Goblin] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/goblin.png");
	texturePool[TextureResID::Enemy_Goblin_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/goblin_sketch.png");
	texturePool[TextureResID::Enemy_GoblinPriest] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/priest_goblin.png");
	texturePool[TextureResID::Enemy_GoblinPriest_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/priest_goblin_sketch.png");

	texturePool[TextureResID::Item_Coin] = IMG_LoadTexture(_renderer, "Assets/Sprite/Item/coin.png");

	texturePool[TextureResID::UI_Icon_Coin] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/coin.png");
	texturePool[TextureResID::UI_Icon_HealthHeart] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/heart.png");
	texturePool[TextureResID::UI_Icon_PlayerAvatar] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/player_avatar.png");
	texturePool[TextureResID::UI_Icon_HomeAvatar] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/home_avatar.png");

	texturePool[TextureResID::UI_SelectCursor] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/select_cursor.png");
	texturePool[TextureResID::UI_Build_Idle] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/place_idle.png");
	texturePool[TextureResID::UI_Build_HoveredTop] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/place_hovered_top.png");
	texturePool[TextureResID::UI_Build_HoveredLeft] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/place_hovered_left.png");
	texturePool[TextureResID::UI_Build_HoveredRight] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/place_hovered_right.png");
	texturePool[TextureResID::UI_Upgrade_Idle] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/upgrade_idle.png");
	texturePool[TextureResID::UI_Upgrade_HoveredTop] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/upgrade_hovered_top.png");
	texturePool[TextureResID::UI_Upgrade_HoveredLeft] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/upgrade_hovered_left.png");
	texturePool[TextureResID::UI_Upgrade_HoveredRight] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/upgrade_hovered_right.png");

	texturePool[TextureResID::UI_Text_Win] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/win_text.png");
	texturePool[TextureResID::UI_Text_Loss] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/loss_text.png");
	texturePool[TextureResID::UI_GameOver] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/game_over_bar.png");

	//检查unordered_map容器中所有键值对的值是否有效；其中auto是自动类型推导，成员second访问的是键值对的值
	for (const auto& _pair : texturePool)
		if (!_pair.second) return false;
	//检查无误就返回加载成功
	return true;
}

bool ResourceManager::LoadMusicRes()
{
	//从文件中加载音乐
	musicPool[MusicResID::Main] = Mix_LoadMUS("Assets/Audio/BGM/main_bgm.mp3");

	//检查unordered_map容器中所有键值对的值是否有效；其中auto是自动类型推导，成员second访问的是键值对的值
	for (const auto& _pair : musicPool)
		if (!_pair.second) return false;
	//检查无误就返回加载成功
	return true;
}

bool ResourceManager::LoadSoundRes()
{
	//从文件中加载音效
	soundPool[SoundResID::Player_Skill_Flash] = Mix_LoadWAV("Assets/Audio/SFX/flash.wav");
	soundPool[SoundResID::Player_Skill_Impact] = Mix_LoadWAV("Assets/Audio/SFX/impact.wav");

	soundPool[SoundResID::Pick_Coin] = Mix_LoadWAV("Assets/Audio/SFX/coin.mp3");
	soundPool[SoundResID::Home_Hurt] = Mix_LoadWAV("Assets/Audio/SFX/home_hurt.wav");

	soundPool[SoundResID::Tower_Build] = Mix_LoadWAV("Assets/Audio/SFX/place_tower.mp3");
	soundPool[SoundResID::Tower_Upgrade] = Mix_LoadWAV("Assets/Audio/SFX/tower_level_up.mp3");
	soundPool[SoundResID::Tower_Error] = Mix_LoadWAV("Assets/Audio/SFX/tower_error.mp3");

	soundPool[SoundResID::Arrow_Fire_1] = Mix_LoadWAV("Assets/Audio/SFX/arrow_fire_1.mp3");
	soundPool[SoundResID::Arrow_Fire_2] = Mix_LoadWAV("Assets/Audio/SFX/arrow_fire_2.mp3");
	soundPool[SoundResID::Arrow_Hit_1] = Mix_LoadWAV("Assets/Audio/SFX/arrow_hit_1.mp3");
	soundPool[SoundResID::Arrow_Hit_2] = Mix_LoadWAV("Assets/Audio/SFX/arrow_hit_2.mp3");
	soundPool[SoundResID::Arrow_Hit_3] = Mix_LoadWAV("Assets/Audio/SFX/arrow_hit_3.mp3");

	soundPool[SoundResID::Axe_Fire] = Mix_LoadWAV("Assets/Audio/SFX/axe_fire.wav");
	soundPool[SoundResID::Axe_Hit_1] = Mix_LoadWAV("Assets/Audio/SFX/axe_hit_1.mp3");
	soundPool[SoundResID::Axe_Hit_2] = Mix_LoadWAV("Assets/Audio/SFX/axe_hit_2.mp3");
	soundPool[SoundResID::Axe_Hit_3] = Mix_LoadWAV("Assets/Audio/SFX/axe_hit_3.mp3");

	soundPool[SoundResID::Shell_Fire] = Mix_LoadWAV("Assets/Audio/SFX/shell_fire.wav");
	soundPool[SoundResID::Shell_Hit] = Mix_LoadWAV("Assets/Audio/SFX/shell_hit.mp3");

	soundPool[SoundResID::Win] = Mix_LoadWAV("Assets/Audio/SFX/win.wav");
	soundPool[SoundResID::Loss] = Mix_LoadWAV("Assets/Audio/SFX/loss.mp3");

	//检查unordered_map容器中所有键值对的值是否有效；其中auto是自动类型推导，成员second访问的是键值对的值
	for (const auto& _pair : soundPool)
		if (!_pair.second) return false;
	//检查无误就返回加载成功
	return true;
}

bool ResourceManager::LoadFontRes()
{
	//从文件中加载字体
	fontPool[FontResID::Pixel_CN] = TTF_OpenFont("Assets/Font/ipix.ttf", 25);

	//检查unordered_map容器中所有键值对的值是否有效；其中auto是自动类型推导，成员second访问的是键值对的值
	for (const auto& _pair : fontPool)
		if (!_pair.second) return false;
	//检查无误就返回加载成功
	return true;
}