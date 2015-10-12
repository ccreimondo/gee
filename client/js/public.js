var SLIDE;
var TRANSCOLOR;
var ROOT = 'http://10.250.94.25:5000';
var FPS = 30;
var POS = [
    {
        "index": 3,
        "frame_pos": 300
    },
    {
        "index": 3,
        "frame_pos": 600
    }
]


function changeWidth(container, width, num) {
    $(container).css({
        width: width * num
    })
}
function slideAnalyzing(anly) {
    console.log(anly)
    var canvas = anly.get(0);
    ctx = canvas.getContext('2d');
    
    var img = new Image();
    img.src = './imgs/analyzing.png';
    
    var k_left = new Image();
    k_left.src = './imgs/k_left.png';
    
    var k_right = new Image();
    k_right.src = './imgs/k_right.png';
    
    img.onload = function() {
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
        
        ctx.drawImage(img, posX, posY);
        ctx.clearRect(0, 0, w1, height);
        ctx.clearRect(w2, 0, width, height);
        
        ctx.drawImage(k_left, w1 - interval, 0);
        ctx.drawImage(k_right, w2 + interval, 0)
        
        if (w1 < 0) {
            ctx.clearRect(0, 0, width, height);
            ctx.drawImage(img, posX, posY);
            ctx.drawImage(k_left, 0, 0);
            ctx.drawImage(k_right, 316, 0)
            
            window.clearInterval(SLIDE);
        }
        w1 -= 4;
        w2 += 4;
    }
}

// setTimeout(slideAnalyzing, 1000);

// setInterval(transColor, 1000);

function transColor() {
    $('.anly').animate({
        'background-color': 'rgba(236, 240, 241, 0)',
        'color': 'rgba(231, 76, 60, 0.8)'
    },  500,
    function () {
        $('.anly').animate({
        'background-color': 'rgba(231, 76, 60, 0.8)',
        'color': 'rgb(236, 240, 241)'
        }, 500);
    })
}

function bindTargetSwitchEvent(curr, max, min, interval) {
    var currDeg = curr;
    var maxDeg = max;
    var minDeg = min + interval;
    
    $('.left_turn').bind('click', function (evevt) {
        // console.log('left')
        if (currDeg >= maxDeg) return;
        currDeg += interval;
        transform($('#container')[0], "rotateY(" + currDeg + "deg)");
        
        // stop the bubbling event
        event.stopPropagation();
    })
    
    $('.right_turn').bind('click', function (event) {
        // console.log('right')
        if (currDeg <= minDeg) return;
        currDeg -= interval;
        transform($('#container')[0], "rotateY(" + currDeg + "deg)");
        
        // stop the bubbling event
        event.stopPropagation();
    })
}

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

var OPPOSITE = {
    realtime_fun: 'reltime_monitor',
    history_fun: 'detail_page'
}


// addListen to switch function
$('.monitor_nav').bind('click', function () {
    // console.log($(this));
    switchFunction($(this));
})

function switchFunction(currFun) {

    // If in the current function
    if (currFun.hasClass('active')) return;
    
    // switch
    if (currFun.hasClass('realtime_fun')) {
        $('.detail_page').css({display: 'none'});
        $('.history_fun').removeClass('active');
        
        $('.realtime_monitor').css({display: 'block'});
        $('.realtime_fun').addClass('active');  
        
        // switch the background
        $('.nav').css({background: 'url("./imgs/realtime_fun.png")'});
    }
    else if (currFun.hasClass('history_fun')) {
        $('.realtime_monitor').css({display: 'none'});
        $('.realtime_fun').removeClass('active');
        
        $('.detail_page').css({display: 'block'});
        $('.history_fun').addClass('active');
        
        // switch the background
        $('.nav').css({background: 'url("./imgs/history_fun.png")'});
    }
}

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
    
    // band swiper
    var swiper = new Swiper('.swiper-container', {
        scrollbar: '.swiper-scrollbar',
        scrollbarHide: true,
        slidesPerView: 'auto',
        centeredSlides: false,
        spaceBetween: 30,
        grabCursor: true
    });
    console.log($(windowClassName))
}


//反射調用
var invokeFieldOrMethod = function(element, method) 
{
   var usablePrefixMethod;
   ["webkit", "moz", "ms", "o", ""].forEach(function(prefix) {
       if (usablePrefixMethod) return;
       if (prefix === "") {
           // 无前缀，方法首字母小写
           method = method.slice(0,1).toLowerCase() + method.slice(1);   
       }
       var typePrefixMethod = typeof element[prefix + method];
       if (typePrefixMethod + "" !== "undefined") {
           if (typePrefixMethod === "function") {
               usablePrefixMethod = element[prefix + method]();
           } else {
               usablePrefixMethod = element[prefix + method];
           }
       }
   });
    
       return usablePrefixMethod;
};
    
