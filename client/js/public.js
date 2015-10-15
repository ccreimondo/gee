// SLIDE & TRANSCOLOR are for animate 
var SLIDE;
var TRANSCOLOR;
var ROOT = 'http://10.250.94.25:5000';
var FPS = 30;
var POS = [];

var CURR_TARGET = 0;

function changeWidth(container, width, num) {
    $(container).css({
        width: width * num
    })
}
function slideAnalyzing(anly) {
    console.log(anly);
    
    anly.css({dispaly: 'block'});
    
    var canvas = anly.get(0);
    var ctx = canvas.getContext('2d');
        
    var img = new Image();
    img.src = './imgs/analyzing.png';
    
    var k_left = new Image();
    k_left.src = './imgs/k_left.png';
    
    var k_right = new Image();
    k_right.src = './imgs/k_right.png';
    
    img.onload = function() {
        ctx.fillStyle = 'rgba(231, 76, 60, 0.8)';
        // ctx.fillRect(6, 4, 315, 36);
        // ctx.font = "24px SimHei";
        // //设置字体填充颜色
        // ctx.fillStyle = 'rgb(236, 240, 241)';
        // //从坐标点(50,50)开始绘制文字
        // ctx.fillText("分 析 中", 113, 32);
        ctx.drawImage(img, 6, 4);
        ctx.drawImage(k_left, 0, 0);
        ctx.drawImage(k_right, 316, 0)
        SLIDE = self.setInterval(slideOut, 1);
    }

    var w1 = 164;
    var w2 = 164;
    
    var width = 327;
    var height = 44;
    
    var posX = 6;
    var posY = 4;
    
    var interval = 6;
    
    function slideOut() {
        ctx.clearRect(0, 0, width, height);
        
        // ctx.fillStyle = 'rgba(231, 76, 60, 0.8)';
        // ctx.fillRect(posX, posY, 315, 36);
        // //设置字体填充颜色
        // ctx.fillStyle = 'rgb(236, 240, 241)';
        // //从坐标点(50,50)开始绘制文字
        // ctx.fillText("分 析 中", 113, 32);
        ctx.drawImage(img, posX, posY);
        ctx.clearRect(0, 0, w1, height);
        ctx.clearRect(w2, 0, width, height);
        
        ctx.drawImage(k_left, w1 - interval, 0);
        ctx.drawImage(k_right, w2 + interval, 0)
        
        if (w1 < 0) {
            ctx.clearRect(0, 0, width, height);
            // ctx.fillStyle = 'rgba(231, 76, 60, 0.8)';
            // ctx.fillRect(posX, posY, 315, 36);
            // //设置字体填充颜色
            // ctx.fillStyle = 'rgb(236, 240, 241)';
            // //从坐标点(50,50)开始绘制文字
            // ctx.fillText("分 析 中", 113, 32);
            ctx.drawImage(img, posX, posY);
            ctx.drawImage(k_left, 0, 0);
            ctx.drawImage(k_right, 316, 0)
            
            window.clearInterval(SLIDE);
        }
        w1 -= 4;
        w2 += 4;
    }
}

// setInterval(transColor, 1000);

function startFlash() {
    $('.analyzing').css({display: 'none'});
    $('.anly_block').css({display: 'block'});
    TRANSCOLOR = self.setInterval(transColor, 1000);
}
function endFlash() {
    window.clearInterval(TRANSCOLOR);
}

function transColor() {
    $('.anly').css({
        'background-color': 'rgba(236, 240, 241, 0)',
        'color': 'rgba(231, 76, 60, 0.8)'
    });
    
    setTimeout(function () {
        $('.anly').css({
            'background-color': 'rgba(231, 76, 60, 0.8)',
            'color': 'rgb(236, 240, 241)'
        });
    }, 500)
}

