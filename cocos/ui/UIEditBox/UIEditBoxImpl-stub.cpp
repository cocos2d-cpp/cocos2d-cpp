#include "ui/UIEditBox/UIEditBox.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID ) && (CC_TARGET_PLATFORM != CC_PLATFORM_IOS ) && (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_MAC) && (CC_TARGET_PLATFORM != CC_PLATFORM_TIZEN) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)

namespace cocos2d {

namespace ui {

EditBoxImpl* __createSystemEditBox(EditBox* pEditBox)
{
    return nullptr;
}

}

} // namespace cocos2d

#endif /* #if (..) */