//進入全屏
 function launchFullscreen(element) 
   {
    //此方法不可以在異步任務中執行，否則火狐無法全屏
     if(element.requestFullscreen) {
       element.requestFullscreen();
     } else if(element.mozRequestFullScreen) {
       element.mozRequestFullScreen();
     } else if(element.msRequestFullscreen){ 
       element.msRequestFullscreen();  
     } else if(element.oRequestFullscreen){
        element.oRequestFullscreen();
    }
    else if(element.webkitRequestFullscreen)
     {
       element.webkitRequestFullScreen();
     }else{
      
        var docHtml  = document.documentElement;
        var docBody  = document.body;
        var videobox  = document.getElementById('videobox');
        var  cssText = 'width:100%;height:100%;overflow:hidden;';
        docHtml.style.cssText = cssText;
        docBody.style.cssText = cssText;
        videobox.style.cssText = cssText+';'+'margin:0px;padding:0px;';
        document.IsFullScreen = true;
 
      }
   }
//退出全屏
   function exitFullscreen()
   {
       if (document.exitFullscreen) {
         document.exitFullscreen();
       } else if (document.msExitFullscreen) {
         document.msExitFullscreen();
       } else if (document.mozCancelFullScreen) {
         document.mozCancelFullScreen();
       } else if(document.oRequestFullscreen){
            document.oCancelFullScreen();
        }else if (document.webkitExitFullscreen){
         document.webkitExitFullscreen();
       }else{
        var docHtml  = document.documentElement;
        var docBody  = document.body;
        var videobox  = document.getElementById('videobox');
        docHtml.style.cssText = "";
        docBody.style.cssText = "";
        videobox.style.cssText = "";
        document.IsFullScreen = false;
    }
  }
  
// document.getElementById('fullScreenBtn').addEventListener('click',function(){
//     launchFullscreen(document.getElementById('video')); 
//     window.setTimeout(function exit(){
//     //檢查瀏覽器是否處於全屏
//     if(invokeFieldOrMethod(document,'FullScreen') || invokeFieldOrMethod(document,'IsFullScreen') || document.IsFullScreen)
//     {
//     exitFullscreen();
//     }
//    },5*1000);
// },false);

// 启动全屏!
// $('body').click(function (){
//     launchFullscreen(document.getElementById('aa')); // 整个网页
// })


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
    console.log($(this).parent().prev().get(0).playbackRate);
});

$('.maxum').bind('click', function () {
    // get video
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
    console.log($(this).parent().parent().prev().get(0))
    var tolTime = $(this).parent().parent().prev().get(0).duration;
    var tolProcessWidth = $(this).parent().children('.tol_process').width();
    $(this).parent().parent().prev().get(0).currentTime = (x / tolProcessWidth) * tolTime;
    
    console.log((x / tolProcessWidth) * tolTime);
    console.log(tolTime);
    console.log(tolProcessWidth);
})

// process bar
for (var i = 0; i < $('.html5_video').length; ++i) {
    $('.html5_video').get(i).addEventListener("timeupdate", function () {
        var totalTime = this.duration;
        var tolProcessWidth = $(this).parent().find('.tol_process').width();
        
        var curProcess = (this.currentTime / totalTime) * tolProcessWidth;
        
        // change the style
        $(this).parent().find('.curr_process').css({width: curProcess});
        $(this).parent().find('.track_point').css({left: curProcess - 8});
        
        if (curProcess == tolProcessWidth) {
            console.log('a')
            $(this).parent().find('.play_pause').addClass('pause');
            $(this).parent().find('.play_pause').removeClass('play');
        }
    }, false);
}


