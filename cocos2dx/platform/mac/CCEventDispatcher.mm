/*
 * cocos2d for iPhone: http://www.cocos2d-iphone.org
 *
 * Copyright (c) 2010 Ricardo Quesada
 * Copyright (c) 2011 Zynga Inc.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Only compile this code on Mac. These files should not be included on your iOS project.
// But in case they are included, it won't be compiled.
#import <Availability.h>

#import "CCEventDispatcher.h"
#import "ccConfig.h"
#include "support/data_support/utlist.h"

#include "CCDirector.h"
#include "keyboard_dispatcher/CCKeyboardDispatcher.h"

//NS_CC_BEGIN;
static CCEventDispatcher *sharedDispatcher = nil;

enum  {
	// mouse
	kImplementsMouseDown			= 1 << 0,
	kImplementsMouseMoved			= 1 << 1,
	kImplementsMouseDragged		= 1 << 2,	
	kImplementsMouseUp			= 1 << 3,
	kImplementsRightMouseDown		= 1 << 4,
	kImplementsRightMouseDragged	= 1 << 5,
	kImplementsRightMouseUp		= 1 << 6,
	kImplementsOtherMouseDown		= 1 << 7,
	kImplementsOtherMouseDragged	= 1 << 8,
	kImplementsOtherMouseUp		= 1 << 9,
	kImplementsScrollWheel		= 1 << 10,
	kImplementsMouseEntered		= 1 << 11,
	kImplementsMouseExited		= 1 << 12,

	kImplementsTouchesBegan		= 1 << 13,
	kImplementsTouchesMoved		= 1 << 14,
	kImplementsTouchesEnded		= 1 << 15,
	kImplementsTouchesCancelled	        = 1 << 16,

	// keyboard
	kImplementsKeyUp				= 1 << 0,
	kImplementsKeyDown			= 1 << 1,
	kImplementsFlagsChanged		= 1 << 2,
};


typedef struct _listEntry
{
	struct	_listEntry	*prev, *next;
	id					delegate;
	NSInteger			priority;
	NSUInteger			flags;
} tListEntry;


#if CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD

#define QUEUE_EVENT_MAX 128
struct _eventQueue {
	SEL		selector;
	NSEvent	*event;
};

static struct	_eventQueue eventQueue[QUEUE_EVENT_MAX];
static int		eventQueueCount;

#endif // CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD


@implementation CCEventDispatcher

@synthesize dispatchEvents=dispatchEvents_;


+(CCEventDispatcher*) sharedDispatcher
{
	@synchronized(self) {
		if (sharedDispatcher == nil)
			sharedDispatcher = [[self alloc] init]; // assignment not done here
	}
	return sharedDispatcher;
}

+(id) allocWithZone:(NSZone *)zone
{
	@synchronized(self) {
		NSAssert(sharedDispatcher == nil, @"Attempted to allocate a second instance of a singleton.");
		return [super allocWithZone:zone];
	}
	return nil; // on subsequent allocation attempts return nil
}

-(id) init
{
	if( (self = [super init]) )
	{
		// events enabled by default
		dispatchEvents_ = YES;

		// delegates
		keyboardDelegates_ = NULL;
		mouseDelegates_ = NULL;
                touchDelegates_ = NULL;
		
#if	CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD
		eventQueueCount = 0;
#endif
	}
	
	return self;
}

- (void) dealloc
{
	[super dealloc];
}

#pragma mark EventDispatcher - add / remove delegates

-(void) addDelegate:(id)delegate priority:(NSInteger)priority flags:(NSUInteger)flags list:(tListEntry**)list
{
	tListEntry *listElement = (tListEntry *)malloc( sizeof(*listElement) );
	
	listElement->delegate = [delegate retain];
	listElement->priority = priority;
	listElement->flags = flags;
	listElement->next = listElement->prev = NULL;
	
	// empty list ?
	if( ! *list ) {
		DL_APPEND( *list, listElement );
		
	} else {
		BOOL added = NO;		
		
		for( tListEntry *elem = *list; elem ; elem = elem->next ) {
			if( priority < elem->priority ) {
				
				if( elem == *list )
					DL_PREPEND(*list, listElement);
				else {
					listElement->next = elem;
					listElement->prev = elem->prev;
					
					elem->prev->next = listElement;
					elem->prev = listElement;
				}
				
				added = YES;
				break;
			}
		}
		
		// Not added? priority has the higher value. Append it.
		if( !added )
			DL_APPEND(*list, listElement);
	}
}

-(void) removeDelegate:(id)delegate fromList:(tListEntry**)list
{
	tListEntry *entry, *tmp;
	
	// updates with priority < 0
	DL_FOREACH_SAFE( *list, entry, tmp ) {
		if( entry->delegate == delegate ) {
			DL_DELETE( *list, entry );
			[delegate release];
			free(entry);
			break;
		}
	}
}

-(void) removeAllDelegatesFromList:(tListEntry**)list
{
	tListEntry *entry, *tmp;

	DL_FOREACH_SAFE( *list, entry, tmp ) {
		DL_DELETE( *list, entry );
		free(entry);
	}
}


-(void) addMouseDelegate:(id<CCMouseEventDelegate>) delegate priority:(NSInteger)priority
{
	NSUInteger flags = 0;
	
	flags |= ( [delegate respondsToSelector:@selector(ccMouseDown:)] ? kImplementsMouseDown : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccMouseDragged:)] ? kImplementsMouseDragged : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccMouseMoved:)] ? kImplementsMouseMoved : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccMouseUp:)] ? kImplementsMouseUp : 0 );

	flags |= ( [delegate respondsToSelector:@selector(ccRightMouseDown:)] ? kImplementsRightMouseDown : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccRightMouseDragged:)] ? kImplementsRightMouseDragged : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccRightMouseUp:)] ? kImplementsRightMouseUp : 0 );

	flags |= ( [delegate respondsToSelector:@selector(ccOtherMouseDown:)] ? kImplementsOtherMouseDown : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccOtherMouseDragged:)] ? kImplementsOtherMouseDragged : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccOtherMouseUp:)] ? kImplementsOtherMouseUp : 0 );

	flags |= ( [delegate respondsToSelector:@selector(ccMouseEntered:)] ? kImplementsMouseEntered : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccMouseExited:)] ? kImplementsMouseExited : 0 );

	flags |= ( [delegate respondsToSelector:@selector(ccScrollWheel:)] ? kImplementsScrollWheel : 0 );

	[self addDelegate:delegate priority:priority flags:flags list:&mouseDelegates_];
}

-(void) removeMouseDelegate:(id) delegate
{
	[self removeDelegate:delegate fromList:&mouseDelegates_];
}

-(void) removeAllMouseDelegates
{
	[self removeAllDelegatesFromList:&mouseDelegates_];
}

-(void) addKeyboardDelegate:(id<CCKeyboardEventDelegate>) delegate priority:(NSInteger)priority
{
	NSUInteger flags = 0;
	
	flags |= ( [delegate respondsToSelector:@selector(ccKeyUp:)] ? kImplementsKeyUp : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccKeyDown:)] ? kImplementsKeyDown : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccFlagsChanged:)] ? kImplementsFlagsChanged : 0 );
	
	[self addDelegate:delegate priority:priority flags:flags list:&keyboardDelegates_];
}

-(void) removeKeyboardDelegate:(id) delegate
{
	[self removeDelegate:delegate fromList:&keyboardDelegates_];
}

-(void) removeAllKeyboardDelegates
{
	[self removeAllDelegatesFromList:&keyboardDelegates_];
}

-(void) addTouchDelegate:(id<CCTouchEventDelegate>) delegate priority:(NSInteger)priority
{
	NSUInteger flags = 0;
	
	flags |= ( [delegate respondsToSelector:@selector(ccTouchesBeganWithEvent:)] ? kImplementsTouchesBegan : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccTouchesMovedWithEvent:)] ? kImplementsTouchesMoved : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccTouchesEndedWithEvent:)] ? kImplementsTouchesEnded : 0 );
	flags |= ( [delegate respondsToSelector:@selector(ccTouchesCancelledWithEvent:)] ? kImplementsTouchesCancelled : 0 );
	
	[self addDelegate:delegate priority:priority flags:flags list:&touchDelegates_];
}

-(void) removeTouchDelegate:(id) delegate
{
	[self removeDelegate:delegate fromList:&touchDelegates_];
}

-(void) removeAllTouchDelegates
{
	[self removeAllDelegatesFromList:&touchDelegates_];
}


#pragma mark EventDispatcher - Mouse events
//
// Mouse events
//

//
// Left
//
- (void)mouseDown:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;

		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsMouseDown ) {
				void *swallows = [entry->delegate performSelector:@selector(ccMouseDown:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

- (void)mouseMoved:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsMouseMoved ) {
				void *swallows = [entry->delegate performSelector:@selector(ccMouseMoved:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

- (void)mouseDragged:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsMouseDragged ) {
				void *swallows = [entry->delegate performSelector:@selector(ccMouseDragged:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

- (void)mouseUp:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsMouseUp ) {
				void *swallows = [entry->delegate performSelector:@selector(ccMouseUp:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

//
// Mouse Right
//
- (void)rightMouseDown:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsRightMouseDown ) {
				void *swallows = [entry->delegate performSelector:@selector(ccRightMouseDown:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

- (void)rightMouseDragged:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsRightMouseDragged ) {
				void *swallows = [entry->delegate performSelector:@selector(ccRightMouseDragged:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

- (void)rightMouseUp:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsRightMouseUp ) {
				void *swallows = [entry->delegate performSelector:@selector(ccRightMouseUp:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

//
// Mouse Other
//
- (void)otherMouseDown:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsOtherMouseDown ) {
				void *swallows = [entry->delegate performSelector:@selector(ccOtherMouseDown:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

- (void)otherMouseDragged:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsOtherMouseDragged ) {
				void *swallows = [entry->delegate performSelector:@selector(ccOtherMouseDragged:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

- (void)otherMouseUp:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsOtherMouseUp ) {
				void *swallows = [entry->delegate performSelector:@selector(ccOtherMouseUp:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

//
// Scroll Wheel
//
- (void)scrollWheel:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsScrollWheel ) {
				void *swallows = [entry->delegate performSelector:@selector(ccScrollWheel:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

//
// Mouse enter / exit
- (void)mouseExited:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsMouseEntered ) {
				void *swallows = [entry->delegate performSelector:@selector(ccMouseEntered:) withObject:event];
				if( swallows )
					break;
			}
		}
	}	
}

- (void)mouseEntered:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( mouseDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsMouseExited) {
				void *swallows = [entry->delegate performSelector:@selector(ccMouseExited:) withObject:event];
				if( swallows )
					break;
			}
		}
	}	
}


#pragma mark EventDispatcher - Keyboard events

// Keyboard events
- (void)keyDown:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;

      cocos2d::KeyboardDispatcher *kbDisp = cocos2d::Director::sharedDirector()->getKeyboardDispatcher();
      kbDisp->dispatchKeyboardEvent(event.keyCode, true);		
		DL_FOREACH_SAFE( keyboardDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsKeyDown ) {
				void *swallows = [entry->delegate performSelector:@selector(ccKeyDown:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

- (void)keyUp:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
      cocos2d::KeyboardDispatcher *kbDisp = cocos2d::Director::sharedDirector()->getKeyboardDispatcher();
      kbDisp->dispatchKeyboardEvent(event.keyCode, true);
		DL_FOREACH_SAFE( keyboardDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsKeyUp ) {
				void *swallows = [entry->delegate performSelector:@selector(ccKeyUp:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}

- (void)flagsChanged:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( keyboardDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsFlagsChanged ) {
				void *swallows = [entry->delegate performSelector:@selector(ccFlagsChanged:) withObject:event];
				if( swallows )
					break;
			}
		}
	}
}


#pragma mark EventDispatcher - Touch events

- (void)touchesBeganWithEvent:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( touchDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsTouchesBegan) {
				void *swallows = [entry->delegate performSelector:@selector(ccTouchesBeganWithEvent:) withObject:event];
				if( swallows )
					break;
			}
		}
	}	
}

- (void)touchesMovedWithEvent:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( touchDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsTouchesMoved) {
				void *swallows = [entry->delegate performSelector:@selector(ccTouchesMovedWithEvent:) withObject:event];
				if( swallows )
					break;
			}
		}
	}	
}

- (void)touchesEndedWithEvent:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( touchDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsTouchesEnded) {
				void *swallows = [entry->delegate performSelector:@selector(ccTouchesEndedWithEvent:) withObject:event];
				if( swallows )
					break;
			}
		}
	}	
}

- (void)touchesCancelledWithEvent:(NSEvent *)event
{
	if( dispatchEvents_ ) {
		tListEntry *entry, *tmp;
		
		DL_FOREACH_SAFE( touchDelegates_, entry, tmp ) {
			if ( entry->flags & kImplementsTouchesCancelled) {
				void *swallows = [entry->delegate performSelector:@selector(ccTouchesCancelledWithEvent:) withObject:event];
				if( swallows )
					break;
			}
		}
	}	
}


#pragma mark EventDispatcher - queue events

#if CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD
-(void) queueEvent:(NSEvent*)event selector:(SEL)selector
{
	NSAssert( eventQueueCount < QUEUE_EVENT_MAX, @"CCEventDispatcher: recompile. Increment QUEUE_EVENT_MAX value");

	@synchronized (self) {
		eventQueue[eventQueueCount].selector = selector;
		eventQueue[eventQueueCount].event = [event copy];
		
		eventQueueCount++;
	}
}

-(void) dispatchQueuedEvents
{
	@synchronized (self) {
		for( int i=0; i < eventQueueCount; i++ ) {
			SEL sel = eventQueue[i].selector;
			NSEvent *event = eventQueue[i].event;
			
			[self performSelector:sel withObject:event];
			
			[event release];
		}
		
		eventQueueCount = 0;
	}
}
#endif // CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD

//NS_CC_END;
@end