function bindTargetSwitchEvent(curr, max, min, interval) {
    var currDeg = curr;
    var maxDeg = max;
    var minDeg = min + interval;
    
    // init switch
    transform($('#container').get(0), "rotateY(" + currDeg + "deg)");
    
    // before bind, need unbind ex-bind
    $('.left_turn').unbind('click');
    $('.right_turn').unbind('click');
    
    $('.left_turn').bind('click', function (evevt) {
        // stop the bubbling event
        event.stopPropagation();
        
        if (currDeg >= maxDeg) return;
        
        // TODO 
        CURR_TARGET--;
        console.log(CURR_TARGET);
        
        currDeg += interval;
        transform($('#container').get(0), "rotateY(" + currDeg + "deg)");
    });
    
    $('.right_turn').bind('click', function (event) {
        // stop the bubbling event
        event.stopPropagation();
        
        if (currDeg <= minDeg) return;
        // TODO
        CURR_TARGET++;
        console.log(CURR_TARGET);
        
        currDeg -= interval;
        transform($('#container').get(0), "rotateY(" + currDeg + "deg)");
    });
}
// bind cancle choose target
$('.choose_target').bind('click', function () {
    // hidden the choose page
    $('.confirm_box').css({display: 'none'});
    $(this).css({display: 'none'});
    
    // could re-choose a target
    $('.anly_block').css({display: 'none'});
    $('.capture').css({display: 'block'});
})

var transform = function (element, value, key)  
{  
    key = key || "Transform";  
    ["Moz", "O", "Ms", "Webkit", ""].forEach(function (prefix)  
    {  
        element.style[prefix + key] = value;  
    });  
    return element;  
}  


$('.result_item').click(function () {
    console.log(this);
});

// addListen to switch function
$('.monitor_nav').bind('click', function () {
    // console.log($(this));
    switchFunction($(this));
})

function switchFunction(currFun) {
    // If in the current function
    if (currFun.hasClass('active')) return;
    
    // switch to history
    if (currFun.hasClass('realtime_fun')) {
        $('.detail_page').css({display: 'none'});
        $('.history_fun').removeClass('active');
        
        $('.realtime_monitor').css({display: 'block'});
        $('.realtime_fun').addClass('active');  
        
        // switch the background
        $('.nav').css({background: 'url("./imgs/realtime_fun.png")'});
        
        // get real time video
        getRealtimeVideoList2();
        
    }
    // switch to real-time video list
    else if (currFun.hasClass('history_fun')) {
        $('.realtime_monitor').css({display: 'none'});
        $('.realtime_fun').removeClass('active');
        
        $('.detail_page').css({display: 'block'});
        $('.history_fun').addClass('active');
        
        // switch the background
        $('.nav').css({background: 'url("./imgs/history_fun.png")'});
        
        // send history video list request
        getHistoryVideoList('20131801');
        // init layout
        initHistoryListLayout();
    }
}

// For real-time monitor
function changeRealtimeStatu(videoWrap, statu, duration) {
    // remove current statu first
    ['idle', 'got', 'finding'].forEach(function (className) {
        videoWrap.removeClass(className);
    });
    // add changedTo statu
    videoWrap.addClass(statu);
    // action for each statu
    switch (statu) {
        case "idle":
            changeToIdleStatu(videoWrap);
            break;
        case "finding":
            changeToFindingStatu(videoWrap);
            break;
        case "got":
            changeToGotStatu(videoWrap, duration);
            break;
        default:
            console.log('Statu error!');
    }
}
function changeToIdleStatu(videoWrap) {
    videoWrap.css("background-image", "url('./imgs/small_wrap.png')");
}
function changeToFindingStatu(videoWrap) {
    // videoWrap.css("background-image", "url('./imgs/finding_bg.png')");
}
function changeToGotStatu(videoWrap, duration) {
    // sparkle
    var sparkle = self.setInterval(function () {
        videoWrap.css("background-image", "url('./imgs/got_bg.png')")
        setTimeout(function () {
            videoWrap.css("background-image", "url('./imgs/small_wrap.png')")
        }, 300)
    }, 600);
    
    // clear sparkle
    setTimeout(function () {
        window.clearInterval(sparkle);
        // return finding statu
        setTimeout(function () {
            changeRealtimeStatu(videoWrap, 'finding');
        }, 300)        
    }, duration);
}
// changeToGotStatu($($('.idle')[1]), 2000);

// addListener to switch result window
$('.one_camera').bind('click', function () {
   switchResultWindow($(this)); 
});

