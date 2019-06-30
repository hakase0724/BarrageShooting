#pragma once
#include "Collider2D.h"
#include "DXGameObject.h"
#include "MeshAndShader.h"
#include <DirectXMath.h>
#include "CommonValues.h"

namespace MyDirectX
{
	class SquareCollider2D :public Collider2D
	{
	public:
		SquareCollider2D() {};
		~SquareCollider2D() {};
		//自身の頂点4つを公開する
		Point2D* GetRightTop() const { return mRightTop; }
		Point2D* GetLeftTop() const { return mLeftTop; }
		Point2D* GetRightBottom() const { return mRightBottom; }
		Point2D* GetLeftBottom() const { return mLeftBottom; }
		//自身の衝突領域を公開する
		float GetLeftX() const { return mPos->x - mOneSideHarf; }
		float GetRightX() const { return mPos->x + mOneSideHarf; }
		float GetTopY() const { return mPos->y + mOneSideHarf; }
		float GetBottomY() const { return mPos->y - mOneSideHarf; }
		virtual void Initialize(DXGameObject* gameObject) override;
		virtual void Initialize() override;
		virtual void LateUpdate() override;
		virtual void Exit() override;
		//衝突判定
		virtual bool IsCollision(Collider2D* otherCollider) override;
		void SetOneSide(float length) { mOneSide = length; }
		float GetOneSide() const { return mOneSide; }
		//衝突応答
		virtual void OnCollision() override;
	private:
		float mOneSide = 0.0f;
		float mOneSideHarf = 0.0f;
		Point2D* mPos = nullptr;
		Point2D* mRightTop = nullptr;
		Point2D* mLeftTop = nullptr;
		Point2D* mRightBottom = nullptr;
		Point2D* mLeftBottom = nullptr;
		void CalcPos();
		bool IsSquareCollision(SquareCollider2D* otherSquareCollider);
	};
}


