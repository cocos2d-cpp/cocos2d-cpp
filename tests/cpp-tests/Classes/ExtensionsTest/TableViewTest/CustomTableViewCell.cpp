#include "CustomTableViewCell.h"

using namespace cocos2d;

void CustomTableViewCell::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	TableViewCell::draw(renderer, transform, flags);
}