function switchResultWindow(currWindow) {
    
    if (currWindow.hasClass('active')) return;
    
    // init all unfocuse
    var cameras = $('.one_camera');
    for (var i = 0; i < cameras.length; ++i) {
        var bg = 'url("./imgs/unf_00' + i + '.png")';
        $(cameras[i]).css({background: bg});
    }
    
    $('.one_camera').removeClass('active');
    currWindow.addClass('active');
    
    // bottom map to window
    var cArry = currWindow.attr('class').split(' ');
    var iArry = cArry[1].split('_');
    var index = iArry[2];
    
    var windowClassName = 'result_window_' + index;
    
    currWindow.css({
        background: 'url("./imgs/f_00' + index + '.png")'
    });
    
    // handle all
    $('.result_window').css({display: 'none'});
    $('.' + windowClassName).css({display: 'block'});
    
    // band swiper again
    var swiper = new Swiper('.swiper-container', {
        scrollbar: '.swiper-scrollbar',
        scrollbarHide: true,
        slidesPerView: 'auto',
        centeredSlides: false,
        spaceBetween: 30,
        grabCursor: true
    });
    // console.log($(windowClassName))
}
  
// listner to video
$('.play_pause').bind('click', function () {
    var video = $(this).parent().parent().children('video');
    
    
    if ($(this).hasClass('pause')) {
        video.trigger("play");
        $(this).removeClass('pause');
        $(this).addClass('play');
        console.log('pause')
    }
    else if ($(this).hasClass('play')) {
        video.trigger("pause");
        $(this).removeClass('play');
        $(this).addClass('pause');
        console.log('play')
    }
});

$('.speedup').bind('click', function () {
    var rate = $(this).parent().prev().get(0).playbackRate % 3;
    $(this).parent().prev().get(0).playbackRate = rate + 1;
    var currRate = $(this).parent().prev().get(0).playbackRate; 
    
    var speedupRate = $(this).parent().find('.speedup_rate');
    speedupRate.html('x' + currRate);
    
    speedupRate.css({display: 'block'});
    
    setTimeout(function () {
        speedupRate.css({display: 'none'});
    }, 500);
});

$('.maxum').bind('click', function () {
    // get video
    var video = $(this).parent().prev().get(0);
    launchFullscreen(video);
});
$('.monitor_mixum').bind('click', function () {
    var video = $(this).parent().prev().get(0);
    launchFullscreen(video);
})

// when click the progress bar
$('.process_area').bind('click', function (event) {
    var x = event.offsetX;
    
    // there has some bug with siblings
    $(this).parent().children('.curr_process').css({width: x});
    // the track
    $(this).siblings('.track_point').css({left: x - 8});
    
    // refresh time
    var tolTime = $(this).parent().parent().prev().get(0).duration;
    var tolProcessWidth = $(this).parent().children('.tol_process').width();
    $(this).parent().parent().prev().get(0).currentTime = (x / tolProcessWidth) * tolTime;
    
    console.log((x / tolProcessWidth) * tolTime);
    console.log(tolTime);
    console.log(tolProcessWidth);
})

// bind process bar
$('.html5_video').bind("timeupdate", function () {
    var totalTime = this.duration;
    var tolProcessWidth = $(this).parent().find('.tol_process').width();
    
    var curProcess = (this.currentTime / totalTime) * tolProcessWidth;
    
    // change the progress bar style
    $(this).parent().find('.curr_process').css({width: curProcess});
    $(this).parent().find('.track_point').css({left: curProcess - 8});
    
    if (curProcess == tolProcessWidth) {
        console.log('a')
        $(this).parent().find('.play_pause').addClass('pause');
        $(this).parent().find('.play_pause').removeClass('play');
    }
    
    // change the current time
    var currTime = $(this).parent().find('.curr_time');
    var startTime = currTime.attr('class').split('start_time')[1];
    var startSecond = timeToSecond(startTime);
    var currSecond = startSecond + this.currentTime;
    currTime.html(secondToTime(currSecond));
});


