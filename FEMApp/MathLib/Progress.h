#pragma once
class CProgress
{
public:
	CProgress();
	virtual ~CProgress(void);
	void SetRange( double m ) { max=m; }

	void NewValue( double v ) { value = v; Update(); }
	void Increment( double dv ) { value += dv; Update(); }

protected:

	virtual void Update() = 0;

	double max, value;
	
};

