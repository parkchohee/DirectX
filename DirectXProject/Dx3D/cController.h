#pragma once
class cController : public cObject
{
public:
	cController();
	virtual ~cController();

	virtual void Setup() PURE;
	virtual void Update(D3DXVECTOR3& camAngle, OUT D3DXVECTOR3& vDirection, OUT D3DXVECTOR3& vPosition) PURE;
};

