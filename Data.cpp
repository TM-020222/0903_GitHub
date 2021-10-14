//-ヘッダーファイル読み込み-
#include "game.h"
#include "Data.h"
#include "shape.h"	//図形の処理


//-グローバル関数-
CHARA_DATA enemy[ENEMY_MAX];	//敵データ

SCORE_DATA score_data;
SCORE_DATA score_dataInit =		//スコアデータ
{ SCORE_DATA_PATH,"NAME1",-1,"NAME2",-1,"NAME3",-1 };//スコアデータ初期化用

//MAP_DATA map1;			//マップ1


//-関数-

/// <summary>
/// CSVファイルを読み込む
/// </summary>
/// <param name="path">CSVファイルのパス</param>
/// <param name="chara">チャラデータ構造体の先頭アドレス</param>
/// <param name="DataMax">CSVファイルの最大数</param>
/// <param name="IsHeader">CSVファイルの一行目がヘッダならTRUE</param>
/// <returns>TRUEは読み込めた/FALSEは失敗した</returns>
BOOL LoadCSVChara(const char* path, CHARA_DATA* chara, int DataMax, BOOL IsHeader = FALSE)
{
	FILE* fp;				//ファイルポインタ
	char GetChar = '\0';	//取得する文字
	int LoopCnt = 0;		//ループカウンタ
	errno_t FileErr = NULL;	//ファイル読み込みエラーを確認
	int IsEOF = NULL;		//ファイルの終わりかチェック

	//ファイルを読み込みモード(r)で開く
	FileErr = fopen_s(&fp, path, "r");
	if (FileErr != NULL)
	{
		//読み込みエラー
		MessageBox(GetMainWindowHandle(), path, CSV_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}


	//正常に読み込めたら下へ

	//CSVの一行目がヘッダの時
	if (IsHeader == TRUE)
	{
		//一文字ずつ取り出し、改行をヒントに一行目を読み飛ばす
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	//実際にデータを格納する
	while (IsEOF != EOF)	//End Of File(ファイルの最後)でないとき繰り返す
	{
		//CSVのデータの一行分を読み取る
		IsEOF = fscanf_s(
			fp,
			CSV_CHARA_FORMAT,
			&(chara + LoopCnt)->No,
			&(chara + LoopCnt)->Name[0], STR_MAX,
			&(chara + LoopCnt)->HP,
			&(chara + LoopCnt)->ATK,
			&(chara + LoopCnt)->DEF,
			&(chara + LoopCnt)->MAX_HP,
			&(chara + LoopCnt)->MAX_ATK,
			&(chara + LoopCnt)->MAX_DEF
			);

		//次のデータへ
		LoopCnt++;
	}
	
	//データの数が違うとき
	if (LoopCnt - 1 != DataMax)
	{
		//読み込みエラー
		MessageBox(GetMainWindowHandle(), path, CSV_DATANUM_ERR_TITLE, MB_OK);
		return FALSE;
	}

	fclose(fp);		//ファイルを閉じる

	return TRUE;
}

/// <summary>
/// スコアデータを読み込む
/// </summary>
/// <param name="path">スコアデータのパス</param>
/// <param name="score">スコアデータ構造体の先頭アドレス</param>
/// <param name="IsHeader">CSVファイルの一行目がヘッダならTRUE</param>
/// <returns>読み込めたならTRUE</returns>
BOOL LoadScoreData(const char* path, SCORE_DATA* score, BOOL IsHeader = FALSE)
{
	FILE* fp;					//ファイルポインタ
	char GetChar = '\0';		//取得する文字
	errno_t FileErr = NULL;		//ファイル読み込みエラーを確認
	int IsEOF = NULL;			//ファイルの終わりかチェック

	//パスのコピー
	SetScoreDataPath(path);

	FileErr = fopen_s(&fp, path, "r");	//ファイルを読み込みモードで開く
	if (FileErr != NULL)
	{
		//スコアデータ作成
		if (CreateScoreData() == FALSE) { return FALSE; }

		//スコアステータス作成されているはず

		FileErr = fopen_s(&fp, path, "r");
		if (FileErr != NULL)
		{
			MessageBox(GetMainWindowHandle(), path, SCORE_LOAD_ERR_TITLE, MB_OK);
			return FALSE;
		}
	}

	//-正常に読み込めたなら-

	//CSVの一行目がヘッダの時
	if (IsHeader == TRUE)
	{
		//1文字ずつ取り出し、一行目を読み飛ばす
		while (GetChar != '\n') { GetChar = fgetc(fp); }
	}

	//データを格納する
	while (IsEOF != EOF)	//End Of File でないとき繰り返す
	{
		//CSVのデータを1行分読み取る
		IsEOF = fscanf_s(
			fp,
			SCORE_DATA_FORMAT,	//データが「1行=1列」で書かれている
			//文字列を読み込むときは、次の因数で文字列最大数を指定する
			&(score)->Name1, STR_MAX,
			&(score)->Score1,
			&(score)->Name2, STR_MAX,
			&(score)->Score2,
			&(score)->Name3, STR_MAX,
			&(score)->Score3
		);
	}

	fclose(fp);	//ファイルを閉じる

	return TRUE;
}

/// <summary>
/// スコアデータ作成
/// </summary>
BOOL CreateScoreData(VOID)
{
	//なかったらスコアデータを作る
	FILE* crefp;
	errno_t creFileErr = NULL;	//ファイル読み込みエラーを確認
	creFileErr = fopen_s(&crefp, GetScoreDataPath(), "w");	//ファイルを書き込みモードで開く
	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//ヘッダを書き込み
	fprintf_s(crefp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//からのスコアデータを書き込み
	fprintf_s(crefp,
		"%s,%d,%s,%d,%s,%d",
		score_dataInit.Name1, score_dataInit.Score1,
		score_dataInit.Name2, score_dataInit.Score2,
		score_dataInit.Name3, score_dataInit.Score3
	);

	fclose(crefp);	//スコアデータを閉じる

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_TITLE, MB_OK);

	return TRUE;
}

/// <summary>
/// スコアデータ保存
/// </summary>
BOOL SaveScoreData(VOID)
{
	//なかったらスコアデータを作る
	FILE* crefp;
	errno_t creFileErr = NULL;	//ファイル読み込みエラーを確認
	creFileErr = fopen_s(&crefp, GetScoreDataPath(), "w");	//ファイルを書き込みモードで開く
	if (creFileErr != NULL)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_CREATE_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//ヘッダを書き込み
	fprintf_s(crefp,
		"Name1,Score1,Name2,Score2,Name3,Score3\n"
	);

	//スコアデータを書き込み
	fprintf_s(crefp,
		"%s,%d,%s,%d,%s,%d",
		score_data.Name1, score_data.Score1,
		score_data.Name2, score_data.Score2,
		score_data.Name3, score_data.Score3
	);

	fclose(crefp);	//スコアデータを閉じる

	MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_SAVE_TITLE, MB_OK);

	return TRUE;
}

/// <summary>
/// スコアデータの削除
/// </summary>
/// <param name="path">スコアデータのパス</param>
BOOL DeleteScoreData(VOID)
{
	//ファイル削除
	if (remove(GetScoreDataPath()) != 0)
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_ERR_TITLE, MB_OK);
	}
	else
	{
		MessageBox(GetMainWindowHandle(), GetScoreDataPath(), SCORE_DELETE_TITLE, MB_OK);
	}

	//スコアデータを作成
	return CreateScoreData();
}

