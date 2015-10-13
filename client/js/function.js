// For transfer current time to the second of the same day
// para:   time (string eg. 00:00:00)
// return: second (number eg. 23423)
function timeToSecond(_time) {
    var time = _time.replace(/\s+/g, "");
    var tArry = time.split(':');
    
    var hour = parseInt(tArry[0]);
    var minute = parseInt(tArry[1]);
    var second = parseInt(tArry[2]);
    
    var tolSecond = hour * 3600 + minute * 60 + second;
    
    return tolSecond;
}

// For transfer a second to the data of the same day
// para:   second (number eg. 324234)
// return: time (string eg/ 00:00:00)
function secondToTime(_second) {
    var tolSecond = Math.floor(_second);
    
    var hour = Math.floor(tolSecond / 3600);
    var minute = Math.floor((tolSecond % 3600) / 60);
    var second = tolSecond % 60;
    
    var sHour = hour + '';
    var sMinute = minute + '';
    var sSecond = second + '';
    
    if (hour < 10) sHour = '0' + sHour;
    if (minute < 10) sMinute = '0' + sMinute;
    if (second < 10) sSecond = '0' + sSecond;
    
    var time = sHour + ':' + sMinute + ':' + sSecond;
    
    return time;
}