function switchDetailCamera(allCam, currCam) {
    
    // TODO
    handleResult(RESULT);
    
    
    
    // when touch again, return
    if (currCam.hasClass('camera_wrap_detail')) return;
    
    // get curr video num
    var cArry = currCam.attr('class').split(' ');
    var iArry = cArry[1].split('_');
    var i = iArry[2];
    
    // remove abslute position first
    // ["1", "2", "3", "4"].forEach(function (index)  
    // {  
    //     allCam.removeClass('camera_wrap_' + index);
    // });
    
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
    
    // ["1", "2", "3", "4"].forEach(function (index)  
    // {  
    //     currCam.parent().find('.tips').removeClass('tips_' + index);
    // });
    
    // band swiper
    var swiper = new Swiper('.swiper-container', {
        scrollbar: '.swiper-scrollbar',
        scrollbarHide: true,
        slidesPerView: 'auto',
        centeredSlides: false,
        spaceBetween: 30,
        grabCursor: true
    });
    
    paintProgressTrack(POS);
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
    
    // upload a imge with target
    // uploadImg(currWrap.find('.html5_video'));
    // change method, post the frame position
    submiteFramePosition($(currWrap).find('.html5_video'));
}

function initHistoryListLayout () {
    // video wrap style
    var cameraWrap = $('.camera_wrap');
    cameraWrap.removeClass('camera_wrap_detail');
    cameraWrap.addClass('camera_wrap_small');
    cameraWrap.css({display: 'block'});
    
    // tips style
    var tips = $('.tips')
    tips.removeClass('tips_detail');
    tips.addClass('tips_small');
    tips.css({display: 'block'});
    
    // abslute position
    // ["1", "2", "3", "4"].forEach(function (index) {
    //     var c1 = 'camera_wrap_' + index;
    //     $(cameraWrap[index - 1]).addClass(c1);
    //     
    //     var c2 = 'tips_' + index;
    //     $(tips[index - 1]).addClass(c2);
    // });
    
    // hidden result windows, add camera container width
    $('.result_container').css({display: 'none'});
    $('.camera_container').css({height: '954px'});
    
    paintProgressTrack(POS);
}
initHistoryListLayout();
$('.head').bind('click', function () {
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

// For get all target in this frame
function submiteFramePosition(video) {
    var v = video.get(0);
    
    var currTime = v.currentTime;
    var framePos = Math.floor(currTime * FPS);
    
    var vId = video.attr('class').split(' ')[1];
    
    var url = ROOT + '/api/gee/personshots/vid:' + vId + '/' + framePos;
    
    // sent request
    // $.ajax({
    //     type: 'GET',
    //     url: url
    // })
    // .done(function (res) {
    //     console.log(res);
    // })
    
    // change the style
    var cArry = video.parent().attr('class').split(' ')[1];
    // console.log(video.attr('class').split(' '))
    var index = cArry.split('_')[2];
    
    var tipClass = '.tips_' + index;
    // $(tipClass).find('.anly_block').css({display: 'block'});
    $(tipClass).find('.analyzing').css({display: 'block'});
    $(tipClass).find('.capture').css({display: 'none'});
    
    slideAnalyzing($(tipClass).find('.analyzing'));

    
    setTimeout(function () {
        showTheTarget($('#container'), PERONSHOT);
    }, 3000);
    
}

function showTheTarget(container, res) {
    var path = res.entrance;
    var count = res.count;
    var targets = res.targets;
    
    $('.choose_target').css({display: 'block'});
    
    for (var i = 0; i < count; ++i) {
        var liNode = $('<li class="one"></li>');
        liNode.addClass(targets[i].id);
        var img = $('<img >');
        
        img.attr('src', path + targets[i].filename);
        liNode.append(img);
        
        // push the node to container
        container.append(liNode);
    }
    
    // add lisentner to left & right event
    bindTargetSwitchEvent(0, 0, -count * 30, 30);
    // bind the new a new event
    $('.one').bind('click', function () {
        console.log('a')
        var target = $(this);
        
        $('.confirm_box').css({display: 'block'});
        
        $('.cancel').bind('click', function () {
            $('.confirm_box').css({display: 'none'});
        })
        $('.confirm').bind('click', function () {
            console.log('b')
            submiteTarget(target);
        })
        
    });
}



function submiteTarget(tarNode) {
    var personShotId = tarNode.attr('class').split(' ')[1];
    
    console.log(personShotId);
    
    // change the style
    $('.choose_target').css({display: 'none'});
    
    // the MASK 
    $('.mask').css({display: 'block'});
    // change The Style
    $('.analyzing').css({display: 'none'});
    $('.anly_block').css({display: 'block'});
    TRANSCOLOR = self.setInterval(transColor, 1000);
    // $.ajax({
    //     type: 'GET',
    //     url: ROOT + '/api/gee/personshots/' + personShotId
    // })
    // .done(function (res) {
    //     console.log(res);
    // })
    setTimeout(function () {
        window.clearInterval(TRANSCOLOR);
        $('.mask').css({display: 'none'});
        $('.tips').css({display: 'none'});
    }, 3000)
}

function getHistoryVideoList(data) {
    var d = data;
    var url = ROOT + '/api/gee/videoshots/date:' + d;
    $.ajax({
        type: 'GET',
        url: url
         
    })
    .done(function (res) {
        console.log(res);
        loadVideo(res);
    })
}
// getHistoryVideoList('20131801');


function loadVideo(res) {
    var path = res.entrance;
    var targets = res.targets;
    
    var videoList = $('.html5_video');
    for (var i = 0; i < res.count; ++i) {
        console.log(targets[i].id);
        // $(videoList[i]).addClass(targets[i].id);
        $(videoList[i]).attr('src', path + targets[i].filename);
    }
}


var RES = {
    "entrance": 'C:\\Users\\boby\\Desktop\\video\\',
    "count": 4,
    "targets": [
        {
            "id": 'WP_20151006_20_18_45_Pro.mp4'
        },
        {
            "id": 'WP_20151006_20_19_44_Pro.mp4'
        },
        {
            "id": 'WP_20151006_20_21_14_Pro.mp4'
        },
        {
            "id": 'WP_20151006_20_21_54_Pro.mp4'
        }
    ]
}
// loadVideo(RES);

var RESULT = {
    "entrance": "./imgs/",
    "count": 3,
    "targets": [
        {
            "filename": "result3.png",
            "rect": "10 10 20 20",
            "frame_name": "frame_id",
            "frame_pos": 10,
            "video": {
                "fps": 30,
                "frames": 800,
                "time_range": {
                    "start": "2015-10-10 22:10:00",
                    "end": "2015-10-10 22:10:10"
                }
            },
            "camera": {
                "ip": "ip",
                "address": "",
                "index": 1
            }
        },
        {
            "filename": "result3.png",
            "rect": "10 10 20 20",
            "frame_name": "frame_id",
            "frame_pos": 800,
            "video": {
                "fps": 30,
                "frames": 800,
                "time_range": {
                    "start": "2015-10-10 22:10:00",
                    "end": "2015-10-10 22:10:10"
                }
            },
            "camera": {
                "ip": "ip",
                "address": "",
                "index": 2
            }
        },
        {
            "filename": "result3.png",
            "rect": "10 10 20 20",
            "frame_name": "frame_id",
            "frame_pos": 300,
            "video": {
                "fps": 30,
                "frames": 800,
                "time_range": {
                    "start": "2015-10-10 22:10:00",
                    "end": "2015-10-10 22:10:10"
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

function handleResult(res) {
    var path = res.entrance;
    var count = res.count;
    var targets = res.targets;
    var progressPos = [];
    
    var rate = 186 / 720;
    
    for (var i = 0; i < count; ++i) {
        // create a node
        var resultItem = $('<div class="result_item swiper-slide"></div>');
        var resultImg = $('<img class="result_img">');
        var resultMask = document.createElement('canvas');
        $(resultMask).addClass('result_mask');
        
        // store the frame inf and camera inf
        $(resultItem).addClass('frame' + targets[i].frame_pos);
        $(resultItem).addClass('index' + targets[i].camera.index);
        var tempPos = {
            "index": targets[i].camera.index,
            "frame_pos": targets[i].frame_pos
        }
        progressPos.push(tempPos);
        
        // add url to img
        resultImg.attr('src', path + targets[i].filename);
        // paint
        var rect = targets[i].rect.split(' ');
        paintCanvasMask(resultMask, parseInt(rect[0])*rate, parseInt(rect[1])*rate,
                        parseInt(rect[2])*rate, parseInt(rect[3])*rate);
                        
        resultItem.append($(resultMask));
        resultItem.append(resultImg);
                
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
    
    console.log(progressPos);
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
    
}

function paintCanvasMask(canvas, posX, posY, width, height) {
    var ctx = canvas.getContext('2d');
    
    ctx.fillStyle = 'rgba(0, 70, 140, 0.6)';
    ctx.fillRect(0, 0, canvas.width, canvas.height);
    ctx.clearRect(posX, posY, width, height);
}


// setTimeout(function () {
//     paintProgressTrack(POS);
// }, 1000)

function paintProgressTrack(pos) {
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
    // ctx.beginPath();
    // ctx.moveTo(posX, posY);
    // ctx.arc(posX, 78, 20, 0, Math.PI * 2, true);
    // ctx.closePath();
    ctx.fill();
}

setTimeout(function () {
    // loadVideo(RES);
}, 3000)