#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "LC1B_26_フミモトコウサク";

const int kWindowWidth = 1260;
const int kWindowHeight = 720;

// 構造体
typedef struct Vector2 {
	float x;
	float y;
};

typedef struct Ball {
	Vector2 pos;
	Vector2 speed;
	float radius;
	int unsigned color;
};

// 関数
// 練習
// 使えそうな関数
float EaseIn_Out(float scale, float radius, float firstScale, float endScale) {
	float easedT = 0.0f;
	easedT = -(cosf(M_PI * scale) - 1.0f) / 2.0f;
	radius = (1.0f - easedT) * firstScale + easedT * endScale;
	return radius;
}

float EaseIn(float scale, float radius, float firstScale, float endScale) {
	float easedT = scale * scale;
	radius = (1.0f - easedT) * firstScale + easedT * endScale;
	return radius;
}

float EaseOut(float scale, float radius, float firstScale, float endScale) {
	float easedT = 1.0f - powf(1.0f - scale, 3.0f);
	radius = (1.0f - easedT) * firstScale + easedT * endScale;
	return radius;
}


float EaseOut_length(float t, float playerPos, float firstPos, float endPos) {
	float easedT = 1.0f - powf(1.0f - t, 3.0f);
	playerPos = (1.0f - easedT) * firstPos + easedT * endPos;
	return playerPos;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// 宣言
	// 矩形(スケールをイージングで変更される矩形)
	Ball ball = {
		{kWindowWidth / 2,kWindowHeight / 2},
		{4.0f,4.0f},
		500.0f,
		BLACK
	};
	// player(WASDで動く円)
	Ball player = {
		{100.0f,100.0f},
		{4.0f,4.0f},
		32.0f,
		WHITE
	};

	// Blinkについて
	Ball blinkPlayer = {
		0.0f,0.0f
	};
	// 移動距離
	float length = 0.0f;
	// カウント
	int blinkCount = 0;
	int blinkCT = 60;

	// フラグ
	int isEaseIn = false;
	int isEaseIn_Out[2] = { false,false };
	int isBlink = false;
	int isRadiusReset = false;
	int isBlinkCount = false;

	// イージング
	float scale = 0.0f;
	float t = 0.0f;
	float easedT = scale * scale;
	// playerの向き
	enum PlayerDir {
		FRONT,
		BACK,
		LEFT,
		RIGHT
	};
	PlayerDir playerDir = LEFT;
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		// playerの移動処理
		if (keys[DIK_W]) {
			player.pos.y -= player.speed.y;
			playerDir = BACK;
		}
		if (keys[DIK_A]) {
			player.pos.x -= player.speed.x;
			playerDir = LEFT;
		}
		if (keys[DIK_S]) {
			player.pos.y += player.speed.y;
			playerDir = FRONT;
		}
		if (keys[DIK_D]) {
			player.pos.x += player.speed.x;
			playerDir = RIGHT;
		}


		if (!keys[DIK_1] && preKeys[DIK_1]) {
			if (isBlinkCount == 0) {
				isBlink = true;
			}
		}
		if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
			isEaseIn_Out[0] = true;
		}


		// Rキーで円の大きさをリセット
		if (!keys[DIK_R] && preKeys[DIK_R]) {
			if (!isRadiusReset) {
				isRadiusReset = true;
			}
		}
		if (isRadiusReset) {
			scale = 0.0f;
			ball.radius = 0.0f;
			isEaseIn_Out[0] = false;
			isRadiusReset = false;
		}

		// ease_in && ease_out
		// scale
		if (isEaseIn_Out[0]) {
			scale += 1.0 / 15.0f;
			ball.radius = EaseIn_Out(scale, ball.radius, 1.0f, 100.0f);
			if (scale >= 20.0f / 15.0f) {
				isEaseIn_Out[0] = false;
				scale = 0.0f;
				isEaseIn_Out[1] = true;
			}
		}
		if (isEaseIn_Out[1]) {
			scale += 1.0f / 10.0f;
			ball.radius = EaseIn_Out(scale, ball.radius, 75.0f, 90.0f);
			if (scale >= 10.0f / 10.0f) {
				isEaseIn_Out[1] = false;
			}
		}
		if (isEaseIn) {
			scale += 1.0 / 15.0f;
			ball.radius = EaseIn(scale, ball.radius, 100, 500);
		}


		// Blink移動
		if (isBlink) {
			t += 1.0f / 8.0f;
			if (blinkCount == 0) {
				if (playerDir == LEFT) {
					length = -300.0f;
					player.pos.x = EaseOut(t, blinkPlayer.pos.x, blinkPlayer.pos.x, blinkPlayer.pos.x + length);
				}
				if (playerDir == RIGHT) {
					length = 300.0f;
					player.pos.x = EaseOut(t, blinkPlayer.pos.x, blinkPlayer.pos.x, blinkPlayer.pos.x + length);
				}
				if (playerDir == FRONT) {
					length = 300.0f;
					player.pos.y = EaseOut(t, blinkPlayer.pos.y, blinkPlayer.pos.y, blinkPlayer.pos.y + length);
				}
				if (playerDir == BACK) {
					length = -300.0f;
					player.pos.y = EaseOut(t, blinkPlayer.pos.y, blinkPlayer.pos.y, blinkPlayer.pos.y + length);
				}
				if (t >= 1.0f) {
					isBlinkCount = true;
					t = 0.0f;
					isBlink = false;
				}
			}
		}
		else if (!isBlink) {
			blinkPlayer.pos.x = player.pos.x;
			blinkPlayer.pos.y = player.pos.y;
		}

		if (isBlinkCount) {
			blinkCount = 1;
			blinkCT--;
		}
		if (blinkCT <= 0) {
			blinkCount = 0;
			blinkCT = 60;
			isBlinkCount = false;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		Novice::ScreenPrintf(0, 10, "isBlink:%d", isBlink);
		Novice::ScreenPrintf(0, 30, "t:%f", t);
		Novice::ScreenPrintf(0, 50, "player.pos.x:%f", player.pos.x);
		Novice::ScreenPrintf(0, 70, "bPlayer.pos.x:%f", blinkPlayer.pos.x);
		// player
		Novice::DrawEllipse(player.pos.x, player.pos.y, player.radius, player.radius, 0.0f, player.color, kFillModeSolid);
		// 矩形
		Novice::DrawBox(ball.pos.x - ball.radius * 5, ball.pos.y - ball.radius * 2, ball.radius * 10, ball.radius * 4, 0.0f, ball.color, kFillModeSolid);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
