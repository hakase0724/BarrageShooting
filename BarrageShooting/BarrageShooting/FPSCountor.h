#pragma once
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <list>

using namespace std;

//時刻取得に使用するAPI
#define FPSCOUNTER_QUERYPER_COUNTER        1
#define FPSCOUNTER_TIMEGETTIME             2
namespace MyDirectX
{
	class FPSCountor
	{
	public:
		FPSCountor(unsigned int smp = 10);
		~FPSCountor();
		// FPS値を取得
		double GetFPS();
		// サンプル数を変更
		void SetSampleNum(unsigned int smp);
		// 現在の時刻を取得
		double GetCurDefTime();
		// FPSを更新
		double UpdateFPS(double Def);
	private:
		int m_iCounterFlag = 0;                // 使用する計測関数の判定フラグ
		LARGE_INTEGER m_Counter;           // クロックカウント数
		double m_dFreq = 0.0;                    // 1秒当たりクロックカウント数（周波数）
		LONGLONG m_OldLongCount = 0;           // 以前のクロックカウント数
		DWORD m_dwTGTOldCount = 0;             // 以前の時刻（ミリ秒）
		list<double> m_dwDefTimeLst;       // 時間リスト
		UINT m_uiNum = 0;                      // 移動平均計算時のデータ数
		double m_dwSumTimes = 0.0;               // 共通部分の合計値
	};

}

