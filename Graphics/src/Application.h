#ifndef _APPLICATION_H_
#define _APPLICATION_H_

class Application
{
public:
	virtual bool startUp() = 0;
	virtual bool update() = 0;
	virtual void Draw() = 0;
	virtual void Destroy() = 0;
};
#endif
