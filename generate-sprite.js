require('node-sprite-generator')({
    src: [
        'icons/*.png'
    ],
    spritePath: 'assets/css/umwp-sprite.png',
    stylesheetPath: 'assets/css/umwp-sprite.css',
    stylesheet: 'prefixed-css',
    stylesheetOptions: {
        prefix: 'umwp-'
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