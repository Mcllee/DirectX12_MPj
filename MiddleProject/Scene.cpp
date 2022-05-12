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

	m_nObjects += 266;				// 레일 수

	m_nObjects += 10 * 4 + 18 * 4;	// 레일 수

	m_nObjects += 192;				// 레일 수
	
	m_nObjects += 3;				// 카트 2개 + 플랫폼 홈

	m_ppObjects = new CGameObject * [m_nObjects];	// 오브젝트 메모리 확보

	//==========================	1		번		트		랙		==============================
	
	RailMaker("Straight", EndLocation, { EndLocation.x, EndLocation.y, EndLocation.z + 15.0f }, 1);
	RailMaker("TurnRight", EndLocation, {}, 1);
	RailMaker("UpWard", EndLocation, {}, 1);
	RailMaker("TurnRight", EndLocation, {}, 1);
	RailMaker("DownWard", EndLocation, {}, 1);
	RailMaker("TurnLeft", EndLocation, {}, 3);
	RailMaker("Straight", EndLocation, { EndLocation.x - 24.0f, EndLocation.y, EndLocation.z }, 1);
	RailMaker("TurnLeft", EndLocation, {}, 1);
	RailMaker("TurnRight", EndLocation, {}, 2);
	RailMaker("Straight", EndLocation, { EndLocation.x, EndLocation.y, EndLocation.z + 11.0f }, 1);

	//==========================	2		번		트		랙		==============================
	
	EndLocation = { 0.0f, 0.0f, 0.0f };
	RailMaker("Straight", EndLocation, { EndLocation.x, EndLocation.y, EndLocation.z + 10.0f }, 1);
	RailMaker("TurnRight", EndLocation, {}, 1);

	RailMaker("Straight", EndLocation, { EndLocation.x + 10.0f, EndLocation.y, EndLocation.z }, 1);
	RailMaker("TurnRight", EndLocation, {}, 1);

	RailMaker("Straight", EndLocation, { EndLocation.x, EndLocation.y, EndLocation.z - 10.0f }, 1);
	RailMaker("TurnRight", EndLocation, {}, 1);

	RailMaker("Straight", EndLocation, { EndLocation.x - 10.0f, EndLocation.y, EndLocation.z }, 1);
	RailMaker("TurnRight", EndLocation, {}, 1);

	//==========================	3		번		트		랙		==============================
	
	EndLocation = { 0.0f, 0.0f, 0.0f };
	RailMaker("Straight", EndLocation, { EndLocation.x, EndLocation.y, EndLocation.z + 20.0f }, 1);
	RailMaker("TurnRight", EndLocation, {}, 1);

	RailMaker("UpWard", EndLocation, {}, 1);
	RailMaker("TurnRight", EndLocation, {}, 1);

	RailMaker("DownWard", EndLocation, {}, 1);
	RailMaker("TurnRight", EndLocation, {}, 1);

	RailMaker("Straight", EndLocation, { EndLocation.x - 24.0f, EndLocation.y, EndLocation.z }, 1);
	RailMaker("TurnRight", EndLocation, {}, 1);

	RailMaker("Straight", EndLocation, { EndLocation.x, EndLocation.y, EndLocation.z + 4.0f }, 1);

	//==============================================================================================


	//===========================		카		트		모		양	================================
	for (int i = 1; i <= 2; ++i) {
		m_ppObjects[m_nObjects - i] = new CExplosiveObject();					// 기본 생성자
		m_ppObjects[m_nObjects - i]->SetMesh(new CCubeMesh(1.0f, 1.0f, 1.0f));	// 박스의 크기 설정
		m_ppObjects[m_nObjects - i]->SetColor(RGB(255, 0, 0));					// 색상 결정
	}
	//==============================================================================================


	//===========================	플		랫		폼		모		양	============================
	m_ppObjects[m_nObjects - 3] = new CExplosiveObject();						// 기본 생성자
	m_ppObjects[m_nObjects - 3]->SetMesh(new CCubeMesh(4.0f, 2.0f, 10.0f));		// 박스의 크기 설정
	m_ppObjects[m_nObjects - 3]->SetColor(RGB(255, 255, 255));					// 색상 결정
	XMFLOAT3 home_position = { -4.0f, 0.0f, 0.0f };								// 위치 결정
	m_ppObjects[m_nObjects - 3]->SetPosition(home_position);					// 플랫폼 홈 위치 설정
	//==============================================================================================


	//===========================	회		전		각		입		력	============================
	for (int i = 0; i < m_nObjects - 3; ++i) {
		X_Radian = DegreeToRadian(m_ppObjects[i]->x_degree);
		Y_Radian = DegreeToRadian(m_ppObjects[i]->y_degree);
		Z_Radian = DegreeToRadian(m_ppObjects[i]->z_degree);

		rotation_X = {
			1,		0,						0,						0,
			0,		(float)cos(X_Radian),	(float)sin(X_Radian),	0,
			0,		(float)-sin(X_Radian),	(float)cos(X_Radian),	0,
			0,		0,						0,						1
		};

		rotation_Y = {
			(float)cos(Y_Radian),	0,		(float)-sin(Y_Radian),	0,
			0,						1,		0,						0,
			(float)sin(Y_Radian),	0,		(float)cos(Y_Radian),	0,
			0,						0,		0,						1
		};

		rotation_Z = {
			(float)cos(Z_Radian),	(float)sin(Z_Radian),	0,	0,
			(float)-sin(Z_Radian),	(float)cos(Z_Radian),	0,	0,
			0,						0,						1,	0,
			0,						0,						0,	1
		};

		rotation_Y = Matrix4x4::Multiply(rotation_X, rotation_Y);
		rotation_Z = Matrix4x4::Multiply(rotation_Y, rotation_Z);

		m_ppObjects[i]->Rotation = rotation_Z;
	}
	//==============================================================================================

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

