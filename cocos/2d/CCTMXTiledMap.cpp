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
#include "2d/CCTMXTiledMap.h"
#include "2d/CCTMXXMLParser.h"
#include "2d/CCTMXLayer.h"
#include "2d/CCSprite.h"
#include "base/ccUTF8.h"

namespace cocos2d {

// implementation TMXTiledMap

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
    CCASSERT(tmxFile.size()>0, "TMXTiledMap: tmx file should not be empty");

    _tmxFile = tmxFile;

    setContentSize(Size::ZERO);

    retaining_ptr<TMXMapInfo> mapInfo = to_retaining_ptr( TMXMapInfo::create(tmxFile) );

    if (! mapInfo)
    {
        return false;
    }
    CCASSERT( !mapInfo->_tilesets.empty(), "TMXTiledMap: Map not found. Please check the filename.");

    buildWithMapInfo(std::move( mapInfo ));

    return true;
}

bool TMXTiledMap::initWithXML(const std::string& tmxString, const std::string& resourcePath)
{
    _tmxFile = tmxString;

    setContentSize(Size::ZERO);

    retaining_ptr<TMXMapInfo> mapInfo = to_retaining_ptr( TMXMapInfo::createWithXML(tmxString, resourcePath) );

    CCASSERT( !mapInfo->_tilesets.empty(), "TMXTiledMap: Map not found. Please check the filename.");
    buildWithMapInfo(std::move( mapInfo ));

    return true;
}

TMXTiledMap::TMXTiledMap()
    :_mapSize(Size::ZERO)
    ,_tileSize(Size::ZERO)        
    ,_tmxFile("")
    , _tmxLayerNum(0)
{
}

TMXTiledMap::~TMXTiledMap()
{
}

// private
TMXLayer * TMXTiledMap::parseLayer(retaining_ptr<TMXLayerInfo> layerInfo, TMXMapInfo *mapInfo)
{
    auto tileset = tilesetForLayer(layerInfo.get(), mapInfo);

    if (tileset == nullptr)
        return nullptr;
    
    TMXLayer *layer = TMXLayer::create(std::move( tileset ), std::move( layerInfo ), *mapInfo);

    if (nullptr != layer)
    {
        layer->setupTiles();
    }

    return layer;
}

retaining_ptr<TMXTilesetInfo> TMXTiledMap::tilesetForLayer(TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo)
{
    auto height = static_cast<uint32_t>(layerInfo->_layerSize.height);
    auto width  = static_cast<uint32_t>(layerInfo->_layerSize.width);
    auto & tilesets = mapInfo->_tilesets;

    for (auto iter = tilesets.rbegin(), end = tilesets.rend(); iter != end; ++iter)
    {
        auto & tileset = *iter;

        if (tileset)
        {
            for (uint32_t y = 0; y < height; y++)
            {
                for (uint32_t x = 0; x < width; x++)
                {
                    auto pos = x + width * y;
                    auto gid = layerInfo->_tiles[ pos ];

                    // FIXME:: gid == 0 --> empty tile
                    if (gid != 0)
                    {
                        // Optimization: quick return
                        // if the layer is invalid (more than 1 tileset per layer)
                        // an CCASSERT will be thrown later
                        if (tileset->_firstGid < 0 ||
                            (gid & kTMXFlippedMask) >= static_cast<uint32_t>(tileset->_firstGid))
                            // TODO FIXME
                            // must be shared
                            return to_retaining_ptr( tileset.get() ); // shared_ptr
                    }
                }
            }        
        }
    }

    // If all the tiles are 0, return empty tileset
    CCLOG("cocos2d: Warning: TMX Layer '%s' has no tiles", layerInfo->_name.c_str());

    return retaining_ptr<TMXTilesetInfo>();
}

void TMXTiledMap::buildWithMapInfo(retaining_ptr<TMXMapInfo> mapInfo)
{
    _mapSize = mapInfo->_mapSize;
    _tileSize = mapInfo->_tileSize;
    _mapOrientation = mapInfo->_orientation;

    _objectGroups = std::move(mapInfo->_objectGroups);

    _properties = mapInfo->_properties;

    _tileProperties = mapInfo->_tileProperties;

    int idx = 0;

    for (auto & layerInfo : mapInfo->_layers)
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
            currentSize.width = std::max(currentSize.width, childSize.width);
            currentSize.height = std::max(currentSize.height, childSize.height);
            this->setContentSize(currentSize);

            idx++;
        }
    }
    _tmxLayerNum = idx;
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

bool TMXTiledMap::getPropertiesForGID(int GID, Value** value)
{
    if (_tileProperties.find(GID) != _tileProperties.end()) {
        *value = &_tileProperties.at(GID);
        return true;
    } else {
        return false;
    }
}

std::string TMXTiledMap::getDescription() const
{
    return StringUtils::format("<TMXTiledMap | Tag = %d, Layers = %d",
                               _tag,
                               static_cast<int>(getChildren().size()));
}

int TMXTiledMap::getLayerNum()
{
    return _tmxLayerNum;
}

} // namespace cocos2d
