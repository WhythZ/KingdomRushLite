#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <unordered_map>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "../Manager.hpp"

//ͼ����Դ����Ƶ��Դ��������Դ
enum class TextureResID
{
	Player_Dragon,                  //��ҿ��Ƶ�����ɫ��֡����
	
	VFX_Player_Dragon_Flash_Up,     //������������磬������
	VFX_Player_Dragon_Flash_Down,   //������������磬������
	VFX_Player_Dragon_Flash_Left,   //������������磬������
	VFX_Player_Dragon_Flash_Right,  //������������磬������
	VFX_Player_Dragon_Impact_Up,    //������������壬������
	VFX_Player_Dragon_Impact_Down,  //������������壬������
	VFX_Player_Dragon_Impact_Left,  //������������壬������
	VFX_Player_Dragon_Impact_Right, //������������壬������

	Tile_TileSet,                   //��Ƭ�ز�ͼ��
	Tile_Home,                      //����Ƭ
	Tile_Spawner,                   //�������ɵ���Ƭ
	Tile_DirectionArrow,            //�н�·������

	Tower_Archer,                   //�����ַ�����
	Tower_Axeman,                   //Ͷ���ַ�����
	Tower_Gunner,	                //ǹ���ַ�����
	
	Bullet_Arrow,                   //����Ͷ����
	Bullet_Axe,                     //����Ͷ����
	Bullet_Shell,                   //�ڵ�Ͷ����
	Bullet_Shell_Explode,           //�ڵ�Ͷ���ﱬը

	Enemy_Slime,                    //ʷ��ķ֡����
	Enemy_Slime_Sketch,             //ʷ��ķ��ɫ��Ӱ�������ܻ���Ч
	Enemy_SlimeKing,                //��ʷ��ķ֡����
	Enemy_SlimeKing_Sketch,         //��ʷ��ķ��ɫ��Ӱ
	Enemy_Skeleton,                 //���ñ�֡����
	Enemy_Skeleton_Sketch,          //���ñ���ɫ��Ӱ
	Enemy_Goblin,                   //�粼��֡����
	Enemy_Goblin_Sketch,            //�粼�ְ�ɫ��Ӱ
	Enemy_GoblinPriest,             //��ʦ�粼��֡����
	Enemy_GoblinPriest_Sketch,      //��ʦ�粼�ְ�ɫ��Ӱ

	Item_Coin,                      //����Ľ��

	UI_Icon_Coin,                   //��ҽ���ܶ�
	UI_Icon_HealthHeart,            //���Ѫ������
	UI_Icon_Player_Dragon,          //�����ͷ��ͼ��
	UI_Icon_Home,                   //��ͷ��ͼ��

	UI_SelectCursor,                //ѡ����Ƭ��Ч��
	UI_Build_Idle,                  //���÷�����������UI
	UI_Build_HoveredTop,            //ָ�������ڷ��������ϲ�ʱ�ķ�����Ч
	UI_Build_HoveredLeft,           //ָ�������ڷ���������ʱ�ķ�����Ч
	UI_Build_HoveredRight,          //ָ�������ڷ��������Ҳ�ʱ�ķ�����Ч
	UI_Upgrade_Idle,                //����������������UI
	UI_Upgrade_HoveredTop,          //ָ�����������������ϲ�ʱ�ķ�����Ч
	UI_Upgrade_HoveredLeft,         //ָ������������������ʱ�ķ�����Ч
	UI_Upgrade_HoveredRight,        //ָ�����������������Ҳ�ʱ�ķ�����Ч
	
	UI_Text_Win,                    //ʤ��������ʾ
	UI_Text_Loss,                   //ʧ��������ʾ
	UI_Bar_GameOver                 //��Ϸ���������
};

enum class MusicResID
{
	BGM_InLevel_0
};

enum class SoundResID
{
	Player_Dragon_Skill_Flash,      //���������������Ч
	Player_Dragon_Skill_Impact,     //���������������Ч

	Pick_Coin,                      //Ӳ�Ҽ�ʰ��Ч
	Home_Hurt,                      //�ұ�������Ч

	Tower_Build,                    //������������Ч
	Tower_Upgrade,                  //������������Ч
	Tower_Error,                    //����������ʧ����Ч

	Bullet_Arrow_Fire_1,            //��ʸͶ����Ч1
	Bullet_Arrow_Fire_2,            //��ʸͶ����Ч2
	Bullet_Arrow_Hit_1,             //��ʸ������Ч1
	Bullet_Arrow_Hit_2,             //��ʸ������Ч2
	Bullet_Arrow_Hit_3,             //��ʸ������Ч3

	Bullet_Axe_Fire,                //��ͷͶ����Ч
	Bullet_Axe_Hit_1,               //��ͷ������Ч1
	Bullet_Axe_Hit_2,               //��ͷ������Ч2
	Bullet_Axe_Hit_3,               //��ͷ������Ч3

	Bullet_Shell_Fire,              //ը��Ͷ����Ч
	Bullet_Shell_Hit,               //ը��������Ч

	Game_Win,                       //��Ϸʤ����Ч
	Game_Loss,                      //��Ϸʧ����Ч
};

enum class FontResID
{
	Ipix,
	VonwaonBitmap12,
	VonwaonBitmap16
};

//ͼ����ࡢ���ֳ��ࡢ�������ࡢ�������
typedef std::unordered_map<TextureResID, SDL_Texture*> TexturePool;
typedef std::unordered_map<MusicResID, Mix_Music*> MusicPool;
typedef std::unordered_map<SoundResID, Mix_Chunk*> SoundPool;
typedef std::unordered_map<FontResID, TTF_Font*> FontPool;

//��Ϸ�ʲ�������
class ResourceManager :public Manager<ResourceManager>
{
	friend class Manager<ResourceManager>;

private:
	TexturePool texturePool;              //ͼ���
	MusicPool musicPool;                  //���ֳ�
	SoundPool soundPool;                  //��Ч��
	FontPool fontPool;                    //�����

public:
	bool LoadResource(SDL_Renderer*);     //����������Դ

	const TexturePool& GetTexturePool();  //��ȡֻ��ͼ���
	const MusicPool& GetMusicPool();      //��ȡֻ����Ƶ��
	const SoundPool& GetSoundPool();      //��ȡֻ����Ч��
	const FontPool& GetFontPool();        //��ȡֻ��ͼ���

private:
	ResourceManager() = default;
	~ResourceManager() = default;

	bool LoadTextureRes(SDL_Renderer*);   //���ļ��м���ͼ���ʲ�
	bool LoadMusicRes();                  //���ļ��м��������ʲ�
	bool LoadSoundRes();                  //���ļ��м�����Ч�ʲ�
	bool LoadFontRes();                   //���ļ��м��������ʲ�
};

#endif