// 레일 위치 생성기
void CScene::RailMaker(char* ModelName, XMFLOAT3 StartPosition, XMFLOAT3 EndPosition, int time)
{
	float fx = 0, fz = 0;	// 레일의 회전/이동 방향
	int length = 0;			// 레일의 총 개수(= 길이)

	// 레일 생성의 방향 (카트의 이동방향)
	if (StartPosition.x - EndPosition.x != 0) {
		if (StartPosition.x - EndPosition.x < 0) {
			length = int(EndPosition.x - StartPosition.x);
			fx = -1;
		}
		else if (StartPosition.x - EndPosition.x > 0) {
			length = int(StartPosition.x - EndPosition.x);
			fx = 1;
		}
	}
	else if (StartPosition.z - EndPosition.z != 0) {
		if (StartPosition.z - EndPosition.z < 0) {
			length = int(EndPosition.z - StartPosition.z);
			fz = 1;
		}
		else if (StartPosition.z - EndPosition.z > 0) {
			length = int(StartPosition.z - EndPosition.z);
			fz = -1;
		}

	}

	// 레일 모양 생성
	if (ModelName == "TurnRight") {
		XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
		for (int i = rail_start_index; i < rail_start_index + 18; ++i) {
			m_ppObjects[i] = new CExplosiveObject();		// 기본 생성자
			m_ppObjects[i]->SetMesh(new CRailMesh());		// 박스의 크기 설정
			m_ppObjects[i]->SetColor(RGB(0, 255, 0));		// 색상 결정

			m_ppObjects[i]->Move(StartPosition.x, StartPosition.y, StartPosition.z);

			m_ppObjects[i]->Rotate(fy, degree);

			m_ppObjects[i]->Move(12.0f, 0.0f, 0.0f);	// 반지름이 12인 원으로 회전
			m_ppObjects[i]->Rotate(fy, 5.0f * (i - rail_start_index));
			m_ppObjects[i]->Move(-12.0f, 0.0f, 0.0f);

			m_ppObjects[i]->y_degree = 5.0f * (i - rail_start_index) + degree;
		}
		degree += 90.0f;

		EndLocation = m_ppObjects[rail_start_index + 17]->GetPosition();
		rail_start_index += 18;
		
		--time;
		if (time)
			RailMaker("TurnRight", EndLocation, {}, time);	// 반복된 회전일 경우
	}
	else if (ModelName == "TurnLeft") {
		XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
		for (int i = rail_start_index; i < rail_start_index + 18; ++i) {
			m_ppObjects[i] = new CExplosiveObject();		// 기본 생성자
			m_ppObjects[i]->SetMesh(new CRailMesh());		// 박스의 크기 설정
			m_ppObjects[i]->SetColor(RGB(0, 255, 0));		// 색상 결정

			m_ppObjects[i]->Move(StartPosition.x, StartPosition.y, StartPosition.z);

			m_ppObjects[i]->Rotate(fy, degree);

			m_ppObjects[i]->Move(-12.0f, 0.0f, 0.0f);	// 반지름이 12인 원으로 회전
			m_ppObjects[i]->Rotate(fy, -5.0f * (i - rail_start_index));
			m_ppObjects[i]->Move(12.0f, 0.0f, 0.0f);

			m_ppObjects[i]->y_degree = -5.0f * (i - rail_start_index) + degree;
		}
		degree -= 90.0f;

		EndLocation = m_ppObjects[rail_start_index + 17]->GetPosition();
		rail_start_index += 18;

		--time;
		if (time)
			RailMaker("TurnLeft", EndLocation, {}, time);
	}
	else if (ModelName == "Straight") {
		if (fz == 1 || fz == -1) {
			XMFLOAT3 fy;
			for (int i = rail_start_index; i < rail_start_index + length; ++i) {
				m_ppObjects[i] = new CExplosiveObject();		// 기본 생성자
				m_ppObjects[i]->SetMesh(new CRailMesh());		// 박스의 크기 설정
				m_ppObjects[i]->SetColor(RGB(0, 255, 0));		// 색상 결정

				fy = { 0.0f, 1.0f, 0.0f };
				m_ppObjects[i]->Move(StartPosition.x - fx * (i - rail_start_index), StartPosition.y, StartPosition.z + fz * (i - rail_start_index));

				if(fz == -1)
					m_ppObjects[i]->y_degree = 180.0f;
			}
		}
		else if(fx == 1 || fx == -1) {
			for (int i = rail_start_index; i < rail_start_index + length; ++i) {
				m_ppObjects[i] = new CExplosiveObject();					// 기본 생성자
				m_ppObjects[i]->SetMesh(new CRailMesh());	// 박스의 크기 설정
				m_ppObjects[i]->SetColor(RGB(0, 255, 0));					// 색상 결정

				XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
				m_ppObjects[i]->Move(StartPosition.x - fx * (i - rail_start_index), StartPosition.y, StartPosition.z + fz * (i - rail_start_index));
				m_ppObjects[i]->Rotate(fy, 90.0f);
							
				m_ppObjects[i]->y_degree = 90.0f * -fx;
			}
		}

		EndLocation = m_ppObjects[rail_start_index + length - 1]->GetPosition();
		rail_start_index += length;
	}
	else if (ModelName == "UpWard") {
		if (fz == 1 || fz == -1) {
			XMFLOAT3 zdegree;
			for (int i = rail_start_index; i < rail_start_index + 18; ++i) {
				m_ppObjects[i] = new CExplosiveObject();					// 기본 생성자
				m_ppObjects[i]->SetMesh(new CRailMesh());	// 박스의 크기 설정
				m_ppObjects[i]->SetColor(RGB(0, 255, 0));					// 색상 결정

				m_ppObjects[i]->Move(StartPosition.x, StartPosition.y + (i - rail_start_index) / 18, StartPosition.z);

				m_ppObjects[i]->Move(0.0f, 12.0f, 0.0f);	// 반지름이 12인 원으로 회전
				m_ppObjects[i]->Rotate(zdegree, 5.0f * (i - rail_start_index));
				m_ppObjects[i]->Move(0.0f, -12.0f, 0.0f);
				
				m_ppObjects[i]->x_degree = 5.0f * (i - rail_start_index);
			}

			StartPosition = m_ppObjects[rail_start_index + 17]->GetPosition();
			StartPosition.y += 1;

			for (int i = rail_start_index + 18; i < rail_start_index + 36; ++i) {
				m_ppObjects[i] = new CExplosiveObject();					// 기본 생성자
				m_ppObjects[i]->SetMesh(new CRailMesh());	// 박스의 크기 설정
				m_ppObjects[i]->SetColor(RGB(0, 255, 0));					// 색상 결정

				m_ppObjects[i]->Move(StartPosition.x, StartPosition.y + (i - rail_start_index - 18) / 18, StartPosition.z + 11.6f);

				m_ppObjects[i]->Move(0.0f, -12.0f, 0.0f);	// 반지름이 12인 원으로 회전
				m_ppObjects[i]->Rotate(zdegree, 5.0f * (i - rail_start_index - 18));
				m_ppObjects[i]->Move(0.0f, 12.0f, 0.0f);

				m_ppObjects[i]->x_degree = -5.0f * (i - rail_start_index - 18);
			}
		}
		else if (fx == 1 || fx == -1) {
			XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
			XMFLOAT3 zdegree = { -1.0f, 0.0f, 0.0f };

			for (int i = rail_start_index; i < rail_start_index + 18; ++i) {
				m_ppObjects[i] = new CExplosiveObject();					// 기본 생성자
				m_ppObjects[i]->SetMesh(new CRailMesh());	// 박스의 크기 설정
				m_ppObjects[i]->SetColor(RGB(0, 255, 0));					// 색상 결정
				
				m_ppObjects[i]->Move(StartPosition.x, StartPosition.y + (i - rail_start_index) / 18, StartPosition.z + fz * (i - rail_start_index));

				m_ppObjects[i]->Rotate(fy, 90.0f);

				m_ppObjects[i]->Move(0.0f, 12.0f, 0.0f);	// 반지름이 12인 원으로 회전
				m_ppObjects[i]->Rotate(zdegree, 5.0f * (i - rail_start_index));
				m_ppObjects[i]->Move(0.0f, -12.0f, 0.0f);


				m_ppObjects[i]->z_degree = 5.0f * (i - rail_start_index);
				m_ppObjects[i]->y_degree = 90.0f;
			}

			StartPosition = m_ppObjects[rail_start_index + 17]->GetPosition();
			StartPosition.y += 1;

			for (int i = rail_start_index + 18; i < rail_start_index + 36; ++i) {
				m_ppObjects[i] = new CExplosiveObject();					// 기본 생성자
				m_ppObjects[i]->SetMesh(new CRailMesh());	// 박스의 크기 설정
				m_ppObjects[i]->SetColor(RGB(0, 255, 0));					// 색상 결정

				m_ppObjects[i]->Move(StartPosition.x + 12.0f, StartPosition.y + (i - rail_start_index - 18) / 18, StartPosition.z + fz * (i - rail_start_index - 18));

				m_ppObjects[i]->Rotate(fy, 90.0f);

				m_ppObjects[i]->Move(0.0f, 0.0f, 0.0f);	// 반지름이 12인 원으로 회전
				m_ppObjects[i]->Rotate(zdegree, -5.0f * (i - rail_start_index - 18));
				m_ppObjects[i]->Move(0.0f, 0.0f, -12.0f);

				m_ppObjects[i]->Rotate(zdegree, 90.0f);

				m_ppObjects[i]->z_degree = -5.0f * (i - rail_start_index - 18);
				m_ppObjects[i]->y_degree = 90.0f;
			}
		}

		EndLocation = m_ppObjects[rail_start_index + 35]->GetPosition();
		rail_start_index += 36;
	}
	else if (ModelName == "DownWard") {
		if (fx == 1 || fx == -1) {
			XMFLOAT3 zdegree = { -1.0f, 0.0f, 0.0f };
			for (int i = rail_start_index; i < rail_start_index + 18; ++i) {
				m_ppObjects[i] = new CExplosiveObject();		// 기본 생성자
				m_ppObjects[i]->SetMesh(new CRailMesh());		// 박스의 크기 설정
				m_ppObjects[i]->SetColor(RGB(0, 255, 0));		// 색상 결정
							
				m_ppObjects[i]->Move(StartPosition.x, StartPosition.y + (i - rail_start_index) / 18, StartPosition.z + fz * (i - rail_start_index) / 18);
							
				m_ppObjects[i]->Move(0.0f, -12.0f, 0.0f);	// 반지름이 12인 원으로 회전
				m_ppObjects[i]->Rotate(zdegree, 5.0f * (i - rail_start_index));
				m_ppObjects[i]->Move(0.0f, 12.0f, 0.0f);
							
				m_ppObjects[i]->x_degree = +5.0f * (i - rail_start_index);
				m_ppObjects[i]->y_degree = degree;
			}
			StartPosition = m_ppObjects[rail_start_index + 17]->GetPosition();
			StartPosition.y -= 1.0f;

			zdegree = { 1.0f, 0.0f, 0.0f };
			for (int i = rail_start_index + 18; i < rail_start_index + 36; ++i) {
				m_ppObjects[i] = new CExplosiveObject();		// 기본 생성자
				m_ppObjects[i]->SetMesh(new CRailMesh());		// 박스의 크기 설정
				m_ppObjects[i]->SetColor(RGB(0, 255, 0));		// 색상 결정

				m_ppObjects[i]->Move(StartPosition.x, StartPosition.y + (i - rail_start_index - 18) / 18, StartPosition.z + fz * (i - rail_start_index - 18) / 18);

				m_ppObjects[i]->Move(0.0f, 0.0f, -12.0f);	// 반지름이 12인 원으로 회전
				m_ppObjects[i]->Rotate(zdegree, 5.0f * (i - rail_start_index - 18));
				m_ppObjects[i]->Move(0.0f, 0.0f, 12.0f);

				m_ppObjects[i]->Rotate(zdegree, -90.0f);

				m_ppObjects[i]->x_degree = -5.0f * (i - rail_start_index - 18);
				m_ppObjects[i]->y_degree = degree;
			}
		}	
		else if (fz == 1 || fz == -1) {
			for (int i = rail_start_index; i < rail_start_index + 18; ++i) {
				m_ppObjects[i] = new CExplosiveObject();					// 기본 생성자
				m_ppObjects[i]->SetMesh(new CRailMesh());	// 박스의 크기 설정
				m_ppObjects[i]->SetColor(RGB(0, 255, 0));					// 색상 결정

				XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
				XMFLOAT3 zdegree = { -1.0f, 0.0f, 0.0f };

				m_ppObjects[i]->Move(StartPosition.x, StartPosition.y + (i - rail_start_index) / 18, StartPosition.z + fz * (i - rail_start_index));

				m_ppObjects[i]->Rotate(fy, 90.0f);

				m_ppObjects[i]->Move(0.0f, 0.0f, 12.0f);	// 반지름이 12인 원으로 회전
				m_ppObjects[i]->Rotate(zdegree, -5.0f * (i - rail_start_index));
				m_ppObjects[i]->Move(0.0f, -12.0f, 0.0f);

				m_ppObjects[i]->z_degree = 5.0f * (i - rail_start_index);
			}
			StartPosition = m_ppObjects[rail_start_index + 17]->GetPosition();
			StartPosition.y -= 1.0f;
			for (int i = rail_start_index + 18; i < rail_start_index + 36; ++i) {
				m_ppObjects[i] = new CExplosiveObject();					// 기본 생성자
				m_ppObjects[i]->SetMesh(new CRailMesh());	// 박스의 크기 설정
				m_ppObjects[i]->SetColor(RGB(0, 255, 0));					// 색상 결정

				XMFLOAT3 fy = { 0.0f, 1.0f, 0.0f };
				XMFLOAT3 zdegree = { -1.0f, 0.0f, 0.0f };

				m_ppObjects[i]->Move(StartPosition.x, StartPosition.y - (i - rail_start_index - 18) / 18, StartPosition.z + fz * (i - rail_start_index - 18));

				m_ppObjects[i]->Rotate(fy, 90.0f);

				m_ppObjects[i]->Move(0.0f, -12.0f, 0.0f);	// 반지름이 12인 원으로 회전
				m_ppObjects[i]->Rotate(zdegree, -5.0f * (i - rail_start_index - 18));
				m_ppObjects[i]->Move(0.0f, 12.0f, 0.0f);

				m_ppObjects[i]->z_degree = -5.0f * (i - rail_start_index - 18);
			}
		}

		EndLocation = m_ppObjects[rail_start_index + 35]->GetPosition();
		rail_start_index += 36;
	}
}