/// <summary>
/// スコアデータのパスを返す
/// </summary>
const char* GetScoreDataPath(VOID)
{
	return score_data.path;
}

/// <summary>
/// スコアデータのパスを設定する
/// </summary>
VOID SetScoreDataPath(const char* path)
{
	strcpyDx(score_data.path, path);
	return;
}

/// <summary>
/// CSVファイルを読み込む
/// </summary>
/// <param name="ImgPath">マップチップの画像ファイルのパス</param>
/// <param name="shitaPath">下ファイルのパス</param>
/// <param name="nakaPath">中ファイルのパス</param>
/// <param name="kaguPath">家具ファイルのパス</param>
/// <param name="nakaatariPath">中当たり判定ファイルのパス</param>
/// <param name="uePath">上ファイルのパス</param>
/// <param name="map">マップデータ構造体の先頭アドレス</param>
/// <param name="divYoko">横の分割数</param>
/// <param name="divTate">縦の分割数</param>
/// <returns>TRUE or FALSE</returns>
BOOL LoadCSVMap(
	const char* ImgPath,
	const char* shitaPath,
	const char* nakaPath,
	const char* kaguPath,
	const char* nakaatariPath,
	const char* uePath,
	MAP_DATA* map,
	int divYoko,
	int divTate)
{
	//マップチップを読み込み
	{
		int IsLoad = -1;	//画像を読み込めたか

		//一時的に画像のハンドルを用意する
		int TempHandle = LoadGraph(ImgPath);

		//読み込みエラー
		if (TempHandle == -1)
		{
			MessageBox
			(
				GetMainWindowHandle(),		//ウィンドウハンドル
				ImgPath,					//本文
				"画像読み込みエラー",		//タイトル
				MB_OK						//ボタン
			);

			return FALSE;	//読み込み失敗
		}

		//画像の幅と高さを取得
		int width = -1;		//幅
		int height = -1;	//高さ
		GetGraphSize(TempHandle, &width, &height);

		//分割して読み込み
		IsLoad = LoadDivGraph
		(
			ImgPath,							//画像のパス
			divYoko * divTate,					//分割総数
			divYoko, divTate,					//横の分割、縦の分割
			width / divYoko, height / divTate,	//画像一つ分の幅、高さ
			map->handle							//連続で管理する配列の先頭アドレス
		);

		if (IsLoad == -1)
		{
			MessageBox
			(
				GetMainWindowHandle(),		//ウィンドウハンドル
				ImgPath,					//本文
				"画像分割エラー",			//タイトル
				MB_OK						//ボタン
			);

			return FALSE;	//読み込み失敗
		}

		//情報を設定
		map->divMax = divYoko * divTate;
		GetGraphSize(map->handle[0], &map->width, &map->height);

		//画像を描画する
		map->IsDraw = TRUE;

		//一時的に読み込んだハンドルを解放
		DeleteGraph(TempHandle);
	}

	FILE* fp;					//ファイルポインタ
	char GetChar = '\0';		//取得する文字
	int LoopCnt = 0;			//ループカウンタ
	errno_t FileErr = NULL;		//ファイル読み込みエラーを確認
	int IsEOF = NULL;			//ファイルの終わりかチェック

	//下CSV
	{
		FileErr = fopen_s(&fp, shitaPath, "r");		//ファイルを読み込みモードで開く
		if (FileErr != NULL)
		{
			//読み込みエラー
			MessageBox
			(
				GetMainWindowHandle(),
				shitaPath,
				CSV_LOAD_ERR_TITLE,
				MB_OK
			);

			return FALSE;
		}

		//正常に読み込めたなら

		//データを格納する
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File
		{
			//ファイルから数値を一つ読み込み、配列に格納する
			//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]は
			//[3][3]の配列の時、LoopCntが5なら
			//[5/3=1][5%3=2]となり、1次元配列の位置を二次元配列として変換できる
			IsEOF = fscanf_s
			(
				fp,
				CSV_MAP_FORMAT,	//データがXX,で書かれている
				&map->CSV_shita[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//次のデータへ
			LoopCnt++;
		}
		fclose(fp);	//ファイルを閉じる
	}

	//中CSV
	{
		FileErr = fopen_s(&fp, nakaPath, "r");		//ファイルを読み込みモードで開く
		if (FileErr != NULL)
		{
			//読み込みエラー
			MessageBox
			(
				GetMainWindowHandle(),
				nakaPath,
				CSV_LOAD_ERR_TITLE,
				MB_OK
			);

			return FALSE;
		}

		//正常に読み込めたなら

		//データを格納する
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File
		{
			//ファイルから数値を一つ読み込み、配列に格納する
			//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]は
			//[3][3]の配列の時、LoopCntが5なら
			//[5/3=1][5%3=2]となり、1次元配列の位置を二次元配列として変換できる
			IsEOF = fscanf_s
			(
				fp,
				CSV_MAP_FORMAT,	//データがXX,で書かれている
				&map->CSV_naka[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//次のデータへ
			LoopCnt++;
		}
		fclose(fp);	//ファイルを閉じる
	}

	//家具等のCSV
	{
		FileErr = fopen_s(&fp, kaguPath, "r");		//ファイルを読み込みモードで開く
		if (FileErr != NULL)
		{
			//読み込みエラー
			MessageBox
			(
				GetMainWindowHandle(),
				kaguPath,
				CSV_LOAD_ERR_TITLE,
				MB_OK
			);

			return FALSE;
		}

		//正常に読み込めたなら

		//データを格納する
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File
		{
			//ファイルから数値を一つ読み込み、配列に格納する
			//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]は
			//[3][3]の配列の時、LoopCntが5なら
			//[5/3=1][5%3=2]となり、1次元配列の位置を二次元配列として変換できる
			IsEOF = fscanf_s
			(
				fp,
				CSV_MAP_FORMAT,	//データがXX,で書かれている
				&map->CSV_kagu[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//次のデータへ
			LoopCnt++;
		}
		fclose(fp);	//ファイルを閉じる
	}

	//中当たり判定CSVを読み取り
	{
		FileErr = fopen_s(&fp, nakaatariPath, "r");		//ファイルを読み込みモードで開く
		if (FileErr != NULL)
		{
			//読み込みエラー
			MessageBox
			(
				GetMainWindowHandle(),
				nakaatariPath,
				CSV_LOAD_ERR_TITLE,
				MB_OK
			);

			return FALSE;
		}

		//正常に読み込めたなら

		//データを格納する
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File
		{
			//ファイルから数値を一つ読み込み、配列に格納する
			//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]は
			//[3][3]の配列の時、LoopCntが5なら
			//[5/3=1][5%3=2]となり、1次元配列の位置を二次元配列として変換できる
			IsEOF = fscanf_s
			(
				fp,
				CSV_MAP_FORMAT,	//データがXX,で書かれている
				&map->CSV_naka_atari[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//次のデータへ
			LoopCnt++;
		}
		fclose(fp);	//ファイルを閉じる
	}

	//上CSV
	{
		FileErr = fopen_s(&fp, uePath, "r");		//ファイルを読み込みモードで開く
		if (FileErr != NULL)
		{
			//読み込みエラー
			MessageBox
			(
				GetMainWindowHandle(),
				uePath,
				CSV_LOAD_ERR_TITLE,
				MB_OK
			);

			return FALSE;
		}

		//正常に読み込めたなら

		//データを格納する
		IsEOF = NULL; LoopCnt = 0;
		while (IsEOF != EOF)	//End Of File
		{
			//ファイルから数値を一つ読み込み、配列に格納する
			//[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]は
			//[3][3]の配列の時、LoopCntが5なら
			//[5/3=1][5%3=2]となり、1次元配列の位置を二次元配列として変換できる
			IsEOF = fscanf_s
			(
				fp,
				CSV_MAP_FORMAT,	//データがXX,で書かれている
				&map->CSV_ue[LoopCnt / MAP1_YOKO_MAX][LoopCnt % MAP1_YOKO_MAX]
			);

			//次のデータへ
			LoopCnt++;
		}
		fclose(fp);	//ファイルを閉じる
	}

	//マップの当たり判定を作成
	{
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				//通れないIDなら
				if (map->CSV_naka_atari[tate][yoko] == MAP_STOP_ID)
				{
					//当たり判定を作成
					map->coll[tate][yoko].left = (yoko + 0) * map->width + 1;
					map->coll[tate][yoko].right = (yoko + 1) * map->width - 1;
					map->coll[tate][yoko].top = (tate + 0) * map->height + 1;
					map->coll[tate][yoko].bottom = (tate + 1) * map->height - 1;
				}
				else
				{
					map->coll[tate][yoko].left = 0;
					map->coll[tate][yoko].right = 0;
					map->coll[tate][yoko].top = 0;
					map->coll[tate][yoko].bottom = 0;
				}
				
				//マップの場所を指定
				map->x[tate][yoko] = (yoko + 0) * map->width;
				map->y[tate][yoko] = (tate + 0) * map->height;
			}
		}
	}

	return TRUE;
}

/// <summary>
/// マップ当たり判定
/// </summary>
/// <returns>当たったらTRUE or 当たらなかったらFALSE</returns>
BOOL CollMap(RECT rect, MAP_DATA map)
{
	//当たり判定チェック
	for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
		{
			if (CheckCollRectToRect(rect, map.coll[tate][yoko]) == TRUE)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

/// <summary>
/// マップ描画
/// </summary>
/// <param name="map">マップ構造体</param>
VOID DrawMap(MAP_DATA map)
{
	//画像が描画できるときは
	if (map.IsDraw == TRUE)
	{
		//下レイヤー
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(
					map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_shita[tate][yoko]], TRUE);
			}
		}

		//中レイヤー
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(
					map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_naka[tate][yoko]], TRUE);
			}
		}

		//家具レイヤー
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(
					map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_kagu[tate][yoko]], TRUE);
			}
		}

		//プレイヤーの画像を描画
		DrawDivImageChara(&samplePlayerImg);	//サンプル分割画像の描画

		//上レイヤー
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				DrawGraph(
					map.x[tate][yoko],
					map.y[tate][yoko],
					map.handle[map.CSV_ue[tate][yoko]], TRUE);
			}
		}
	}

	//デバッグモードの時は
	if (GAME_DEBUG == TRUE)
	{
		for (int tate = 0; tate < MAP1_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < MAP1_YOKO_MAX; yoko++)
			{
				//当たり判定の描画
				DrawRect(map.coll[tate][yoko], GetColor(255, 255, 255), FALSE);
			}
		}
	}

	return;
}

extern VOID CreateEventMass(int x, int y, EVENT* events, MAP_DATA map)
{
	events->x = x;
	events->y = y;

	events->coll.top = map.height * events->y - 1;
	events->coll.left = map.width * events->x - 1;
	events->coll.bottom = map.height * (events->y + 1) + 1;
	events->coll.right = map.width * (events->x + 1) + 1;

	return;
}

extern VOID CreateEventMultiMass(int x1, int y1, int x2, int y2, EVENT* events, MAP_DATA map)
{
	events->x = x1;
	events->y = y1;

	events->coll.top = map.height * y1 - 1;
	events->coll.left = map.width * x2 - 1;
	events->coll.bottom = map.height * (y2 + 1) + 1;
	events->coll.right = map.width * (x1 + 1) + 1;

	return;
}