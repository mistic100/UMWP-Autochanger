var nsg = require('node-sprite-generator');

nsg({
    src: [
        'icons/*.png'
    ],
    spritePath: 'sprite.png',
    stylesheetPath: 'sprite.css',
    stylesheet: 'prefixed-css',
    stylesheetOptions: {
        prefix: 'icon-'
    },
    layout: 'packed'
}, function(err) {
    if (err) {
        console.error(err.message);
    }
    else {
        console.log('Sprite generated!');
    }
});