var video = document.getElementById('myVideo');
// var canvas = document.getElementById('canvas');
// var ctx = canvas.getContext('2d');

var tolProcessWidth = 580;
var marginProcess = 20 + 10;
// $(".video").trigger("play");
$('.bt').addClass('pause');
$('.bt').click(function() {
    if ($(this).hasClass('pause')) {
        
        $(this).parent().prev().trigger("play");
        
        $(this).removeClass('pause');
        $(this).addClass('play');
    } else {
        $(this).parent().prev().trigger("pause");
        $(this).removeClass('play');
        $(this).addClass('pause');
    }
});

$('.rewind').click(function () {
    $(this).parent().prev().get(0).currentTime -= 20;
});

$('.forword').click(function () {
    $(this).parent().prev().get(0).currentTime += 20;
});

$('.rate').click(function () {
    
    rate = $(this).parent().prev().get(0).playbackRate % 3;
    $(this).parent().prev().get(0).playbackRate = rate + 1;
    console.log($(this).parent().prev().get(0).playbackRate);
});

// process bar
$('.video').get(0).addEventListener("timeupdate", function () {
    var totalTime = this.duration;
    var curProcess = (this.currentTime / totalTime) * tolProcessWidth;
    $(this).parent().find('.cur_process').css({width: curProcess});
}, false);

// 
$('.process').click(function (event) {
    event = event || window.event;
    var absluteX = event.pageX || e.clientX + document.body.scrollLeft;
    console.log(absluteX);
    var videoX = $(this).parent().parent().parent().position().left;
    console.log(videoX);
    var x = absluteX - videoX - marginProcess;
    console.log(x);
    var tolTime = $(this).parent().parent().prev().get(0).duration;
    
    // change currProcess style
    $(this).parent().children('.cur_process').css({width: x});
    // change video curr time
    console.log($(this).parent().parent().prev().get(0))
    $(this).parent().parent().prev().get(0).currentTime = (x / tolProcessWidth) * tolTime;
});


// new function
var _videoWidth = 1920;
var _videoHeight = 1080;

var cvWidth = 320;
var cvHeight = 180;

var windowHieght = $('.result_window').height();

var posX = 110;
var posY = 40;
var humWidth = 30;
var humHeight = 80;

$('.screenshot').click(function () {
    screenShot(0);
});

function screenShot(index) {
    var newCanvas = document.createElement('canvas');
    $(newCanvas).css({
        'width': cvWidth, 
        'Height': cvHeight, 
        'margin-bottom': '10px',
        'display': 'none'
    });
    var newCtx = newCanvas.getContext('2d');
    // get video image
    newCtx.drawImage(video, 0, 0, _videoWidth, _videoHeight, 0, 0, cvWidth, cvHeight);
    
    newCtx.fillStyle = "rgba(0, 0, 125, 0.3)";
    var path = new Path2D();
    path.moveTo(0, 0);
    path.lineTo(0, cvHeight);
    path.lineTo(cvWidth, cvHeight);
    path.lineTo(cvWidth, posY + humHeight);
    path.lineTo(posX, posY + humHeight);
    path.lineTo(posX, posY);
    path.lineTo(posX + humWidth, posY);
    path.lineTo(posX + humWidth, posY + humHeight);
    path.lineTo(cvWidth, posY + humHeight);
    path.lineTo(cvWidth, 0);
    path.lineTo(0, 0);
    
    newCtx.fill(path);
    
    console.log($('.result_list')[index])
    $($('.result_list')[index]).append(newCanvas);
    
    // add animation    
    $(newCanvas).fadeIn(500);
    
    var listHeight = $($('.result_list')[index]).height();
    console.log(listHeight)
    $($('.result_window')[index]).animate({
        scrollTop: listHeight
    }, 700);
}

function addTarget(container, src) {
    var item = $('<img class="target_item">');
    item.attr('src', src);
    
    container.append(item);
    
    // animate
    item.fadeIn(500);
    
    var listHeight = container.height();
    console.log(listHeight)
    container.parent().animate({
        scrollTop: listHeight
    }, 300);
    
    // addListenEvent
    item.click(function() {
        console.log(item.attr('src'));
    });    
}

var i = 0;
setInterval(function () {
    // addTarget($('.target_list'), './imgs/target_2.bmp');
}, 200);


// switch the camera
function switchCamera(button, window) {
    $('.one_camera').css({background: 'rgba(0, 0, 255, 0.4)'});
    $(button).css({background: 'rgba(255, 0, 0, 0.4)'});
    
    $('.result_window').css({display: 'none'});
    $(window).css({display: 'block'});
}

// addListenEvent
$('.one_camera').click(function () {
    var className = this.className;
    var a = className.split(' ');
    
    switchCamera(this, $('.result_window')[a[1]]);
});