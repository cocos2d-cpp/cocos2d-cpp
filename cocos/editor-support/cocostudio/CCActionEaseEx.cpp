/****************************************************************************
Copyright (c) 2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCActionEaseEx.h"

using namespace cocos2d;

namespace cocostudio {

static inline float bezieratFunction( float a, float b, float c, float d, float t )
{
	return (powf(1-t,3) * a + 3*t*(powf(1-t,2))*b + 3*powf(t,2)*(1-t)*c + powf(t,3)*d );
}

EaseBezierAction* EaseBezierAction::create(cocos2d::ActionInterval* action)
{
	EaseBezierAction *_ret = new EaseBezierAction();
	if (_ret)
	{  
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

void EaseBezierAction::setBezierParamer( float p0, float p1, float p2, float p3)
{
	_p0 = p0;
	_p1 = p1;
	_p2 = p2;
	_p3 = p3;
}

EaseBezierAction* EaseBezierAction::clone() const
{
	// no copy constructor
	auto a = new EaseBezierAction();
	a->initWithAction(_inner->clone());
	a->setBezierParamer(_p0,_p1,_p2,_p3);
	a->autorelease();
	return a;
}

void EaseBezierAction::update(float time)
{
	_inner->update(bezieratFunction(_p0,_p1,_p2,_p3,time));
}

EaseBezierAction* EaseBezierAction::reverse() const
{
	EaseBezierAction* reverseAction = EaseBezierAction::create(_inner->reverse());
	reverseAction->setBezierParamer(_p3,_p2,_p1,_p0);
	return reverseAction;
}

//
// EaseQuadraticActionIn
//

EaseQuadraticActionIn* EaseQuadraticActionIn::create(ActionInterval* action)
{
	EaseQuadraticActionIn *_ret = new EaseQuadraticActionIn();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseQuadraticActionIn* EaseQuadraticActionIn::clone() const
{
	// no copy constructor
	auto a = new EaseQuadraticActionIn();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}
void EaseQuadraticActionIn::update(float time)
{
	_inner->update(powf(time,2));
}

EaseQuadraticActionIn* EaseQuadraticActionIn::reverse() const
{
	return EaseQuadraticActionIn::create(_inner->reverse());
}

//
// EaseQuadraticActionOut
//

EaseQuadraticActionOut* EaseQuadraticActionOut::create(ActionInterval* action)
{
	EaseQuadraticActionOut *_ret = new EaseQuadraticActionOut();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseQuadraticActionOut* EaseQuadraticActionOut::clone() const
{
	// no copy constructor
	auto a = new EaseQuadraticActionOut();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseQuadraticActionOut::update(float time)
{	
	_inner->update(-time*(time-2));
}

EaseQuadraticActionOut* EaseQuadraticActionOut::reverse() const
{
	return EaseQuadraticActionOut::create(_inner->reverse());
}

//
// EaseQuadraticActionInOut
//

EaseQuadraticActionInOut* EaseQuadraticActionInOut::create(ActionInterval* action)
{
	EaseQuadraticActionInOut *_ret = new EaseQuadraticActionInOut();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseQuadraticActionInOut* EaseQuadraticActionInOut::clone() const
{
	// no copy constructor
	auto a = new EaseQuadraticActionInOut();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseQuadraticActionInOut::update(float time)
{
	float resultTime = time;
	time = time*0.5f;
	if (time < 1)
	{
		resultTime = time * time * 0.5f;
	}
	else
	{
		--time;
		resultTime = -0.5f * (time * (time - 2) - 1);
	}

	_inner->update(resultTime);
}

EaseQuadraticActionInOut* EaseQuadraticActionInOut::reverse() const
{
	return EaseQuadraticActionInOut::create(_inner->reverse());
}

//
// EaseQuarticActionIn
//

EaseQuarticActionIn* EaseQuarticActionIn::create(ActionInterval* action)
{
	EaseQuarticActionIn *_ret = new EaseQuarticActionIn();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseQuarticActionIn* EaseQuarticActionIn::clone() const
{
	// no copy constructor
	auto a = new EaseQuarticActionIn();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseQuarticActionIn::update(float time)
{
	_inner->update(powf(time,4.0f));
}

EaseQuarticActionIn* EaseQuarticActionIn::reverse() const
{
	return EaseQuarticActionIn::create(_inner->reverse());
}

//
// EaseQuarticActionOut
//

EaseQuarticActionOut* EaseQuarticActionOut::create(ActionInterval* action)
{
	EaseQuarticActionOut *_ret = new EaseQuarticActionOut();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseQuarticActionOut* EaseQuarticActionOut::clone() const
{
	// no copy constructor
	auto a = new EaseQuarticActionOut();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseQuarticActionOut::update(float time)
{
	float tempTime = time -1;	
	_inner->update(1- powf(tempTime,4.0f));
}

EaseQuarticActionOut* EaseQuarticActionOut::reverse() const
{
	return EaseQuarticActionOut::create(_inner->reverse());
}

//
// EaseQuarticActionInOut
//

EaseQuarticActionInOut* EaseQuarticActionInOut::create(ActionInterval* action)
{
	EaseQuarticActionInOut *_ret = new EaseQuarticActionInOut();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseQuarticActionInOut* EaseQuarticActionInOut::clone() const
{
	// no copy constructor
	auto a = new EaseQuarticActionInOut();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseQuarticActionInOut::update(float time)
{
	float tempTime = time * 0.5f;
	if (tempTime < 1)
		tempTime =  powf(tempTime,4.0f) * 0.5f;
	else
	{
		tempTime -= 2;
		tempTime = 1 - powf(tempTime,4.0f)* 0.5f;
	}

	_inner->update(tempTime);
}

EaseQuarticActionInOut* EaseQuarticActionInOut::reverse() const
{
	return EaseQuarticActionInOut::create(_inner->reverse());
}

//
// EaseQuinticActionIn
//

EaseQuinticActionIn* EaseQuinticActionIn::create(ActionInterval* action)
{
	EaseQuinticActionIn *_ret = new EaseQuinticActionIn();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseQuinticActionIn* EaseQuinticActionIn::clone() const
{
	// no copy constructor
	auto a = new EaseQuinticActionIn();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseQuinticActionIn::update(float time)
{
	_inner->update(powf(time,5.0f));
}

EaseQuinticActionIn* EaseQuinticActionIn::reverse() const
{
	return EaseQuinticActionIn::create(_inner->reverse());
}

//
// EaseQuinticActionOut
//

EaseQuinticActionOut* EaseQuinticActionOut::create(ActionInterval* action)
{
	EaseQuinticActionOut *_ret = new EaseQuinticActionOut();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseQuinticActionOut* EaseQuinticActionOut::clone() const
{
	// no copy constructor
	auto a = new EaseQuinticActionOut();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseQuinticActionOut::update(float time)
{
	float tempTime = time -1;	
	_inner->update(1 + powf(tempTime,5.0f));
}

EaseQuinticActionOut* EaseQuinticActionOut::reverse() const
{
	return EaseQuinticActionOut::create(_inner->reverse());
}

//
// EaseQuinticActionInOut
//

EaseQuinticActionInOut* EaseQuinticActionInOut::create(ActionInterval* action)
{
	EaseQuinticActionInOut *_ret = new EaseQuinticActionInOut();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseQuinticActionInOut* EaseQuinticActionInOut::clone() const
{
	// no copy constructor
	auto a = new EaseQuinticActionInOut();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseQuinticActionInOut::update(float time)
{
	float tempTime = time * 0.5f;
	if (tempTime < 1)
		tempTime =  powf(tempTime,5.0f) * 0.5f;
	else
	{
		tempTime -= 2;
		tempTime = 1 + powf(tempTime,5.0f)* 0.5f;
	}

	_inner->update(tempTime);
}

EaseQuinticActionInOut* EaseQuinticActionInOut::reverse() const
{
	return EaseQuinticActionInOut::create(_inner->reverse());
}

//
// EaseCircleActionIn
//

EaseCircleActionIn* EaseCircleActionIn::create(ActionInterval* action)
{
	EaseCircleActionIn *_ret = new EaseCircleActionIn();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseCircleActionIn* EaseCircleActionIn::clone() const
{
	// no copy constructor
	auto a = new EaseCircleActionIn();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseCircleActionIn::update(float time)
{
	_inner->update(1-sqrt(1-powf(time,2.0f)));
}

EaseCircleActionIn* EaseCircleActionIn::reverse() const
{
	return EaseCircleActionIn::create(_inner->reverse());
}

//
// EaseCircleActionOut
//

EaseCircleActionOut* EaseCircleActionOut::create(ActionInterval* action)
{
	EaseCircleActionOut *_ret = new EaseCircleActionOut();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseCircleActionOut* EaseCircleActionOut::clone() const
{
	// no copy constructor
	auto a = new EaseCircleActionOut();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseCircleActionOut::update(float time)
{
	float tempTime = time - 1;
	_inner->update(sqrt(1-powf(tempTime,2.0f)));
}

EaseCircleActionOut* EaseCircleActionOut::reverse() const
{
	return EaseCircleActionOut::create(_inner->reverse());
}

//
// EaseCircleActionInOut
//

EaseCircleActionInOut* EaseCircleActionInOut::create(ActionInterval* action)
{
	EaseCircleActionInOut *_ret = new EaseCircleActionInOut();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseCircleActionInOut* EaseCircleActionInOut::clone() const
{
	// no copy constructor
	auto a = new EaseCircleActionInOut();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseCircleActionInOut::update(float time)
{
	float tempTime = time * 0.5f;
	if (tempTime < 1)
		tempTime =  (1- sqrt(1 - powf(tempTime,2.0f))) * 0.5f;
	else
	{
		tempTime -= 2;
		tempTime = (1+ sqrt(1 - powf(tempTime,2.0f)));
	}

	_inner->update(time);
}

EaseCircleActionInOut* EaseCircleActionInOut::reverse() const
{
	return EaseCircleActionInOut::create(_inner->reverse());
}

//
// EaseCubicActionIn
//

EaseCubicActionIn* EaseCubicActionIn::create(ActionInterval* action)
{
	EaseCubicActionIn *_ret = new EaseCubicActionIn();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseCubicActionIn* EaseCubicActionIn::clone() const
{
	// no copy constructor
	auto a = new EaseCubicActionIn();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseCubicActionIn::update(float time)
{
	_inner->update(powf(time,3.0f));
}

EaseCubicActionIn* EaseCubicActionIn::reverse() const
{
	return EaseCubicActionIn::create(_inner->reverse());
}

//
// EaseCubicActionOut
//

EaseCubicActionOut* EaseCubicActionOut::create(ActionInterval* action)
{
	EaseCubicActionOut *_ret = new EaseCubicActionOut();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseCubicActionOut* EaseCubicActionOut::clone() const
{
	// no copy constructor
	auto a = new EaseCubicActionOut();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseCubicActionOut::update(float time)
{
	_inner->update(1+powf(time,3.0f));
}

EaseCubicActionOut* EaseCubicActionOut::reverse() const
{
	return EaseCubicActionOut::create(_inner->reverse());
}

//
// EaseCubicActionInOut
//

EaseCubicActionInOut* EaseCubicActionInOut::create(ActionInterval* action)
{
	EaseCubicActionInOut *_ret = new EaseCubicActionInOut();
	if (_ret)
	{
		if (_ret->initWithAction(action))
		{
			_ret->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(_ret);
		}
	}

	return _ret; 
}

EaseCubicActionInOut* EaseCubicActionInOut::clone() const
{
	// no copy constructor
	auto a = new EaseCubicActionInOut();
	a->initWithAction(_inner->clone());
	a->autorelease();
	return a;
}

void EaseCubicActionInOut::update(float time)
{
	float tempTime = time * 0.5f;
	if (tempTime < 1)
		tempTime =  powf(tempTime,3.0f) * 0.5f;
	else
	{
		tempTime -= 2;
		tempTime = 1 + powf(tempTime,3.0f)* 0.5f;
	}
	_inner->update(time);
}

EaseCubicActionInOut* EaseCubicActionInOut::reverse() const
{
	return EaseCubicActionInOut::create(_inner->reverse());
}

}