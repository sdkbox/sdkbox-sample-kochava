var HelloWorldLayer = cc.Layer.extend({
    sprite: null,
    ctor: function() {
        //////////////////////////////
        // 1. super init first
        this._super();

        cc.log("Sample Startup")

        this.createTestMenu();

        var winsize = cc.winSize;

        var logo = new cc.Sprite("res/Logo.png");
        var logoSize = logo.getContentSize();
        logo.x = logoSize.width / 2;
        logo.y = winsize.height - logoSize.height / 2;
        this.addChild(logo);

        var quit = new cc.MenuItemLabel(new cc.LabelTTF("QUIT", "sans", 32), function() {
            cc.log("QUIT");
        });
        var menu = new cc.Menu(quit);
        var size = quit.getContentSize();
        menu.x = winsize.width - size.width / 2 - 16;
        menu.y = size.height / 2 + 16;
        this.addChild(menu);

        return true;
    },

    createTestMenu: function() {
        sdkbox.PluginKochava.init();

        let self = this;
        var item1 = new cc.MenuItemLabel(new cc.LabelTTF("track event", "sans", 28), function() {
            cc.log("track event");
            sdkbox.PluginKochava.trackEvent("test event", "1111");
        });
        var item2 = new cc.MenuItemLabel(new cc.LabelTTF("retrieve attribution", "sans", 28), function() {
            cc.log("retrieve attribution");
            let attrs = sdkbox.PluginKochava.retrieveAttribution();
            let s = self.dumpObject(attrs);
            cc.log('Attr:' + s);
        });

        var winsize = cc.winSize;
        var menu = new cc.Menu(item1, item2);
        menu.x = winsize.width / 2;
        menu.y = winsize.height / 2;
        menu.alignItemsVerticallyWithPadding(20);
        this.addChild(menu);

        sdkbox.PluginKochava.setAttributionCallback(function(attrs) {
            cc.log("Kochava setAttributionCallback");
            cc.log(attrs);
        });
        sdkbox.PluginKochava.setBeaconCallback(function(str) {
            cc.log("Kochava kochavaCallbackString " + str);
        });
    },

    dumpObject: function(arr, level) {
        var dumped_text = "";
        if (!level) level = 0;

        var level_padding = "";
        for (var j = 0; j < level + 1; j++) level_padding += "    ";

        if (typeof(arr) == 'object') {
            for (var item in arr) {
                var value = arr[item];

                if (typeof(value) == 'object') {
                    dumped_text += level_padding + "'" + item + "' ...\n";
                    dumped_text += mydump(value, level + 1);
                } else {
                    dumped_text += level_padding + "'" + item + "' => \"" + value + "\"\n";
                }
            }
        } else {
            dumped_text = "===>" + arr + "<===(" + typeof(arr) + ")";
        }
        return dumped_text;
    }
});

var HelloWorldScene = cc.Scene.extend({
    onEnter: function() {
        this._super();
        var layer = new HelloWorldLayer();
        this.addChild(layer);
    }
});
