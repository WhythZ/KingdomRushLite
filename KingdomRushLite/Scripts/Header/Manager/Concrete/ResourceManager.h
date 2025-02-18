#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <unordered_map>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "../Manager.hpp"

//ͼ����Դ����Ƶ��Դ��������Դ
enum TextureResID
{
	Player,                      //��ҿ��ƽ�ɫ��֡����
	VFX_Flash_Up,                //��Ҽ������磬������
	VFX_Flash_Down,              //��Ҽ������磬������
	VFX_Flash_Left,              //��Ҽ������磬������
	VFX_Flash_Right,             //��Ҽ������磬������
	VFX_Impact_Up,               //
	VFX_Impact_Down,             //
	VFX_Impact_Left,             //
	VFX_Impact_Right,            //

	Tile_TileSet,                //��Ƭ�ز�ͼ��
	Tile_Home,                   //����Ƭ
	Tile_Spawner,                //�������ɵ���Ƭ
	Tile_DirectionArrow,         //�н�·������

	Tower_Archer,                //�����ַ�����
	Tower_Axeman,                //Ͷ���ַ�����
	Tower_Gunner,	             //ǹ���ַ�����
	Bullet_Arrow,                //����Ͷ����
	Bullet_Axe,                  //����Ͷ����
	Bullet_Shell,                //�ڵ�Ͷ����
	Bullet_Shell_Explode,        //�ڵ�Ͷ���ﱬը

	Enemy_Slime,                 //ʷ��ķ֡����
	Enemy_SlimeKing,             //��ʷ��ķ֡����
	Enemy_Skeleton,              //���ñ�֡����
	Enemy_Goblin,                //�粼��֡����
	Enemy_GoblinPriest,          //��ʦ�粼��֡����
	Enemy_Slime_Sketch,          //ʷ��ķ��ɫ��Ӱ�������ܻ���Ч
	Enemy_SlimeKing_Sketch,      //��ʷ��ķ��ɫ��Ӱ
	Enemy_Skeleton_Sketch,       //���ñ���ɫ��Ӱ
	Enemy_Goblin_Sketch,         //�粼�ְ�ɫ��Ӱ
	Enemy_GoblinPriest_Sketch,   //��ʦ�粼�ְ�ɫ��Ӱ

	Item_Coin,                   //����Ľ��

	UI_Coin,                     //��ҽ���ܶ�
	UI_Heart,                    //���Ѫ������
	UI_Avatar_Player,            //���ͷ��
	UI_Avatar_Home,              //��ͷ��

	UI_SelectCursor,             //ѡ�ж������Ч
	UI_Place_Idle,               //���÷�����������UI
	UI_Place_HoveredTop,         //ָ�������ڷ��������ϲ�ʱ�ķ�����Ч
	UI_Place_HoveredLeft,        //ָ�������ڷ���������ʱ�ķ�����Ч
	UI_Place_HoveredRight,       //ָ�������ڷ��������Ҳ�ʱ�ķ�����Ч
	UI_Upgrade_Idle,             //����������������UI
	UI_Upgrade_HoveredTop,       //ָ�����������������ϲ�ʱ�ķ�����Ч
	UI_Upgrade_HoveredLeft,      //ָ������������������ʱ�ķ�����Ч
	UI_Upgrade_HoveredRight,     //ָ�����������������Ҳ�ʱ�ķ�����Ч

	UI_Text_Win,                 //ʤ��������ʾ
	UI_Text_Loss,                //ʧ��������ʾ
	UI_GameOver                  //ʧ�ܽ���
};

enum MusicResID
{
	Main,                        //����������
};

enum SoundResID
{
	PlayerSkill_Flash,           //��Ҽ�����Ч
	PlayerSkill_Impact,          //��Ҽ�����Ч

	Pick_Coin,                   //Ӳ�Ҽ�ʰ��Ч
	HomeHurt,                    //�ұ�������Ч

	Tower_Place,                 //������������Ч
	Tower_Upgrade,               //������������Ч

	Arrow_Shoot_1,               //��ʸͶ����Ч1
	Arrow_Shoot_2,               //��ʸͶ����Ч2
	Arrow_Hit_1,                 //��ʸ������Ч1
	Arrow_Hit_2,                 //��ʸ������Ч2
	Arrow_Hit_3,                 //��ʸ������Ч3

	Axe_Shoot,                   //��ͷͶ����Ч
	Axe_Hit_1,                   //��ͷ������Ч1
	Axe_Hit_2,                   //��ͷ������Ч2
	Axe_Hit_3,                   //��ͷ������Ч3

	Shell_Shoot,                 //ը��Ͷ����Ч
	Shell_Hit,                   //ը��������Ч

	Win,                         //��Ϸʤ����Ч
	Loss,                        //��Ϸʧ����Ч
};

enum FontResID
{
	Pixel_CN,                   //��Ϸ�����壨�����ģ�
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
