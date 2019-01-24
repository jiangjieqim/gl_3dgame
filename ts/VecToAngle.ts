/**两向量的夹角 */
function getRotateAngle(x1: number, y1: number, x2: number, y2: number) {
    const epsilon = 1.0e-6;
    const nyPI = Math.acos(-1.0);
    let dist: number, dot: number, degree: number, angle: number;

    // normalize
    dist = Math.sqrt(x1 * x1 + y1 * y1);
    x1 /= dist;
    y1 /= dist;
    dist = Math.sqrt(x2 * x2 + y2 * y2);
    x2 /= dist;
    y2 /= dist;
    // dot product
    dot = x1 * x2 + y1 * y2;
    if (Math.abs(dot - 1.0) <= epsilon)
        angle = 0.0;
    else if (Math.abs(dot + 1.0) <= epsilon)
        angle = nyPI;
    else {
        let cross;

        angle = Math.acos(dot);
        //cross product
        cross = x1 * y2 - x2 * y1;
        // vector p2 is clockwise from vector p1 
        // with respect to the origin (0.0)
        if (cross < 0) {
            angle = 2 * nyPI - angle;
        }
    }
    degree = angle * 180.0 / nyPI;
    return degree;
}
/**
 * 求两个向量的单位方向向量
 * s: 起始向量 
 * e: 结束向量
 */
function convertNormalVec(s: egret.Point, e: egret.Point): egret.Point {
    let sub: egret.Point = new egret.Point(e.x - s.x, e.y - s.y)
    let d: number = egret.Point.distance(s, e)//Math.sqrt(e.x)
    sub.x /= d;
    sub.y /= d;
    return sub
}
/**
 * 在起始s坐标指向e坐标,设置对象的rotation值
 * arrow.rotation = vecToAngle(s,e)
 */
export function vecToAngle(s: egret.Point, e: egret.Point): number {
    let p: egret.Point = convertNormalVec(s, e)
    return -getRotateAngle(p.x, p.y, 1, 0)  //1,0代表与x轴水平方向的向量,求得角度取反
}
