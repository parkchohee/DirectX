#pragma once
class cController : public cObject
{
protected:
	float			 m_fMoveSpeed;

public:
	cController();
	virtual ~cController();

	virtual void Setup(float moveSpeed = 0.1f) PURE;
	virtual void Update(OUT D3DXVECTOR3& camAngle, OUT D3DXVECTOR3& vDirection, OUT D3DXVECTOR3& vPosition) PURE;
};

