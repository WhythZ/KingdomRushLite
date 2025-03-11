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
	//SDL_Surface* _imgSurface = IMG_Load("/Assets/xx.jpg");                             //从外存加载到内存（内存结构体）
	//SDL_Texture* _imgTexture = SDL_CreateTextureFromSurface(_renderer, _imgSurface);  //从内存加载到显存（GPU纹理数据）

	//将图片资源加载存储在图像池（unordered_map类型的容器）
	texturePool[TextureResID::Player_Dragon] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Player/PlayerDragon/Player_Dragon.png");

	texturePool[TextureResID::Player_Dragon_Skill_Flash_Up] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Player/PlayerDragon/Player_Dragon_Skill_Flash_Up.png");
	texturePool[TextureResID::Player_Dragon_Skill_Flash_Down] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Player/PlayerDragon/Player_Dragon_Skill_Flash_Down.png");
	texturePool[TextureResID::Player_Dragon_Skill_Flash_Left] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Player/PlayerDragon/Player_Dragon_Skill_Flash_Left.png");
	texturePool[TextureResID::Player_Dragon_Skill_Flash_Right] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Player/PlayerDragon/Player_Dragon_Skill_Flash_Right.png");
	texturePool[TextureResID::Player_Dragon_Skill_Impact_Up] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Player/PlayerDragon/Player_Dragon_Skill_Impact_Up.png");
	texturePool[TextureResID::Player_Dragon_Skill_Impact_Down] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Player/PlayerDragon/Player_Dragon_Skill_Impact_Down.png");
	texturePool[TextureResID::Player_Dragon_Skill_Impact_Left] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Player/PlayerDragon/Player_Dragon_Skill_Impact_Left.png");
	texturePool[TextureResID::Player_Dragon_Skill_Impact_Right] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Player/PlayerDragon/Player_Dragon_Skill_Impact_Right.png");

	texturePool[TextureResID::Tile_TileSet] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Tile/Tile_TileSet.png");
	texturePool[TextureResID::Tile_Home] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Tile/Tile_Home.png");
	texturePool[TextureResID::Tile_Spawner] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Tile/Tile_Spawner.png");
	texturePool[TextureResID::Tile_DirectionArrow] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Tile/Tile_DirectionArrow.png");

	texturePool[TextureResID::Tower_Archer] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Tower/Tower_Archer.png");
	texturePool[TextureResID::Tower_Axeman] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Tower/Tower_Axeman.png");
	texturePool[TextureResID::Tower_Gunner] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Tower/Tower_Gunner.png");

	texturePool[TextureResID::Bullet_Arrow] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Bullet/Bullet_Arrow.png");
	texturePool[TextureResID::Bullet_Axe] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Bullet/Bullet_Axe.png");
	texturePool[TextureResID::Bullet_Shell] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Bullet/Bullet_Shell.png");
	texturePool[TextureResID::Bullet_Shell_Explode] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Bullet/Bullet_Shell_Explode.png");

	texturePool[TextureResID::Enemy_Slime] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Enemy/Enemy_Slime.png");
	texturePool[TextureResID::Enemy_Slime_Sketch] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Enemy/Enemy_Slime_Sketch.png");
	texturePool[TextureResID::Enemy_SlimeKing] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Enemy/Enemy_SlimeKing.png");
	texturePool[TextureResID::Enemy_SlimeKing_Sketch] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Enemy/Enemy_SlimeKing_Sketch.png");
	texturePool[TextureResID::Enemy_Skeleton] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Enemy/Enemy_Skeleton.png");
	texturePool[TextureResID::Enemy_Skeleton_Sketch] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Enemy/Enemy_Skeleton_Sketch.png");
	texturePool[TextureResID::Enemy_Goblin] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Enemy/Enemy_Goblin.png");
	texturePool[TextureResID::Enemy_Goblin_Sketch] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Enemy/Enemy_Goblin_Sketch.png");
	texturePool[TextureResID::Enemy_GoblinPriest] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Enemy/Enemy_GoblinPriest.png");
	texturePool[TextureResID::Enemy_GoblinPriest_Sketch] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Enemy/Enemy_GoblinPriest_Sketch.png");

	texturePool[TextureResID::Item_Coin] = IMG_LoadTexture(_renderer, "/Assets/Sprites/Item/Item_Coin.png");

	texturePool[TextureResID::UI_Icon_Coin] = IMG_LoadTexture(_renderer, "/Assets/Sprites/UI/UI_Icon_Coin.png");
	texturePool[TextureResID::UI_Icon_HealthHeart] = IMG_LoadTexture(_renderer, "/Assets/Sprites/UI/UI_Icon_HealthHeart.png");
	texturePool[TextureResID::UI_Icon_Player_Dragon] = IMG_LoadTexture(_renderer, "/Assets/Sprites/UI/UI_Icon_Player_Dragon.png");

	texturePool[TextureResID::UI_SelectCursor] = IMG_LoadTexture(_renderer, "/Assets/Sprites/UI/UI_SelectCursor.png");
	texturePool[TextureResID::UI_Build_Idle] = IMG_LoadTexture(_renderer, "/Assets/Sprites/UI/UI_Build_Idle.png");
	texturePool[TextureResID::UI_Build_HoveredTop] = IMG_LoadTexture(_renderer, "/Assets/Sprites/UI/UI_Build_HoveredTop.png");
	texturePool[TextureResID::UI_Build_HoveredLeft] = IMG_LoadTexture(_renderer, "/Assets/Sprites/UI/UI_Build_HoveredLeft.png");
	texturePool[TextureResID::UI_Build_HoveredRight] = IMG_LoadTexture(_renderer, "/Assets/Sprites/UI/UI_Build_HoveredRight.png");
	texturePool[TextureResID::UI_Upgrade_Idle] = IMG_LoadTexture(_renderer, "/Assets/Sprites/UI/UI_Upgrade_Idle.png");
	texturePool[TextureResID::UI_Upgrade_HoveredTop] = IMG_LoadTexture(_renderer, "/Assets/Sprites/UI/UI_Upgrade_HoveredTop.png");
	texturePool[TextureResID::UI_Upgrade_HoveredLeft] = IMG_LoadTexture(_renderer, "/Assets/Sprites/UI/UI_Upgrade_HoveredLeft.png");
	texturePool[TextureResID::UI_Upgrade_HoveredRight] = IMG_LoadTexture(_renderer, "/Assets/Sprites/UI/UI_Upgrade_HoveredRight.png");

	//检查unordered_map容器中所有键值对的值是否有效；其中auto是自动类型推导，成员second访问的是键值对的值
	for (const auto& _pair : texturePool)
		if (!_pair.second) return false;
	//检查无误就返回加载成功
	return true;
}