function switchDetailCamera(allCam, currCam) {
    
    // TODO     
    
    // when touch again, return
    if (currCam.hasClass('camera_wrap_detail')) return;
    
    // get curr video num
    var cArry = currCam.attr('class').split(' ');
    var iArry = cArry[1].split('_');
    var i = iArry[2];
    
    allCam.removeClass('camera_wrap_small');
    allCam.addClass('camera_wrap_detail');
    
    // show something hiddend just now
    $('.result_container').css({display: 'block'});
    $('.camera_container').css({height: '610px'});
    
    // show curr
    allCam.css({display: 'none'});
    currCam.css({display: 'block'});
    
    // change the tips style
    currCam.parent().find('.tips').removeClass('tips_small');
    currCam.parent().find('.tips').addClass('tips_detail');
    currCam.parent().find('.tips').css({display: 'none'});
    
    $(currCam.parent().find('.tips')[i - 1]).css({display: 'block'});
    console.log($(currCam.parent().find('.tips')[i - 1]));
    
    // band swiper
    var swiper = new Swiper('.swiper-container', {
        scrollbar: '.swiper-scrollbar',
        scrollbarHide: true,
        slidesPerView: 'auto',
        centeredSlides: false,
        spaceBetween: 30,
        grabCursor: true
    });
    
    // repaint process track with result
    paintProgressTrack(POS);
    
    $('.video_list_fun').css('display', 'block');
    
    slideAnalyzing($('.analyzing'));
}

$('.html5_video').bind('click', function () {
    var videoWrap = $(this).parent();
    
    if (videoWrap.hasClass('camera_wrap_small')) {
        switchDetailCamera($('.camera_wrap_small'), videoWrap);
    }
});

$('.capture').bind('click', function () {
    captureTarget($(this).parent());
})

// bind hover a track_target
$('.one_track_target').hover(function () {
    $(this).children('img').css({height: '144px'});
    $(this).children('.delete_one_target').css({
        'transform': 'rotate(45deg)',
        background: 'rgba(0, 0, 0, 0.8)',
        color: '#ecf0f1'
    })
}, function () {
    $(this).children('img').css({height: '80px'});
    $(this).children('.delete_one_target').css({
        'transform': 'rotate(-45deg)',
        background: 'rgba(0, 0, 0, 0)',
        color: 'rgba(0, 0, 0, 0)'
    })
})

// bind delete a track target form track target list
$('.delete_one_target').bind('click', function () {
    
    // TODO, brute force
    $(this).parent().css('display', 'none');
})

function initHistoryListLayout () {
    // video wrap style
    var cameraWrap = $('.camera_wrap');
    cameraWrap.removeClass('camera_wrap_detail');
    cameraWrap.addClass('camera_wrap_small');
    cameraWrap.css({display: 'block'});
    
    // tips style
    var tips = $('.tips');
    tips.removeClass('tips_detail');
    tips.addClass('tips_small');
    tips.css({display: 'block'});
    
    // hidden result windows, add camera container width
    $('.result_container').css({display: 'none'});
    $('.camera_container').css({height: '954px'});
    
    // to update process bar, update time
    var videoList = $('.html5_video');
    for (var i = 0; i < videoList.length; ++i) {
        videoList[i].currentTime += 0.001;
    }
    // repaint progress tarack
    paintProgressTrack(POS);
    // hidden video list function
    $('.video_list_fun').css('display', 'none');
}

$('.video_list_fun').bind('click', function () {
    initHistoryListLayout();
})

function convertCanvasToImage(canvas) {
    var img = new Image();
    img.setAttribute('crossOrigin', 'anonymous');

    img.src = canvas.toDataURL();
    return img;
}

//
// API
//
var PERONSHOT = {
    "entrance": "./imgs/",
    "count": 5,
    "targets": [
        {
            "filename": "target_1.bmp",
            "id": "test1",
            "scale": "3:8"
        },
        {
            "filename": "target_2.bmp",
            "id": "test2",
            "scale": "3:8"
        },
        {
            "filename": "target_3.bmp",
            "id": "test3",
            "scale": "3:8"
        },
        {
            "filename": "target_4.bmp",
            "id": "test4",
            "scale": "3:8"
        },
        {
            "filename": "target_5.bmp",
            "id": "test5",
            "scale": "3:8"
        }
    ]
}

