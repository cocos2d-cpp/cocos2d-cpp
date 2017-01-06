/****************************************************************************
Copyright (c) 2009-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2016 Chukong Technologies Inc.

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
#include "2d/CCFastTMXTiledMap.h"
#include "2d/CCFastTMXLayer.h"
#include "base/ccUTF8.h"

namespace cocos2d {
namespace experimental {

// implementation FastTMXTiledMap

TMXTiledMap * TMXTiledMap::create(const std::string& tmxFile)
{
    TMXTiledMap *ret = new (std::nothrow) TMXTiledMap();
    if (ret->initWithTMXFile(tmxFile))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

TMXTiledMap* TMXTiledMap::createWithXML(const std::string& tmxString, const std::string& resourcePath)
{
    TMXTiledMap *ret = new (std::nothrow) TMXTiledMap();
    if (ret->initWithXML(tmxString, resourcePath))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TMXTiledMap::initWithTMXFile(const std::string& tmxFile)
{
    CCASSERT(tmxFile.size()>0, "FastTMXTiledMap: tmx file should not be empty");
    
    setContentSize(Size::ZERO);

    std::unique_ptr<TMXMapInfo> mapInfo( TMXMapInfo::create(tmxFile) );

    if (! mapInfo)
    {
        return false;
    }
    CCASSERT( !mapInfo->_tilesets.empty(), "FastTMXTiledMap: Map not found. Please check the filename.");
    buildWithMapInfo(std::move( mapInfo ));

    return true;
}

bool TMXTiledMap::initWithXML(const std::string& tmxString, const std::string& resourcePath)
{
    setContentSize(Size::ZERO);

    std::unique_ptr<TMXMapInfo> mapInfo( TMXMapInfo::createWithXML(tmxString, resourcePath) );

    CCASSERT( !mapInfo->_tilesets.empty(), "FastTMXTiledMap: Map not found. Please check the filename.");
    buildWithMapInfo(std::move( mapInfo ));

    return true;
}

TMXTiledMap::TMXTiledMap()
    :_mapSize(Size::ZERO)
    ,_tileSize(Size::ZERO)        
{
}

TMXTiledMap::~TMXTiledMap()
{
}

// private
TMXLayer * TMXTiledMap::parseLayer(std::unique_ptr<TMXLayerInfo> layerInfo, TMXMapInfo *mapInfo)
{
    auto tileset = tilesetForLayer(layerInfo.get(), mapInfo);
    if (! tileset)
        return nullptr;
    
    TMXLayer *layer = TMXLayer::create(tileset, std::move( layerInfo ), *mapInfo);

    // tell the layerinfo to release the ownership of the tiles map.
    layer->setupTiles();

    return layer;
}

std::shared_ptr<TMXTilesetInfo> TMXTiledMap::tilesetForLayer(TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo)
{
    Size size = layerInfo->_layerSize;
    auto & tilesets = mapInfo->_tilesets;

    for (auto iter = tilesets.crbegin(), iterCrend = tilesets.crend(); iter != iterCrend; ++iter)
    {
        auto & tilesetInfo = *iter;

        if (tilesetInfo)
        {
            for( int y=0; y < size.height; y++ )
            {
                for( int x=0; x < size.width; x++ )
                {
                    uint32_t pos = static_cast<uint32_t>(x + size.width * y);
                    uint32_t gid = layerInfo->_tiles[ pos ];
                    
                    // gid are stored in little endian.
                    // if host is big endian, then swap
                    //if( o == CFByteOrderBigEndian )
                    //    gid = CFSwapInt32( gid );
                    /* We support little endian.*/
                    
                    // FIXME: gid == 0 --> empty tile
                    if( gid != 0 )
                    {
                        // Optimization: quick return
                        // if the layer is invalid (more than 1 tileset per layer) an CCASSERT will be thrown later
                        if( (gid & kTMXFlippedMask)
                            >= static_cast<uint32_t>(tilesetInfo->_firstGid))
                        {
                            return tilesetInfo;
                        }
                    }
                }
            }
        }
    }

    // If all the tiles are 0, return empty tileset
    CCLOG("cocos2d: Warning: TMX Layer '%s' has no tiles", layerInfo->_name.c_str());
    return nullptr;
}

void TMXTiledMap::buildWithMapInfo(std::unique_ptr<TMXMapInfo> mapInfo)
{
    _mapSize = mapInfo->_mapSize;
    _tileSize = mapInfo->_tileSize;
    _mapOrientation = mapInfo->_orientation;

    _objectGroups = std::move( mapInfo->_objectGroups );

    _properties = mapInfo->_properties;

    _tileProperties = mapInfo->_tileProperties;

    int idx=0;

    for(auto & layerInfo : mapInfo->_layers)
    {
        if (layerInfo->_visible)
        {
            TMXLayer *child = parseLayer(std::move( layerInfo ), mapInfo.get());
            if (child == nullptr) {
                idx++;
                continue;
            }
            addChild(child, idx, idx);
            
            // update content size with the max size
            const Size& childSize = child->getContentSize();
            Size currentSize = this->getContentSize();
            currentSize.width = std::max( currentSize.width, childSize.width );
            currentSize.height = std::max( currentSize.height, childSize.height );
            this->setContentSize(currentSize);
            
            idx++;
        }
    }
}

// public
TMXLayer * TMXTiledMap::getLayer(const std::string& layerName) const
{
    CCASSERT(layerName.size() > 0, "Invalid layer name!");
    
    for (auto & child : getChildren())
    {
        TMXLayer* layer = dynamic_cast<TMXLayer*>(child.get());
        if(layer)
        {
            if(layerName.compare( layer->getLayerName()) == 0)
            {
                return layer;
            }
        }
    }

    // layer not found
    return nullptr;
}

TMXObjectGroup * TMXTiledMap::getObjectGroup(const std::string& groupName) const
{
    CCASSERT(groupName.size() > 0, "Invalid group name!");

    for (const auto & objectGroup : _objectGroups)
    {
        if (objectGroup && objectGroup->getGroupName() == groupName)
        {
            return objectGroup.get();
        }
    }

    // objectGroup not found
    return nullptr;
}

Value TMXTiledMap::getProperty(const std::string& propertyName) const
{
    if (_properties.find(propertyName) != _properties.end())
        return _properties.at(propertyName);
    
    return Value();
}

Value TMXTiledMap::getPropertiesForGID(int GID) const
{
    if (_tileProperties.find(GID) != _tileProperties.end())
        return _tileProperties.at(GID);
    
    return Value();
}

std::string TMXTiledMap::getDescription() const
{
    return StringUtils::format("<FastTMXTiledMap | Tag = %d, Layers = %d",
                               _tag,
                               static_cast<int>(getChildren().size()));
}

} //end of namespace experimental

} // namespace cocos2d

