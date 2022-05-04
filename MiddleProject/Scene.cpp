#include "stdafx.h"
#include "Scene.h"
#include "GraphicsPipeline.h"

CScene::CScene(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

CScene::~CScene()
{
}

void CScene::BuildObjects()
{
	CExplosiveObject::PrepareExplosion();

	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 200.0f;
	CWallMesh* pWallCubeMesh = new CWallMesh(fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);

	m_pWallsObject = new CWallsObject();


	m_nObjects = 15 + 18 + 15 + 15;
	m_ppObjects = new CGameObject * [m_nObjects];

	RailMaker("Straight", EndLocation, { EndLocation.x, EndLocation.y, EndLocation.z + 15.0f });
	RailMaker("TurnRight", EndLocation, {});
	RailMaker("UpStraight", EndLocation, { EndLocation.x + 15.0f, EndLocation.y, EndLocation.z });
	RailMaker("ConvexityUpStraight", EndLocation, { EndLocation.x + 15.0f, EndLocation.y, EndLocation.z });
	// RailMaker("TurnRight", EndLocation, {});

#ifdef _WITH_DRAW_AXIS
	m_pWorldAxis = new CGameObject();
	CAxisMesh* pAxisMesh = new CAxisMesh(0.5f, 0.5f, 0.5f);
	m_pWorldAxis->SetMesh(pAxisMesh);
#endif
}

void CScene::ReleaseObjects()
{
	if (CExplosiveObject::m_pExplosionMesh) CExplosiveObject::m_pExplosionMesh->Release();

	for (int i = 0; i < m_nObjects; i++) if (m_ppObjects[i]) delete m_ppObjects[i];
	if (m_ppObjects) delete[] m_ppObjects;

	if (m_pWallsObject) delete m_pWallsObject;

#ifdef _WITH_DRAW_AXIS
	if (m_pWorldAxis) delete m_pWorldAxis;
#endif
}

// 레일 생성기
void CScene::RailMaker(char* ModelName, XMFLOAT3 StartPosition, XMFLOAT3 EndPosition)
{
	if (ModelName == "TurnRight") {
		for (int i = rail_start_index; i < rail_start_index + 18; ++i) {
			m_ppObjects[0 + i] = new CExplosiveObject();		// 기본 생성자
			m_ppObjects[0 + i]->SetMesh(new CRailMesh());		// 박스의 크기 설정
			m_ppObjects[0 + i]->SetColor(RGB(0, 255, 0));		// 색상 결정

			XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
			m_ppObjects[0 + i]->Move(StartPosition.x, StartPosition.y, StartPosition.z);

			m_ppObjects[0 + i]->Move(12.0f, 0.0f, 0.0f);	// 반지름이 12인 원으로 회전
			m_ppObjects[0 + i]->Rotate(fy, 5.0f * (i - rail_start_index) + degree);
			m_ppObjects[0 + i]->Move(-12.0f, 0.0f, 0);
		}

		EndLocation = m_ppObjects[rail_start_index + 17]->GetPosition();
		// EndLocation.x += 1;	// 다음 칸에서 시작할 수 있다.
		 
		rail_start_index += 18;
	}
	else if (ModelName == "TurnLeft") {
		for (int i = rail_start_index; i < rail_start_index + 18; ++i) {
			m_ppObjects[0 + i] = new CExplosiveObject();		// 기본 생성자
			m_ppObjects[0 + i]->SetMesh(new CRailMesh());		// 박스의 크기 설정
			m_ppObjects[0 + i]->SetColor(RGB(0, 255, 0));		// 색상 결정

			XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
			m_ppObjects[0 + i]->Move(StartPosition.x, StartPosition.y, StartPosition.z);

			m_ppObjects[0 + i]->Move(-12.0f, 0.0f, 0.0f);	// 반지름이 12인 원으로 회전
			m_ppObjects[0 + i]->Rotate(fy, 5.0f * (i - rail_start_index));
			m_ppObjects[0 + i]->Move(12.0f, 0.0f, 0.0f);
		}

		EndLocation = m_ppObjects[rail_start_index + 17]->GetPosition();
		// EndLocation.z += 1;	// 다음 칸에서 시작한다.
		
		rail_start_index += 18;
	}
	else if (ModelName == "Straight") {
		float fx = 0, fz = 0;
		int length = 0;

		if (StartPosition.x - EndPosition.x != 0) {
			if (StartPosition.x - EndPosition.x < 0)
				length = -(StartPosition.x - EndPosition.x);
			else if (StartPosition.x - EndPosition.x > 0)
				length = (StartPosition.x - EndPosition.x);
			fx = 1;
		}
		else if (StartPosition.z - EndPosition.z != 0) {
			if (StartPosition.z - EndPosition.z < 0)
				length = -(StartPosition.z - EndPosition.z);
			else if (StartPosition.z - EndPosition.z > 0)
				length = (StartPosition.z - EndPosition.z);
			fz = 1;
		}

		if(fz == 1)
			for (int i = rail_start_index; i < rail_start_index + length; ++i) {
				m_ppObjects[0 + i] = new CExplosiveObject();		// 기본 생성자
				m_ppObjects[0 + i]->SetMesh(new CRailMesh(1.0f, 1.0f, 1.0f));		// 박스의 크기 설정
				m_ppObjects[0 + i]->SetColor(RGB(0, 255, 0));		// 색상 결정

				XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
				m_ppObjects[0 + i]->Move(StartPosition.x + fx * (i - rail_start_index), StartPosition.y, StartPosition.z + fz * (i - rail_start_index));
			}
		else if(fx == 1) {
			for (int i = rail_start_index; i < rail_start_index + length; ++i) {
				m_ppObjects[0 + i] = new CExplosiveObject();					// 기본 생성자
				m_ppObjects[0 + i]->SetMesh(new CRailMesh(1.0f, 1.0f, 1.0f));	// 박스의 크기 설정
				m_ppObjects[0 + i]->SetColor(RGB(0, 255, 0));					// 색상 결정

				XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
				m_ppObjects[0 + i]->Move(StartPosition.x + fx * (i - rail_start_index), StartPosition.y, StartPosition.z + fz * (i - rail_start_index));
				m_ppObjects[0 + i]->Rotate(fy, 90.0f);
			}
		}

		EndLocation = m_ppObjects[rail_start_index + length - 1]->GetPosition();
		rail_start_index += length;
	}
	else if (ModelName == "UpStraight") {
		float fx = 0, fz = 0;
		int length = 0;

		if (StartPosition.x - EndPosition.x != 0) {
			if (StartPosition.x - EndPosition.x < 0)
				length = -(StartPosition.x - EndPosition.x);
			else if (StartPosition.x - EndPosition.x > 0)
				length = (StartPosition.x - EndPosition.x);
			fx = 1;
		}
		else if (StartPosition.z - EndPosition.z != 0) {
			if (StartPosition.z - EndPosition.z < 0)
				length = -(StartPosition.z - EndPosition.z);
			else if (StartPosition.z - EndPosition.z > 0)
				length = (StartPosition.z - EndPosition.z);
			fz = 1;
		}

		if (fz == 1)
			for (int i = rail_start_index; i < rail_start_index + length; ++i) {
				m_ppObjects[0 + i] = new CExplosiveObject();					// 기본 생성자
				m_ppObjects[0 + i]->SetMesh(new CRailMesh(1.0f, 1.0f, 1.0f));	// 박스의 크기 설정
				m_ppObjects[0 + i]->SetColor(RGB(0, 255, 0));					// 색상 결정

				XMFLOAT3 zdegree = { -1.0f, 0.0f, 0.0f };

				m_ppObjects[0 + i]->Move(StartPosition.x, StartPosition.y + (i - rail_start_index) / length, StartPosition.z);

				m_ppObjects[0 + i]->Move(0.0f, 12.0f, 0.0f);	// 반지름이 12인 원으로 회전
				m_ppObjects[0 + i]->Rotate(zdegree, 5.0f * (i - rail_start_index));
				m_ppObjects[0 + i]->Move(0.0f, -12.0f, 0.0f);
			}
		else if (fx == 1) {
			for (int i = rail_start_index; i < rail_start_index + length; ++i) {
				m_ppObjects[0 + i] = new CExplosiveObject();					// 기본 생성자
				m_ppObjects[0 + i]->SetMesh(new CRailMesh(1.0f, 1.0f, 1.0f));	// 박스의 크기 설정
				m_ppObjects[0 + i]->SetColor(RGB(0, 255, 0));					// 색상 결정

				XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
				XMFLOAT3 zdegree = { -1.0f, 0.0f, 0.0f };
				
				m_ppObjects[0 + i]->Move(StartPosition.x, StartPosition.y + (i - rail_start_index) / length, StartPosition.z + fz * (i - rail_start_index));
				
				m_ppObjects[0 + i]->Rotate(fy, 90.0f);

				m_ppObjects[0 + i]->Move(0.0f, 12.0f, 0.0f);	// 반지름이 12인 원으로 회전
				m_ppObjects[0 + i]->Rotate(zdegree, 5.0f * (i - rail_start_index));
				m_ppObjects[0 + i]->Move(0.0f, -12.0f, 0.0f);
			}
		}

		EndLocation = m_ppObjects[rail_start_index + length - 1]->GetPosition();
		rail_start_index += length;
	}
	else if (ModelName == "ConvexityUpStraight") {
	float fx = 0, fz = 0;
	int length = 0;

	if (StartPosition.x - EndPosition.x != 0) {
		if (StartPosition.x - EndPosition.x < 0)
			length = EndPosition.x - StartPosition.x + 1;
		else if (StartPosition.x - EndPosition.x > 0)
			length = (StartPosition.x - EndPosition.x);
		fx = 1;
	}
	else if (StartPosition.z - EndPosition.z != 0) {
		if (StartPosition.z - EndPosition.z < 0)
			length = -(StartPosition.z - EndPosition.z);
		else if (StartPosition.z - EndPosition.z > 0)
			length = (StartPosition.z - EndPosition.z);
		fz = 1;
	}
	// length = 15;
	if (fz == 1) {
		for (int i = rail_start_index; i < rail_start_index + length; ++i) {
			m_ppObjects[0 + i] = new CExplosiveObject();					// 기본 생성자
			m_ppObjects[0 + i]->SetMesh(new CRailMesh(1.0f, 1.0f, 1.0f));	// 박스의 크기 설정
			m_ppObjects[0 + i]->SetColor(RGB(0, 255, 0));					// 색상 결정

			XMFLOAT3 zdegree = { -1.0f, 0.0f, 0.0f };

			m_ppObjects[0 + i]->Move(StartPosition.x , StartPosition.y + (i - rail_start_index) / length + 8.9f, StartPosition.z + 11.6f);

			m_ppObjects[0 + i]->Move(0.0f, -12.0f, 0.0f);	// 반지름이 12인 원으로 회전
			m_ppObjects[0 + i]->Rotate(zdegree, 5.0f * (i - rail_start_index));
			m_ppObjects[0 + i]->Move(0.0f, 12.0f, 0.0f);
		}
	}
	else if (fx == 1) {
		for (int i = rail_start_index; i < rail_start_index + length; ++i) {
			m_ppObjects[0 + i] = new CExplosiveObject();					// 기본 생성자
			m_ppObjects[0 + i]->SetMesh(new CRailMesh(1.0f, 1.0f, 1.0f));	// 박스의 크기 설정
			m_ppObjects[0 + i]->SetColor(RGB(0, 255, 0));					// 색상 결정

			XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
			XMFLOAT3 zdegree = { -1.0f, 0.0f, 0.0f };

			m_ppObjects[0 + i]->Move(StartPosition.x + 11.6f, StartPosition.y + (i - rail_start_index) / length + 8.9f, StartPosition.z + fz * (i - rail_start_index));

			m_ppObjects[0 + i]->Rotate(fy, 90.0f);

			m_ppObjects[0 + i]->Move(0.0f, -12.0f, 0.0f);	// 반지름이 12인 원으로 회전
			m_ppObjects[0 + i]->Rotate(zdegree, 5.0f * (i - rail_start_index));
			m_ppObjects[0 + i]->Move(0.0f, 12.0f, 0.0f);
		}
	}

	EndLocation = m_ppObjects[rail_start_index + length - 1]->GetPosition();
	rail_start_index += length;
	}
	// 수정 필요 -> 상수값 너무 많음
	else if (ModelName == "DownStraight") {
		float fx = 0, fz = 0;
		int length = 0;

		if (StartPosition.x - EndPosition.x != 0) {
			if (StartPosition.x - EndPosition.x < 0)
				length = -(StartPosition.x - EndPosition.x);
			else if (StartPosition.x - EndPosition.x > 0)
				length = (StartPosition.x - EndPosition.x);
			fx = 1;
		}
		else if (StartPosition.z - EndPosition.z != 0) {
			if (StartPosition.z - EndPosition.z < 0)
				length = -(StartPosition.z - EndPosition.z);
			else if (StartPosition.z - EndPosition.z > 0)
				length = (StartPosition.z - EndPosition.z);
			fz = 1;
		}

		if (fz == 1)
			for (int i = rail_start_index; i < rail_start_index + length; ++i) {
				m_ppObjects[0 + i] = new CExplosiveObject();		// 기본 생성자
				m_ppObjects[0 + i]->SetMesh(new CRailMesh(1.0f, 1.0f, 1.0f));		// 박스의 크기 설정
				m_ppObjects[0 + i]->SetColor(RGB(0, 255, 0));		// 색상 결정

				XMFLOAT3 zdegree = { -1.0f, 0.0f, 0.0f };
				m_ppObjects[0 + i]->Move(StartPosition.x + fx * (i - rail_start_index), StartPosition.y, StartPosition.z);
				m_ppObjects[0 + i]->Rotate(zdegree, 5.0f * (i - rail_start_index));
			}
		else if (fx == 1) {
			for (int i = rail_start_index; i < rail_start_index + length; ++i) {
				m_ppObjects[0 + i] = new CExplosiveObject();					// 기본 생성자
				m_ppObjects[0 + i]->SetMesh(new CRailMesh(1.0f, 1.0f, 1.0f));	// 박스의 크기 설정
				m_ppObjects[0 + i]->SetColor(RGB(0, 255, 0));					// 색상 결정

				XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
				XMFLOAT3 zdegree = { -1.0f, 0.0f, 0.0f };

				m_ppObjects[0 + i]->Move(StartPosition.x - 0.5f, StartPosition.y + (i - rail_start_index) / length + 3.5f, StartPosition.z + fz * (i - rail_start_index));

				m_ppObjects[0 + i]->Rotate(fy, 90.0f);

				m_ppObjects[0 + i]->Move(0.0f, 0.0f, 12.0f);	// 반지름이 12인 원으로 회전
				m_ppObjects[0 + i]->Rotate(zdegree, -5.0f * (i - rail_start_index));
				m_ppObjects[0 + i]->Move(0.0f, -12.0f, 0.0f);
			}
		}

		EndLocation = m_ppObjects[rail_start_index + length - 1]->GetPosition();
		rail_start_index += length;
	}
	else if (ModelName == "ConvexityDownStraight") {
	float fx = 0, fz = 0;
	int length = 0;

	if (StartPosition.x - EndPosition.x != 0) {
		if (StartPosition.x - EndPosition.x < 0)
			length = -(StartPosition.x - EndPosition.x);
		else if (StartPosition.x - EndPosition.x > 0)
			length = (StartPosition.x - EndPosition.x);
		fx = 1;
	}
	else if (StartPosition.z - EndPosition.z != 0) {
		if (StartPosition.z - EndPosition.z < 0)
			length = -(StartPosition.z - EndPosition.z);
		else if (StartPosition.z - EndPosition.z > 0)
			length = (StartPosition.z - EndPosition.z);
		fz = 1;
	}

	if (fz == 1)
		for (int i = rail_start_index; i < rail_start_index + length; ++i) {
			m_ppObjects[0 + i] = new CExplosiveObject();		// 기본 생성자
			m_ppObjects[0 + i]->SetMesh(new CRailMesh(1.0f, 1.0f, 1.0f));		// 박스의 크기 설정
			m_ppObjects[0 + i]->SetColor(RGB(0, 255, 0));		// 색상 결정

			XMFLOAT3 zdegree = { -1.0f, 0.0f, 0.0f };
			m_ppObjects[0 + i]->Move(StartPosition.x + fx * (i - rail_start_index), StartPosition.y, StartPosition.z + fz * (i - rail_start_index));
			m_ppObjects[0 + i]->Rotate(zdegree, 5.0f * (i - rail_start_index));
		}
	else if (fx == 1) {
		for (int i = rail_start_index; i < rail_start_index + length; ++i) {
			m_ppObjects[0 + i] = new CExplosiveObject();					// 기본 생성자
			m_ppObjects[0 + i]->SetMesh(new CRailMesh(1.0f, 1.0f, 1.0f));	// 박스의 크기 설정
			m_ppObjects[0 + i]->SetColor(RGB(0, 255, 0));					// 색상 결정

			XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
			XMFLOAT3 zdegree = { -1.0f, 0.0f, 0.0f };

			m_ppObjects[0 + i]->Move(StartPosition.x, StartPosition.y - (i - rail_start_index) / length, StartPosition.z + fz * (i - rail_start_index));

			m_ppObjects[0 + i]->Rotate(fy, 90.0f);

			m_ppObjects[0 + i]->Move(0.0f, -10.0f, 0.0f);	// 반지름이 12인 원으로 회전
			m_ppObjects[0 + i]->Rotate(zdegree, -5.0f * (i - rail_start_index));
			m_ppObjects[0 + i]->Move(0.0f, 10.0f, 0.0f);
		}
	}

	EndLocation = m_ppObjects[rail_start_index + length - 1]->GetPosition();
	rail_start_index += length;
	}
}

void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	// 사각형 객체를 터뜨리는 역할을 하므로 필요가 없다.
	return;

	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[int(wParam - '1')];
			pExplosiveObject->m_bBlowingUp = true;
			break;
		}
		case 'A':
			for (int i = 0; i < m_nObjects; i++)
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[i];
				pExplosiveObject->m_bBlowingUp = true;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

