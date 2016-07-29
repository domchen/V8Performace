var RectangleJS = (function () {
    function RectangleJS(x, y, width, height) {
        if (x === void 0) {
            x = 0;
        }
        if (y === void 0) {
            y = 0;
        }
        if (width === void 0) {
            width = 0;
        }
        if (height === void 0) {
            height = 0;
        }
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }

    RectangleJS.prototype.setTo = function (x, y, width, height) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
        return this;
    };
    RectangleJS.prototype.toString = function () {
        return "(x=" + this.x + ", y=" + this.y + ", width=" + this.width + ", height=" + this.height + ")";
    };
    return RectangleJS;
})();

var MatrixJS = (function () {
    function MatrixJS(a, b, c, d, tx, ty) {
        if (a === void 0) {
            a = 1;
        }
        if (b === void 0) {
            b = 0;
        }
        if (c === void 0) {
            c = 0;
        }
        if (d === void 0) {
            d = 1;
        }
        if (tx === void 0) {
            tx = 0;
        }
        if (ty === void 0) {
            ty = 0;
        }
        this.a = a;
        this.b = b;
        this.c = c;
        this.d = d;
        this.tx = tx;
        this.ty = ty;
    }

    MatrixJS.prototype.setTo = function (a, b, c, d, tx, ty) {
        this.a = a;
        this.b = b;
        this.c = c;
        this.d = d;
        this.tx = tx;
        this.ty = ty;
        return this;
    };
    MatrixJS.prototype.toString = function () {
        return "(a=" + this.a + ", b=" + this.b + ", c=" + this.c + ", d=" + this.d + ", tx=" + this.tx + ", ty=" + this.ty + ")";
    };
    MatrixJS.prototype.transformBounds = function (bounds) {
        var a = this.a;
        var b = this.b;
        var c = this.c;
        var d = this.d;
        var tx = this.tx;
        var ty = this.ty;
        var x = bounds.x;
        var y = bounds.y;
        var xMax = x + bounds.width;
        var yMax = y + bounds.height;
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
        bounds.x = Math.floor(x0 < x2 ? x0 : x2);
        bounds.width = Math.ceil((x1 > x3 ? x1 : x3) - bounds.x);
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
        bounds.y = Math.floor(y0 < y2 ? y0 : y2);
        bounds.height = Math.ceil((y1 > y3 ? y1 : y3) - bounds.y);
    };
    return MatrixJS;
})();

function transformBoundsNoField(x, y, width, height) {
    var a = 2;
    var b = 0;
    var c = 0;
    var d = 0.5;
    var tx = 11;
    var ty = 19;
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
};


var rectJS = new RectangleJS();
var matrixJS = new MatrixJS(2, 0, 0, 0.5, 11, 19);
var rect = new Rectangle();
var matrix = new Matrix(2, 0, 0, 0.5, 11, 19);

var TIMES = 10;
var SIZE = 100000;

var data = [];
var i, t, x, y, width, height, index, start;
for (i = 0; i < SIZE; i++) {
    x = parseInt(Math.random() * 100);
    y = parseInt(Math.random() * 100);
    width = parseInt(Math.random() * 100);
    height = parseInt(Math.random() * 100);
    data.push(x, y, width, height);
}
data = new Int16Array(data);

//JS测试
start = Date.now();
for (t = 0; t < TIMES; t++) {
    index = 0;
    for (i = 0; i < SIZE; i++) {
        x = data[index++];
        y = data[index++];
        width = data[index++];
        height = data[index++];
        rectJS.setTo(x, y, width, height);
        matrixJS.transformBounds(rectJS);
    }
}
var jsTime = Date.now() - start;
console.log("JS耗时:" + jsTime + "ms");

//JS-NoProp测试
start = Date.now();
for (t = 0; t < TIMES; t++) {
    index = 0;
    for (i = 0; i < SIZE; i++) {
        x = data[index++];
        y = data[index++];
        width = data[index++];
        height = data[index++];
        transformBoundsNoField(x, y, width, height);
    }
}
var jsTime = Date.now() - start;
console.log("JS-NoField耗时:" + jsTime + "ms");

//JSBinding测试
start = Date.now();
for (t = 0; t < TIMES; t++) {
    index = 0;
    for (i = 0; i < SIZE; i++) {
        x = data[index++];
        y = data[index++];
        width = data[index++];
        height = data[index++];
        rect.setTo(x, y, width, height);
        matrix.transformBounds(rect);
    }
}
var jsBindingTime = Date.now() - start;
console.log("JSBinding耗时:" + jsBindingTime + "ms");

//Native测试
runtNativeTest(data, TIMES);



