$(function() {
    $.get('last-version.txt', function(data) {
        var lastVersion = data.split('\n');
        $('#version').text(lastVersion[0]);
    });
});