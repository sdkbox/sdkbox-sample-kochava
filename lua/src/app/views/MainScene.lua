
local MainScene = class("MainScene", cc.load("mvc").ViewBase)

function MainScene:onCreate()
    print("Sample Startup")

    local label = cc.Label:createWithSystemFont("QUIT", "sans", 32)
    local quit = cc.MenuItemLabel:create(label)
    quit:onClicked(function()
        os.exit(0)
    end)
    local size = label:getContentSize()
    local menu = cc.Menu:create(quit)
    menu:setPosition(display.right - size.width / 2 - 16, display.bottom + size.height / 2 + 16)
    self:addChild(menu)

    self:setupTestMenu()
end

function MainScene:setupTestMenu()
    sdkbox.PluginKochava:init()
    sdkbox.PluginKochava:setAttributionCallback(function(t)
        dump(t)
    end)
    sdkbox.PluginKochava:setBeaconCallback(function(s)
        print(s)
    end)

    local item1 = cc.MenuItemLabel:create(cc.Label:createWithSystemFont("track event", "sans", 28))
        :onClicked(function()
            print("track event")
            sdkbox.PluginKochava:trackEvent("test event", "1111")
        end)
    local item2 = cc.MenuItemLabel:create(cc.Label:createWithSystemFont("retrieve attribution", "sans", 28))
        :onClicked(function()
            print("retrieve attribution")
            local attribution = sdkbox.PluginKochava:retrieveAttribution()
            dump(attribution)
        end)

    local menu = cc.Menu:create(item1, item2)
    menu:alignItemsVerticallyWithPadding(24)
    self:addChild(menu)
end

return MainScene