var VIDEOLIST = {
    "entrance": 'C:\\Users\\boby\\Desktop\\video\\',
    "count": 4,
    "targets": [
        {
            "id": 'WP_20151006_20_18_45_Pro.mp4',
            "filename": 'WP_20151006_20_18_45_Pro.mp4',
            "time_range": {
                "start": "2015-10-10 00:10:00",
                "end": "2015-10-10 01:10:10"
            },
        },
        {
            "id": 'WP_20151006_20_19_44_Pro.mp4',
            "filename": 'WP_20151006_20_19_44_Pro.mp4',
            "time_range": {
                "start": "2015-10-10 02:10:00",
                "end": "2015-10-10 03:10:10"
            },
        },
        {
            "id": 'WP_20151006_20_21_14_Pro.mp4',
            "filename": 'WP_20151006_20_21_14_Pro.mp4',
            "time_range": {
                "start": "2015-10-10 04:10:00",
                "end": "2015-10-10 05:10:10"
            },
        },
        {
            "id": 'WP_20151006_20_21_54_Pro.mp4',
            "filename": 'WP_20151006_20_21_54_Pro.mp4',
            "time_range": {
                "start": "2015-10-10 06:10:00",
                "end": "2015-10-10 06:10:10"
            },
        }
    ]
}

var RESULT = {
    "entrance": "./imgs/",
    "count": 3,
    "targets": [
        {
            "filename": "result_1.jpg",
            "rect": "10 10 0 0",
            "frame_name": "frame_id",
            "frame_pos": 450,
            "video": {
                "fps": 30,
                "frames": 800,
                "time_range": {
                    "start_time": "2015-10-10 14:00:00",
                    "end_time": "2015-10-10 14:10:00"
                }
            },
            "camera": {
                "ip": "ip",
                "address": "",
                "index": 0
            }
        },
        {
            "filename": "result_2.jpg",
            "rect": "10 10 0 0",
            "frame_name": "frame_id",
            "frame_pos": 8580,
            "video": {
                "fps": 30,
                "frames": 800,
                "time_range": {
                    "start_time": "2015-10-10 14:00:00",
                    "end_time": "2015-10-10 14:10:00"
                }
            },
            "camera": {
                "ip": "ip",
                "address": "",
                "index": 1
            }
        },
        {
            "filename": "result_3.jpg",
            "rect": "10 10 0 0",
            "frame_name": "frame_id",
            "frame_pos": 14920,
            "video": {
                "fps": 30,
                "frames": 800,
                "time_range": {
                    "start_time": "2015-10-10 14:00:00",
                    "end_time": "2015-10-10 14:10:00"
                }
            },
            "camera": {
                "ip": "ip",
                "address": "",
                "index": 3
            }
        }
    ]
}

// TODO, for realtime
function playTogether(videoList) {
    for (var i = 0; i < videoList.length; ++i) {
        videoList[i].muted = true;
        // form begain
        videoList[i].currentTime = 0;
        changeRealtimeStatu($(videoList[i]).parent(), 'finding', 0)
        $(videoList[i]).trigger('play');
    }
}
$('.html5_realmonitor').bind("timeupdate", function () {

    var currSecond = this.currentTime;
    // console.log(currSecond);
    var tolSecond = timeToSecond('12:00:00') + currSecond;
    // console.log(tolSecond);
    $(this).parent().find('.monitor_data').html('2015-10-13 ' + secondToTime(tolSecond));

})

// TODO, for screenCAP
function getRealtimeVideoList2 () {
    setTimeout(function () {
        console.log('start')
        playTogether($('.html5_realmonitor'));
    }, 0);
    setTimeout(function () {
        changeRealtimeStatu($($('.html5_realmonitor')[0]).parent(), 'got', 5000);
    }, 110000);
}


function jumpVideoPos(currResult) {
    var cArry = currResult.attr('class');
    
    var frame = parseInt(cArry.split('frame')[1].split(' ')[0]);
    var index = parseInt(cArry.split('index')[1].split(' ')[0]);
    
    // get video
    var videoWrapClass = '.camera_wrap_' + index;
    var video = $(videoWrapClass).find('video').get(0);
    
    // show video
    $('.camera_wrap').css({display: 'none'});
    $(videoWrapClass).css({display: 'block'});
    // drag the progress bar
    video.currentTime = frame / FPS;
    
    console.log(video.currentTime);
}

