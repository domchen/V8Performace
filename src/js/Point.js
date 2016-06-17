var PointJS = (function () {
    function PointJS(x, y) {
        if (x === void 0) {
            x = 0;
        }
        if (y === void 0) {
            y = 0;
        }
        this.x = x;
        this.y = y;
    }

    PointJS.prototype.setTo = function (x, y, width, height) {
        var a = 2;
        var b = 0;
        var c = 0;
        var d = 0.5;
        var tx = 100;
        var ty = 50;
        var xMax = x + width;
        var yMax = y + height;
        var x0 = a * x + c * y + tx;
        var y0 = b * x + d * y + ty;
        var x1 = a * xMax + c * y + tx;
        var y1 = b * xMax + d * y + ty;
        var x2 = a * xMax + c * yMax + tx;
        var y2 = b * xMax + d * yMax + ty;
        var x3 = a * x + c * yMax + tx;
        var y3 = b * x + d * yMax + ty;
        var tmp = 0;
        if (x0 > x1) {
            tmp = x0;
            x0 = x1;
            x1 = tmp;
        }
        if (x2 > x3) {
            tmp = x2;
            x2 = x3;
            x3 = tmp;
        }
        x = Math.floor(x0 < x2 ? x0 : x2);
        width = Math.ceil((x1 > x3 ? x1 : x3) - x);
        if (y0 > y1) {
            tmp = y0;
            y0 = y1;
            y1 = tmp;
        }
        if (y2 > y3) {
            tmp = y2;
            y2 = y3;
            y3 = tmp;
        }
        y = Math.floor(y0 < y2 ? y0 : y2);
        height = Math.ceil((y1 > y3 ? y1 : y3) - y);
        this.x = width;
        this.y = height;
    };
    return PointJS;
})();
var times = 1000;
var size = 1000;
var data = [];
for(var i=0;i<size;i++){
    var x = parseInt(Math.random()*100);
    var y = parseInt(Math.random()*100);
    var width = parseInt(Math.random()*100);
    var height = parseInt(Math.random()*100);
    data.push(x,y,width,height);
}
data = new Int16Array(data);
var index = 0;
var pointJS = new PointJS();
var start = Date.now();
for(var j=0;j<times;j++){
    index = 0;
    for(var i=0;i<size;i++){
        var x = data[index++];
        var y = data[index++];
        var width = data[index++];
        var height = data[index++];
        pointJS.setTo(x,y,width,height,j);
    }
}
var jsTime = Date.now()-start;

var point = new Point();
start = Date.now();
for(var j=0;j<times;j++){
    index = 0;
    for(var i=0;i<size;i++){
        var x = data[index++];
        var y = data[index++];
        var width = data[index++];
        var height = data[index++];
        point.setTo(x,y,width,height,j);
    }
}
var nativeTime = Date.now()-start;

start = Date.now();
for(var j=0;j<times;j++) {
    point.setTo2(data);
}
var batchTime = Date.now()-start;

start = Date.now();
for(var j=0;j<times;j++){
    for(var i=0;i<size;i++){
        point.setTo3(data,i,j);
    }
}
var typedTime = Date.now()-start;

"JS:"+jsTime+"ms\nNativeJS:"+nativeTime+"ms\nNativeJS-Batch:"+batchTime+"ms\nNativeJS-Array&Index:"+typedTime+"ms";