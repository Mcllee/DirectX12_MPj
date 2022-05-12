#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Player.h"

#include <vector>

class CScene
{
public:
	CScene(CPlayer *pPlayer);
	virtual ~CScene();

private:
	int							m_nObjects = 0;
	CGameObject					**m_ppObjects = NULL;

	CWallsObject*				m_pWallsObject = NULL;

	CPlayer*					m_pPlayer = NULL;

#ifdef _WITH_DRAW_AXIS
	CGameObject*				m_pWorldAxis = NULL;
#endif

public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	int rail_start_index = 0;
	float degree = 0.0f;
	XMFLOAT3 EndLocation = { 0.0f, 0.0f, 0.0f };
	void RailMaker(char* ModelName, XMFLOAT3 StartPosition, XMFLOAT3 EndPosition, int time);

	static int rail_shape;
	static int print_time;
	XMFLOAT3 vPosition;
	XMFLOAT3 at;
	XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };

	static int camera_number;
	
	float X_Radian;
	float Y_Radian;
	float Z_Radian;

	XMFLOAT4X4 rotation_X;
	XMFLOAT4X4 rotation_Y;
	XMFLOAT4X4 rotation_Z;

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
};