function paintCanvasMask(canvas, posX, posY, width, height) {
    var ctx = canvas.getContext('2d');
    
    ctx.fillStyle = 'rgba(0, 70, 140, 0.6)';
    ctx.fillRect(0, 0, canvas.width, canvas.height);
    ctx.clearRect(posX, posY, width, height);
}

function paintProgressTrack(pos) {
    console.log(pos);
    // before paint, clear all
    $('.track_unit').empty();
    
    for (var i = 0; i < pos.length; ++i) {
        var index = pos[i].index;
        var frame = pos[i].frame_pos;
        
        // get video
        var videoWrapClass = '.camera_wrap_' + index;
        var videoWrap = $(videoWrapClass);
        
        // console.log(videoWrap)
        
        // get video totalTime
        var video = videoWrap.find('.html5_video').get(0);
        var tolTime = video.duration;
        var currTime = frame / FPS;
        var width = videoWrap.find('.progress_bar').width();
        
        var currWidth = width * currTime / tolTime;
        
        console.log(currWidth);

        // get progress bar -> process canvas
        // var canvas = videoWrap.find('.process_canvas').get(0);
        var trackOne = $('<div class="track_one"></div>');
        
        if (videoWrap.hasClass('camera_wrap_detail')) {
            console.log('detail');
            // paintOneTrack(canvas, currWidth, 15, 6);
            trackOne.css({
                width: '12px',
                height: '12px',
                top: '9px',
                left: currWidth - 6,
                "border-radius": 6
            });
        }
        else if (videoWrap.hasClass('camera_wrap_small')) {
            console.log('small')
            // paintOneTrack(canvas, currWidth, 12, 5)
            trackOne.css({
                width: '8px',
                height: '8px',
                top: '8px',
                left: currWidth - 4,
                "border-radius": 4 
            });
        }
        // 
        $(videoWrap).find('.track_unit').append(trackOne);
    }
}

function paintOneTrack(canvas, posX, posY, radius) {
    var ctx = canvas.getContext('2d');
    console.log(ctx)    
    console.log(posX)
    console.log(posY)
    
    ctx.fillStyle = 'rgba(231, 76, 60, 0.8)'
    ctx.fillRect(0, 65, 100, 23)
    ctx.fill();
}

// for real time
$('.monitor_capture').bind('click', function () {
   console.log(this) 
});


// Step 1, request history video list
function getHistoryVideoList(data) {
    
    // if alread has a src, return
    // if ($($('.html5_video')[0]).attr('src') != '') return;

    var d = data;
    var url = ROOT + '/api/gee/videoshots/date:' + d;
    $.ajax({
        type: 'GET',
        url: url
    })
    .done(function (res) {
        console.log(res);
        loadHistoryVideo(res);
    });
}

// Step 1_2, request real-time video list
function getRealtimeVideoList() {
    var url = ROOT + '/api/gee/feeds/';
    $.ajax({
        type: 'GET',
        url: url
    })
    .done(function (res) {
        
    })
}

// Step 2, load history video on the list
function loadHistoryVideo(res) {
    var path = res.entrance;
    var targets = res.targets;
    
    var videoList = $('.html5_video');
    for (var i = 0; i < res.count; ++i) {
        // load video fisrt
        console.log(targets[i].id);
        $(videoList[i]).addClass(targets[i].id);
        
        // TODO
        // $(videoList[i]).attr('src', ROOT + path + targets[i].filename);
        
        // change the camera data
        var startTime = targets[i].time_range.start_time.split(' ')[1];
        var endTime = targets[i].time_range.end_time.split(' ')[1];
        
        // record the start_time and end_time
        $(videoList[i]).parent().find('.curr_time').html(startTime);
        $(videoList[i]).parent().find('.end_time').html(endTime);
        $(videoList[i]).parent().find('.curr_time').addClass('start_time' + startTime);
    }
}

// Step 2_2, load readtime video on the list
function loadRealtimeVideo(res) {
    var path = res.entrance;
    var targets = res.targets;
    
    var videoList = '';
}

