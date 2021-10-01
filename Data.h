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
#define SCORE_CREATE_TITLE		TEXT("スコアデータ作成OK!!")
#define SCORE_SAVE_TITLE		TEXT("スコアデータ保存OK!!")
#define SCORE_DELETE_TITLE		TEXT("スコアデータ削除OK!!")
#define SCORE_DELETE_ERR_TITLE	TEXT("スコアデータ削除エラー")

//スコアデータのパス
#define SCORE_DATA_PATH	TEXT(".\\data\\monika.chr")	//ANSIの文字コード

//データの書式指定子
#define SCORE_DATA_FORMAT	TEXT("%[^,],%d,%[^,],%d,%[^,],%d")	//スコアデータの書式指定子

//マップチップの画像のファイルパス
#define IMG_PATH_MAP1				TEXT(".\\map\\mapchip.png")

//マップチップの画像のファイルパス
#define CSV_PATH_MAP1_SHITA			TEXT(".\\map\\map_sita.csv")
#define CSV_PATH_MAP1_NAKA			TEXT(".\\map\\map_naka.csv")
#define CSV_PATH_MAP1_KAGU			TEXT(".\\map\\map_kagu.csv")
#define CSV_PATH_MAP1_NAKA_ATARI	TEXT(".\\map\\map_naka_atari.csv")
#define CSV_PATH_MAP1_UE			TEXT(".\\map\\map_ue.csv")

//マップの書式指定子
#define CSV_MAP_FORMAT		TEXT("%d,")

#define MAP1_YOKO_MAX			20	//マップの横サイズ
#define MAP1_TATE_MAX			20	//マップの縦サイズ

#define MAP1_YOKO_DIV			60	//マップの横分割数
#define MAP1_TATE_DIV			60	//マップの縦分割数

#define MAP_MOVE_ID				56	//通れるマップID
#define MAP_STOP_ID				57	//通れないマップID
#define MAP_NONE_ID				59	//何もないマップID

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

struct MAP_DATA
{
	//CSVデータ(分割画像のID)
	int CSV_shita[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_kagu[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_naka_atari[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int CSV_ue[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	//マップのハンドル
	int handle[MAP1_TATE_DIV * MAP1_YOKO_DIV];

	//マップの場所
	int x[MAP1_TATE_MAX][MAP1_YOKO_MAX];
	int y[MAP1_TATE_MAX][MAP1_YOKO_MAX];

	int width;
	int height;

	int divMax;
	
	BOOL IsDraw = FALSE;

	//当たり判定
	RECT coll[MAP1_TATE_MAX][MAP1_YOKO_MAX];
};


//-外部のグローバル関数-
extern CHARA_DATA enemy[ENEMY_MAX];		//敵データ
extern SCORE_DATA score_data;			//スコアデータ
extern SCORE_DATA score_dataInit;		//スコアデータ初期化用


//-プロトタイプ宣言-

extern BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader);
extern BOOL LoadScoreData(const char* path, SCORE_DATA* score, BOOL IsHeader);

extern BOOL LoadCSVMap(
	const char* ImgPath,
	const char* shitaPath,
	const char* nakaPath,
	const char* kaguPath,
	const char* nakaatariPath,
	const char* uePath,
	MAP_DATA* map,
	int divYoko,
	int divTate);

extern BOOL CreateScoreData(VOID);
extern BOOL SaveScoreData(VOID);
extern BOOL DeleteScoreData(VOID);
extern const char* GetScoreDataPath(VOID);
extern VOID SetScoreDataPath(const char* path);

extern BOOL CollMap(RECT rect, MAP_DATA map);
extern VOID DrawMap(MAP_DATA map);