CGameObject* CScene::PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera)
{
	

	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / (float)pCamera->m_Viewport.m_nWidth) - 1) / pCamera->m_xmf4x4PerspectiveProject._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / (float)pCamera->m_Viewport.m_nHeight) - 1) / pCamera->m_xmf4x4PerspectiveProject._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmmtxView = XMLoadFloat4x4(&pCamera->m_xmf4x4View);

	int nIntersected = 0;
	float fNearestHitDistance = FLT_MAX;
	CGameObject* pNearestObject = NULL;
	for (int i = 0; i < m_nObjects; i++)
	{
		float fHitDistance = FLT_MAX;
		nIntersected = m_ppObjects[i]->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);
		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
		{
			fNearestHitDistance = fHitDistance;
			pNearestObject = m_ppObjects[i];
		}
	}
	return(pNearestObject);
}

void CScene::CheckObjectByObjectCollisions()
{
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->m_pObjectCollided = NULL;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = (i + 1); j < m_nObjects; j++)
		{
			if (m_ppObjects[i]->m_xmOOBB.Intersects(m_ppObjects[j]->m_xmOOBB))
			{
				m_ppObjects[i]->m_pObjectCollided = m_ppObjects[j];
				m_ppObjects[j]->m_pObjectCollided = m_ppObjects[i];
			}
		}
	}
	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i]->m_pObjectCollided)
		{
			XMFLOAT3 xmf3MovingDirection = m_ppObjects[i]->m_xmf3MovingDirection;
			float fMovingSpeed = m_ppObjects[i]->m_fMovingSpeed;
			m_ppObjects[i]->m_xmf3MovingDirection = m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection;
			m_ppObjects[i]->m_fMovingSpeed = m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
			m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed = fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_pObjectCollided = NULL;
			m_ppObjects[i]->m_pObjectCollided = NULL;
		}
	}
}