// Step 3, capture the target image
function captureTarget(currWindow) {
    
    var cArry = currWindow.attr('class').split(' ');
    var iArry = cArry[1].split('_');
    var index = iArry[1];
    
    // switch to detail page
    var currClass = '.camera_wrap_' + index;
    var currWrap = $(currClass);
    
    // switch to DerailCamera
    switchDetailCamera($('.camera_wrap'), currWrap);
    
    // stop the video
    currWrap.find('.html5_video').trigger('pause');
    currWrap.find('.play_pause').addClass('pause');
    currWrap.find('.play_pause').removeClass('play');
    
    // upload a frame of image
    submitFramePosition($(currWrap).find('.html5_video'));
}

// Step 4, request target person list, For getting all target in this frame
function submitFramePosition(video) {
    var v = video.get(0);
    
    var currTime = v.currentTime;
    var framePos = Math.floor(currTime * FPS);
    
    var vId = video.attr('class').split(' ')[1];
    
    var url = ROOT + '/api/gee/personshots/' + vId +  '/' + framePos + '/';
    
    console.log(url)
    // change the style
    var cArry = video.parent().attr('class').split(' ')[1];
    // console.log(video.attr('class').split(' '))
    var index = cArry.split('_')[2];
    
    var tipClass = '.tips_' + index;
    // $(tipClass).find('.anly_block').css({display: 'block'});
    $(tipClass).find('.analyzing').css({display: 'block'});
    $(tipClass).find('.capture').css({display: 'none'});
    
    // animate
    slideAnalyzing($(tipClass).find('.analyzing'));

    setTimeout(startFlash, 700);
    
    // cann't do anything
    $('.mask').css({display: 'block'});

    console.log(url)
    // sent request
    $.ajax({
        type: 'GET',
        url: url
    })
    .done(function (res) {
        // end flash
        console.log(url);
        endFlash();
        $('.mask').css({display: 'none'});
        showTheTarget($('#container'), res);
    });
}

// Step 5, show all target in this frame 
function showTheTarget(container, res) {
    var path = ROOT + res.entrance;
    var count = res.count;
    var targets = res.targets;
    
    // show choose target page
    $('.choose_target').css({display: 'block'});
    // clear history record
    $('.choose_target').find('#container').empty();
    
    for (var i = 0; i < count; ++i) {
        var liNode = $('<li class="one"></li>');
        liNode.addClass(targets[i].id);
        var img = $('<img >');
        
        img.attr('src', path + targets[i].filename);
        liNode.append(img);
        img.addClass(targets[i].id);
        
        // push the node to container
        container.append(liNode);
    }
    
    // init current target's  pos
    CURR_TARGET = Math.floor(count / 2);
    // add lisentner to left & right event
    bindTargetSwitchEvent(-Math.floor(count / 2) * 30, 0, -count * 30, 30);
    
    // before bind event, need to unbind ex-bind
    $('.one').unbind('click');
    // bind the new a new event
    $('.one').bind('click', function (event) {
        event.stopPropagation();
        
        var target = $(this);
        // console.log(target);
        // console.log($('#container').css('transform'));
        console.log(CURR_TARGET);
        $('.confirm_box').css({display: 'block'});
        
        // before bind event, need to unbind ex-bind
        $('.cancel').unbind('click');
        $('.confirm').unbind('click');
        $('.target_track').unbind('click');
        
        // cancel this capture
        $('.cancel').bind('click', function (event) {
            event.stopPropagation();
            $('.confirm_box').css({display: 'none'});            
        });
        // add this target to history research
        $('.confirm').bind('click', function (event) {
            event.stopPropagation();
            submitTarget(target);
        });
        // add this target to track
        $('.target_track').bind('click', function (evevt) {
            event.stopPropagation();
            console.log('track');
            trackTarget(target);            
        });
    });
}

