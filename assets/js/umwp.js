$(function() {
    var $root = $('html, body');
    $('a').click(function() {
        var href = $.attr(this, 'href');
        var target = $(href);

        if (target.length) {
            $root.animate({
                scrollTop: target.offset().top
            }, 500, function() {
                window.location.hash = href;
                pk.checkScrollForTransparentNavbar();
            });

            return false;
        }
    });
});