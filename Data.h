#pragma once


//-ヘッダーファイル読み込み-

//ゲームのメインヘッダーファイル
#include "game.h"


//-マクロ定義-

//エラーメッセージ
#define CSV_LOAD_ERR_TITLE		TEXT("CSV読み込みエラー")
#define CSV_DATANUM_ERR_TITLE	TEXT("CSVデータ数エラー")


//キャラデータのパス
#define CSV_PATH_ENEMY		TEXT(".\\Data\\enemy.csv")

//データの書式指定子
//キャラクターの書式指定子
#define CSV_CHARA_FORMAT	TEXT("%d,%[^,],%d,%d,%d,%d,%d,%d")

//敵データ
#define ENEMY_MAX	8

//エラーメッセージ
#define SCORE_CREATE_ERR_TITLE	TEXT("スコアデータ作成エラー")
#define SCORE_LOAD_ERR_TITLE	TEXT("スコアデータ読込エラー")
#define SCORE_CREATE_TITLE	TEXT("スコアデータ作成OK!!")
#define SCORE_SAVE_TITLE	TEXT("スコアデータ保存OK!!")
#define SCORE_DELETE_TITLE	TEXT("スコアデータ削除OK!!")
#define SCORE_DELETE_ERR_TITLE	TEXT("スコアデータ削除エラー")

//スコアデータのパス
#define SCORE_DATA_PATH	TEXT(".\\data\\score.a")	//ANSIの文字コード

//データの書式指定子
#define SCORE_DATA_FORMAT	TEXT("%[^,],%d,%[^,],%d,%[^,],%d")	//スコアデータの書式指定子


//キャラデータ構造体(あくまでシンプル)
struct CHARA_DATA
{
	int No;					//No
	char Name[STR_MAX];		//名前

	//実際に使う値
	int HP;					//HP
	int ATK;				//攻撃力
	int DEF;				//防御力
	
	//最大値
	int MAX_HP;				//HP
	int MAX_ATK;			//攻撃力
	int MAX_DEF;			//防御力
};

struct SCORE_DATA
{
	char path[PATH_MAX];	//パス
	char Name1[STR_MAX];	//名前1
	int Score1;				//スコア1
	char Name2[STR_MAX];	//名前2
	int Score2;				//スコア2
	char Name3[STR_MAX];	//名前3
	int Score3;				//スコア3
};


//-外部のグローバル関数-
extern CHARA_DATA enemy[ENEMY_MAX];		//敵データ
extern SCORE_DATA score_data;			//スコアデータ
extern SCORE_DATA score_dataInit;		//スコアデータ初期化用


//-プロトタイプ宣言-

extern BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader);

extern BOOL CreateScoreData(VOID);
extern BOOL SaveScoreData(VOID);
extern BOOL DeleteScoreData(VOID);
extern const char* GetScoreDataPath(VOID);
extern BOOL SetScoreDataPath(const char* path);
