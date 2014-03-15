var ASCGIF = ASCGIF || {};
    ASCGIF.frameRate = 2, // Show 3 frames per second
    ASCGIF.frameSep = 'thisisthedelimiterbro',
    ASCGIF.defaultPort = '5050',
    ASCGIF.port = ASCGIF.defaultPort,
    ASCGIF.params = {},
    ASCGIF.params.gif = '?gif=',
    ASCGIF.frames = [],
    ASCGIF.rawData = '',
    ASCGIF.url = '',
    ASCGIF.defaultUrl = 'http://media2.giphy.com/media/12gRrmtMlhC9O/giphy.gif',
    ASCGIF.animation;

var respCallback = function(data) {
    ASCGIF.rawData = data;
};

;(function() {

    var run = function() {
        getPort();
        hideError();
        resetAnimation();
        getFileUrl();
        console.log(ASCGIF.port);
        var request = $.ajax({
            url: 'http://localhost:' + ASCGIF.port + '/' + ASCGIF.params.gif + ASCGIF.url,
            type: 'GET',
            crossDomain: true,
            dataType: 'jsonp',
            // timeout: 3000,
            beforeSend: function() {
                showLoading();
            },
            complete: function(xhr, textStatus) {
                hideLoading();
                if (xhr.status !== 200) {
                    showError();
                } else {
                    parseFrames();
                    animate();
                }
            }
        });
    };

    var animate = function() {
        var frame = 0;
        ASCGIF.animation = setInterval(function() {
            $('div.ascgif-demo').html(ASCGIF.frames[frame]);
            frame = (frame + 1) % ASCGIF.frames.length;
        }, 1000 / ASCGIF.frameRate);
    };

    var parseFrames = function() {
        ASCGIF.frames = ASCGIF.rawData.split(ASCGIF.frameSep);
    };

    var resetAnimation = function() {
        ASCGIF.frames = [];
        ASCGIF.rawData = '';
        clearInterval(ASCGIF.animation);
    };

    var getFileUrl = function() {
        ASCGIF.url = $('input.file-url').val() || ASCGIF.defaultUrl;
    };

    var getPort = function() {
        var port = "" + $('input.port-number').val();
        if (port === "") {
            ASCGIF.port = ASCGIF.defaultPort;
        } else {
            ASCGIF.port = port;
        }
    };

    var showLoading = function() {
        $('div.loading-message-box').slideDown();
    };

    var hideLoading = function() {
        $('div.loading-message-box').slideUp();
    };

    var showError = function() {
        $('div.error-message-box').slideDown();
    };

    var hideError = function() {
        $('div.error-message-box').slideUp();
    };

    /*
     * 1. Bind the click event to the asciify button
     * 2. Fade in content
     * 3. Start a demo run with the default .gif url
     */
    $('div.menu-button').click(function() {
        $('div.menu-content').slideToggle();
    });
    $('button.asciify').click(function() {
        run();
    });
    $('span.close-error').click(function() {
        hideError();
    });
    $('div.content-wrapper').fadeTo(500, 1.0);
    run();

})();
