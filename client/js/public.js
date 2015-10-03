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
    
    // console.log(_videoWidth)
    // ctx.fillStyle = '#fff';
    // ctx.fillRect(0, 0, _videoWidth, _videoHeight);
    // ctx.drawImage(video, 0, 0, _videoWidth, _videoHeight, 0, 0, cvWidth, cvHeight);
    // 
   
    
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
    // newCtx.clearRect(30, 30, 40, 80);
    
    // draw human
    // var humCanvas = document.createElement('canvas');
    // var humCtx = humCanvas.getContext('2d');
    // 
    // humCtx.fillStyle = "rgba(0, 0, 125, 0.3)";
    // humCtx.fillRect(0, 0, cvWidth, cvHeight);
    // humCtx.clearRect(30, 30, 40, 80);
    
    // $(newCanvas).append(humCtx);
    
    $('.result_list').append(newCanvas);
    
    // add animation
    console.log($('.result_list').scrollTop())
    
    $(newCanvas).fadeIn(500);
    
    var listHeight = $('.result_list').height();
    // $('.result_window').scrollTop(listHeight);
    $('.result_window').animate({
        scrollTop: listHeight
    }, 700);
})