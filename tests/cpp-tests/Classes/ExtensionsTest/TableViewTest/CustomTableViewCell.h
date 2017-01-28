#ifndef __CUSTOMTABELVIEWCELL_H__
#define __CUSTOMTABELVIEWCELL_H__

#include "GUI/CCScrollView/CCTableView.h"

class CustomTableViewCell : public cocos2d::extension::TableViewCell
{
public:
	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
};

#endif /* __CUSTOMTABELVIEWCELL_H__ */