int CScene::print_time = 0;		// 카트 이동에서 사용
int CScene::rail_shape = 1;		// 레일의 모양 선택
int CScene::camera_number = 1;	// 카메라 위치 선택

// 애니메이션 함수
void CScene::Animate(float fElapsedTime)
{
	print_time += 1;

	if(rail_shape == 1 && print_time == 266)
		print_time = 0;
	else if (rail_shape == 2 && print_time == 377)
		print_time = 266;
	else if (rail_shape == 3 && print_time == 569)
		print_time = 377;

	m_ppObjects[m_nObjects - 1]->SetPosition(	m_ppObjects[print_time]->GetPosition().x,
												m_ppObjects[print_time]->GetPosition().y + 1.0f,
												m_ppObjects[print_time]->GetPosition().z);
	m_ppObjects[m_nObjects - 1]->SetRotationTransform(&m_ppObjects[print_time]->Rotation);

	m_ppObjects[m_nObjects - 2]->SetPosition(	m_ppObjects[print_time + 1]->GetPosition().x,
												m_ppObjects[print_time + 1]->GetPosition().y + 1.0f,
												m_ppObjects[print_time + 1]->GetPosition().z);
	m_ppObjects[m_nObjects - 2]->SetRotationTransform(&m_ppObjects[print_time]->Rotation);
}

void CScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	Y_Radian = DegreeToRadian(m_ppObjects[print_time]->y_degree);
	at = { m_ppObjects[m_nObjects - 1]->GetPosition() };

	if (camera_number == 1) {
		vPosition = {	m_ppObjects[m_nObjects - 1]->GetPosition().x - 12.0f * (float)(sin(Y_Radian)),
						m_ppObjects[m_nObjects - 1]->GetPosition().y + 3.0f,
						m_ppObjects[m_nObjects - 1]->GetPosition().z - 12.0f * (float)(cos(Y_Radian)) };
		at = { m_ppObjects[m_nObjects - 1]->GetPosition() };		
		up = { 0.0f, 1.0f, 0.0f };
	}
	else if (camera_number == 2) {
		vPosition = { -10.0f, 4.0f, 0.0f };
		at = { 1.0f, 4.0f, 0.0f };
		up = { 0.0f, 1.0f, 0.0f };
	}
	else if (camera_number == 3) {
		vPosition = { 45.0f, 30.0f, -50.0f };		// 멀리서 바라보기.
		at = { 35.0f, 0.0f, 0.0f };
		up = { 0.0f, 1.0f, 0.0f };
	}
	else if (camera_number == 4) {
		vPosition = { 30.0f, 60.0f, 8.0f };
		at = { 30.0f, 0.0f, 8.0f };
		up = { 0.0f, 0.0f, 1.0f };
	}

	pCamera->SetLookAt(vPosition, at, up);

	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);
	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);

	// 1번 트랙 출력
	if (rail_shape == 1)
		for (int i = 0; i < 266; ++i) m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);
	// 2번 트랙 출력
	else if(rail_shape == 2)
		for (int i = 266; i < 377; ++i) m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);
	// 3번 트랙 출력
	else if (rail_shape == 3)
		for (int i = 377; i < 570; ++i) m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);

	// 탑승 플랫폼, 1번 열차, 2번 열차를 출력
	for (int i = 570; i < m_nObjects; ++i) m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);

//UI
#ifdef _WITH_DRAW_AXIS
	CGraphicsPipeline::SetViewOrthographicProjectTransform(&pCamera->m_xmf4x4ViewOrthographicProject);
	m_pWorldAxis->SetRotationTransform(&m_pPlayer->m_xmf4x4World);
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif
}