// Step 6, sumbmit target person, search in history
function submitTarget(target) {
    
    var personShotId = target.find('img').attr('class');    
    
    console.log(personShotId);
    
    // new a target
    var targetImg = $('<img class="target_img">');
    targetImg.attr('src', target.find('img').attr('src'));
    $('.detail_page').append(targetImg);
    
    // change the style
    $('.choose_target').css({display: 'none'});
    
    // the MASK 
    $('.mask').css({display: 'block'});
    // change The Style
    $('.analyzing').css({display: 'none'});
    $('.anly_block').css({display: 'block'});
    
    TRANSCOLOR = self.setInterval(transColor, 1000);
    
    // $.ajax({
    //     type: 'POST',
    //     url: ROOT + '/api/gee/personshots/fake/' + personShotId
    // })
    // .done(function (res) {
    //     // change the style
    //     window.clearInterval(TRANSCOLOR);
    //     $('.mask').css({display: 'none'});
    //     $('.tips').css({display: 'none'});
    //     
    //     // handle result
    //     console.log(res);
    //     handleResult(res);
    // });
    
    // fake 
    setTimeout(function () {
        window.clearInterval(TRANSCOLOR);
        $('.mask').css({display: 'none'});
        $('.tips').css({display: 'none'});
        
        // handle result
        console.log(RESULT);
        handleResult(RESULT);
    });
}

// Step 6_2, track target person, track in real-time monitor
function trackTarget(target) {
    // a new target
    var oneTarget = $('<div class="one_track_target"></div>');
    var targetImg = $('<img class="one_track_img">');
    var deleteTar = $('<div title="删除" class="delete_one_target">十</div>');
    targetImg.attr('src', target.find('img').attr('src'));
    // console.log(target.attr('src'));
    oneTarget.append(targetImg);
    oneTarget.append(deleteTar);
    
    // push to track target list
    $('.track_target_list').append(oneTarget);
    
    // switch function to real-time monitor
    switchFunction($('.realtime_fun'));

    targetImg.css({
        height: '144px'
    });
    targetImg.animate({
        height: '80px'
    }, 200);
    
    targetImg.hover( function () {
        $(this).css({
            height: '144px'
        })
    }, function () {
        $(this).css({
            height: '80px'
        })
    })
    
}

// Step 7, handle all result
function handleResult(res) {
    var path = res.entrance;
    var count = res.count;
    var targets = res.targets;
    POS = [];
    
    var rate = 186 / 720;
    
    for (var i = 0; i < count; ++i) {
        // create a node
        var resultItem = $('<div class="result_item swiper-slide"></div>');
        var resultImg = $('<img class="result_img">');
        var resultMask = document.createElement('canvas');
        $(resultMask).addClass('result_mask');
        
        targets[i].camera.index += 1;
        // store the frame inf and camera inf
        $(resultItem).addClass('frame' + targets[i].frame_pos);
        $(resultItem).addClass('index' + targets[i].camera.index);
        var tempPos = {
            "index": targets[i].camera.index,
            "frame_pos": targets[i].frame_pos
        }
        POS.push(tempPos);
        
        // add attach
        var attach = $('<div class="attach"></div>');
        var att = $('<div class="att"></div>');
        var resultTime = $('<div class="resutl_time"></div>');
        // calculate the date
        var startTime = targets[i].video_shot.time_range.start_time.split(' ');
        var frameTime = secondToTime(timeToSecond(startTime[1]) + targets[i].frame_pos / 30) 
        resultTime.html(startTime[0] + ' ' + frameTime);
        attach.append(att);
        attach.append(resultTime);
        
        // add url to img
        resultImg.attr('src', path + targets[i].filename);
        // paint
        var rect = targets[i].rect.split(' ');
        paintCanvasMask(resultMask, parseInt(rect[0])*rate, parseInt(rect[1])*rate,
                        parseInt(rect[2])*rate, parseInt(rect[3])*rate);
                        
        resultItem.append($(resultMask));
        resultItem.append(resultImg);
        resultItem.append(attach);
                
        // append to all
        $('.result_window_0').find('.result_list').append(resultItem);
        
        // need to repaint
        var copyResult = resultItem.get(0).cloneNode(true);
        var cMask = $(copyResult).find('canvas').get(0);
        paintCanvasMask(cMask, parseInt(rect[0])*rate, parseInt(rect[1])*rate,
                        parseInt(rect[2])*rate, parseInt(rect[3])*rate);

        // append to map
        var index = targets[i].camera.index;
        var resultClass = '.result_window_' + index;
        $(resultClass).find('.result_list').append(copyResult);
        
        // bind progress bar
        resultItem.bind('click', function () {
            jumpVideoPos($(this));
        });
        $(copyResult).bind('click', function () {
            jumpVideoPos($(this));
        })
    }
    
    // paint the process trace
    paintProgressTrack(POS);
}