bool ResourceManager::LoadMusicRes()
{
	//从文件中加载音乐
	musicPool[MusicResID::BGM_InLevel_0] = Mix_LoadMUS("/Assets/Audios/Musics/BGM_InLevel_0.flac");

	//检查unordered_map容器中所有键值对的值是否有效；其中auto是自动类型推导，成员second访问的是键值对的值
	for (const auto& _pair : musicPool)
		if (!_pair.second) return false;
	//检查无误就返回加载成功
	return true;
}

bool ResourceManager::LoadSoundRes()
{
	//从文件中加载音效
	soundPool[SoundResID::Player_Dragon_Skill_Flash] = Mix_LoadWAV("/Assets/Audios/Sounds/Player_Dragon_Skill_Flash.ogg");
	soundPool[SoundResID::Player_Dragon_Skill_Impact] = Mix_LoadWAV("/Assets/Audios/Sounds/Player_Dragon_Skill_Impact.ogg");

	soundPool[SoundResID::Pick_Coin] = Mix_LoadWAV("/Assets/Audios/Sounds/Pick_Coin.ogg");
	soundPool[SoundResID::Home_Hurt] = Mix_LoadWAV("/Assets/Audios/Sounds/Home_Hurt.ogg");

	soundPool[SoundResID::Bullet_Arrow_Fire_1] = Mix_LoadWAV("/Assets/Audios/Sounds/Bullet_Arrow_Fire_1.ogg");
	soundPool[SoundResID::Bullet_Arrow_Fire_2] = Mix_LoadWAV("/Assets/Audios/Sounds/Bullet_Arrow_Fire_2.ogg");
	soundPool[SoundResID::Bullet_Arrow_Hit_1] = Mix_LoadWAV("/Assets/Audios/Sounds/Bullet_Arrow_Hit_1.ogg");
	soundPool[SoundResID::Bullet_Arrow_Hit_2] = Mix_LoadWAV("/Assets/Audios/Sounds/Bullet_Arrow_Hit_2.ogg");
	soundPool[SoundResID::Bullet_Arrow_Hit_3] = Mix_LoadWAV("/Assets/Audios/Sounds/Bullet_Arrow_Hit_3.ogg");

	soundPool[SoundResID::Bullet_Axe_Fire] = Mix_LoadWAV("/Assets/Audios/Sounds/Bullet_Axe_Fire.ogg");
	soundPool[SoundResID::Bullet_Axe_Hit_1] = Mix_LoadWAV("/Assets/Audios/Sounds/Bullet_Axe_Hit_1.ogg");
	soundPool[SoundResID::Bullet_Axe_Hit_2] = Mix_LoadWAV("/Assets/Audios/Sounds/Bullet_Axe_Hit_2.ogg");
	soundPool[SoundResID::Bullet_Axe_Hit_3] = Mix_LoadWAV("/Assets/Audios/Sounds/Bullet_Axe_Hit_3.ogg");

	soundPool[SoundResID::Bullet_Shell_Fire] = Mix_LoadWAV("/Assets/Audios/Sounds/Bullet_Shell_Fire.ogg");
	soundPool[SoundResID::Bullet_Shell_Hit] = Mix_LoadWAV("/Assets/Audios/Sounds/Bullet_Shell_Hit.ogg");

	soundPool[SoundResID::UI_Tower_Show_Panel] = Mix_LoadWAV("/Assets/Audios/Sounds/UI_Tower_Show_Panel.ogg");
	soundPool[SoundResID::UI_Tower_Build] = Mix_LoadWAV("/Assets/Audios/Sounds/UI_Tower_Build.ogg");
	soundPool[SoundResID::UI_Tower_Upgrade] = Mix_LoadWAV("/Assets/Audios/Sounds/UI_Tower_Upgrade.ogg");
	soundPool[SoundResID::UI_Tower_Error] = Mix_LoadWAV("/Assets/Audios/Sounds/UI_Tower_Error.ogg");

	soundPool[SoundResID::UI_Game_Win] = Mix_LoadWAV("/Assets/Audios/Sounds/UI_Game_Win.ogg");
	soundPool[SoundResID::UI_Game_Loss] = Mix_LoadWAV("/Assets/Audios/Sounds/UI_Game_Loss.ogg");

	//检查unordered_map容器中所有键值对的值是否有效；其中auto是自动类型推导，成员second访问的是键值对的值
	for (const auto& _pair : soundPool)
		if (!_pair.second) return false;
	//检查无误就返回加载成功
	return true;
}

bool ResourceManager::LoadFontRes()
{
	//从文件中加载字体
	fontPool[FontResID::Ipix] = TTF_OpenFont("/Assets/Fonts/Ipix.ttf", 25);
	fontPool[FontResID::VonwaonBitmap12] = TTF_OpenFont("/Assets/Fonts/VonwaonBitmap-12pxLite.ttf", 12);
	fontPool[FontResID::VonwaonBitmap16] = TTF_OpenFont("/Assets/Fonts/VonwaonBitmap-16pxLite.ttf", 16);

	//检查unordered_map容器中所有键值对的值是否有效；其中auto是自动类型推导，成员second访问的是键值对的值
	for (const auto& _pair : fontPool)
		if (!_pair.second) return false;
	//检查无误就返回加载成功
	return true;
}