#pragma once
class iMap
{
public:
	virtual void Render() = 0;
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) = 0;
};

