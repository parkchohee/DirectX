#pragma once
class cCamera
{
protected:
	D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vLookAt;
	D3DXVECTOR3		m_vUp;
	D3DXVECTOR3*	m_pvTarget;

	POINT			m_ptPrevMouse;
	bool			m_isLButtonDown;
	float			m_fCameraDistance;

	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vCamRotAngle, CamRotAngle);

public:
	cCamera(void);
	virtual ~cCamera(void);

	virtual void Setup(D3DXVECTOR3* pvTarget = NULL);
	virtual void Update();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

