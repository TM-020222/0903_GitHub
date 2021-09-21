#pragma once


//-�w�b�_�[�t�@�C���ǂݍ���-

//�Q�[���̃��C���w�b�_�[�t�@�C��
#include "game.h"


//-�}�N����`-

//�G���[���b�Z�[�W
#define CSV_LOAD_ERR_TITLE		TEXT("CSV�ǂݍ��݃G���[")
#define CSV_DATANUM_ERR_TITLE	TEXT("CSV�f�[�^���G���[")


//�L�����f�[�^�̃p�X
#define CSV_PATH_ENEMY		TEXT(".\\Data\\enemy.csv")

//�f�[�^�̏����w��q
//�L�����N�^�[�̏����w��q
#define CSV_CHARA_FORMAT	TEXT("%d,%[^,],%d,%d,%d,%d,%d,%d")

//�G�f�[�^
#define ENEMY_MAX	8

//�G���[���b�Z�[�W
#define SCORE_CREATE_ERR_TITLE	TEXT("�X�R�A�f�[�^�쐬�G���[")
#define SCORE_LOAD_ERR_TITLE	TEXT("�X�R�A�f�[�^�Ǎ��G���[")
#define SCORE_CREATE_TITLE	TEXT("�X�R�A�f�[�^�쐬OK!!")
#define SCORE_SAVE_TITLE	TEXT("�X�R�A�f�[�^�ۑ�OK!!")
#define SCORE_DELETE_TITLE	TEXT("�X�R�A�f�[�^�폜OK!!")
#define SCORE_DELETE_ERR_TITLE	TEXT("�X�R�A�f�[�^�폜�G���[")

//�X�R�A�f�[�^�̃p�X
#define SCORE_DATA_PATH	TEXT(".\\data\\score.a")	//ANSI�̕����R�[�h

//�f�[�^�̏����w��q
#define SCORE_DATA_FORMAT	TEXT("%[^,],%d,%[^,],%d,%[^,],%d")	//�X�R�A�f�[�^�̏����w��q


//�L�����f�[�^�\����(�����܂ŃV���v��)
struct CHARA_DATA
{
	int No;					//No
	char Name[STR_MAX];		//���O

	//���ۂɎg���l
	int HP;					//HP
	int ATK;				//�U����
	int DEF;				//�h���
	
	//�ő�l
	int MAX_HP;				//HP
	int MAX_ATK;			//�U����
	int MAX_DEF;			//�h���
};

struct SCORE_DATA
{
	char path[PATH_MAX];	//�p�X
	char Name1[STR_MAX];	//���O1
	int Score1;				//�X�R�A1
	char Name2[STR_MAX];	//���O2
	int Score2;				//�X�R�A2
	char Name3[STR_MAX];	//���O3
	int Score3;				//�X�R�A3
};


//-�O���̃O���[�o���֐�-
extern CHARA_DATA enemy[ENEMY_MAX];		//�G�f�[�^
extern SCORE_DATA score_data;			//�X�R�A�f�[�^
extern SCORE_DATA score_dataInit;		//�X�R�A�f�[�^�������p


//-�v���g�^�C�v�錾-

extern BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader);

extern BOOL CreateScoreData(VOID);
extern BOOL SaveScoreData(VOID);
extern BOOL DeleteScoreData(VOID);
extern const char* GetScoreDataPath(VOID);
extern BOOL SetScoreDataPath(const char* path);
