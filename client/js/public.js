var SLIDE;

function changeWidth(container, width, num) {
    $(container).css({
        width: width * num
    })
}
function slideAnalyzing() {
    var canvas = document.getElementById('analyzing');
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

setTimeout(slideAnalyzing, 1000);

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

$(function () {
    var currDeg = 0;
    var maxDeg = 0;
    var minDeg = -80;
    
    $('.left_turn').bind('click', function () {
        if (currDeg >= maxDeg) return;
        currDeg += 20;
        transform($('#container')[0], "rotateY(" + currDeg + "deg)");
    })
    
    $('.right_turn').bind('click', function () {
        if (currDeg <= minDeg) return;
        currDeg -= 20;
        transform($('#container')[0], "rotateY(" + currDeg + "deg)");
    })
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

var OPPOSITE = {
    realtime_fun: 'reltime_monitor',
    history_fun: 'detail_page'
}


// addListen to switch function
$('.monitor_nav').bind('click', function () {
    console.log($(this));
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
    
    $('.one_camera').css({
        background: 'rgba(0, 0, 0, 0)',
        color: '#2980b9'
    });
    $('.one_camera').removeClass('active');
    
    currWindow.css({
        background: '#3498db',
        color: '#062331'
    });
    currWindow.addClass('active');
    
    // bottom map to window
    var cArry = currWindow.attr('class').split(' ');
    var iArry = cArry[1].split('_');
    var index = iArry[2];
    
    var windowClassName = 'result_window_' + index;
    
    // handle all
    $('.result_window').css({display: 'none'});
    $('.' + windowClassName).css({display: 'block'});
    
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
    }
    else if ($(this).hasClass('play')) {
        video.trigger("pause");
        $(this).removeClass('play');
        $(this).addClass('pause');
    }
});

$('.speedup').bind('click', function () {
    var rate = $(this).parent().prev().get(0).playbackRate % 3;
    $(this).parent().prev().get(0).playbackRate = rate + 1;
    console.log($(this).parent().prev().get(0).playbackRate);
})


$('.process').bind('click', function (event) {
    var x = event.offsetX;
    
    // there has some bug with siblings
    $(this).parent().children('.curr_process').css({width: x});
    // the track
    $(this).siblings('.track_point').css({left: x - 8});
    
    console.log($(this).parent().parent().prev());
    // refresh time
    var tolTime = $(this).parent().parent().prev().get(0).duration;
    var tolProcessWidth = $(this).parent().children('.tol_process').width();
    $(this).parent().parent().prev().get(0).currentTime = (x / tolProcessWidth) * tolTime;
    
    console.log((x / tolProcessWidth) * tolTime);
    console.log(tolTime);
    console.log(tolProcessWidth);
})

// process bar
$('.html5_video').get(0).addEventListener("timeupdate", function () {
    var totalTime = this.duration;
    var tolProcessWidth = $(this).parent().find('.tol_process').width();
    
    var curProcess = (this.currentTime / totalTime) * tolProcessWidth;
     
    // change the style
    $(this).parent().find('.curr_process').css({width: curProcess});
    $(this).parent().find('.track_point').css({left: curProcess - 8});
    
    if (curProcess == tolProcessWidth) {
        $(this).parent().find('.play_pause').addClass('pause');
        $(this).parent().find('.play_pause').removeClass('play');
    }
}, false);