void CScene::CheckObjectByWallCollisions()
{
	for (int i = 0; i < m_nObjects; i++)
	{
		ContainmentType containType = m_pWallsObject->m_xmOOBB.Contains(m_ppObjects[i]->m_xmOOBB);
		switch (containType)
		{
		case DISJOINT:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == BACK)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
		case INTERSECTS:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == INTERSECTING)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
		case CONTAINS:
			break;
		}
	}
}

void CScene::CheckPlayerByWallCollision()
{
	BoundingOrientedBox xmOOBBPlayerMoveCheck;
	m_pWallsObject->m_xmOOBBPlayerMoveCheck.Transform(xmOOBBPlayerMoveCheck, XMLoadFloat4x4(&m_pWallsObject->m_xmf4x4World));
	XMStoreFloat4(&xmOOBBPlayerMoveCheck.Orientation, XMQuaternionNormalize(XMLoadFloat4(&xmOOBBPlayerMoveCheck.Orientation)));

	if (!xmOOBBPlayerMoveCheck.Intersects(m_pPlayer->m_xmOOBB)) m_pWallsObject->SetPosition(m_pPlayer->m_xmf3Position);
}

void CScene::CheckObjectByBulletCollisions()
{
	CBulletObject** ppBullets = ((CAirplanePlayer*)m_pPlayer)->m_ppBullets;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBullets[j]->m_bActive && m_ppObjects[i]->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[i];
				pExplosiveObject->m_bBlowingUp = true;
				ppBullets[j]->Reset();
			}
		}
	}
}

// 애니메이션 함수
void CScene::Animate(float fElapsedTime)
{
	return;

	m_pWallsObject->Animate(fElapsedTime);

	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Animate(fElapsedTime);

	CheckPlayerByWallCollision();

	CheckObjectByWallCollisions();

	CheckObjectByObjectCollisions();

	CheckObjectByBulletCollisions();
}

void CScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);

	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);
	m_pWallsObject->Render(hDCFrameBuffer, pCamera);
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);

	if (m_pPlayer) m_pPlayer->Render(hDCFrameBuffer, pCamera);

//UI
#ifdef _WITH_DRAW_AXIS
	CGraphicsPipeline::SetViewOrthographicProjectTransform(&pCamera->m_xmf4x4ViewOrthographicProject);
	m_pWorldAxis->SetRotationTransform(&m_pPlayer->m_xmf4x4World);
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif
}
