
let tf: egret.TextField = new egret.TextField();
let spr: egret.Sprite;
/**
 * 回车锁定输入框
 * a键解锁输入框
 */
class InputForce extends egret.Sprite {

    constructor() {
        super();
        spr = this;
        tf.type = egret.TextFieldType.INPUT;
        tf.touchEnabled = true;
        this.addChild(tf);

        this.graphics.lineStyle(1, 0xff0000);
        this.graphics.drawRect(0, 0, tf.width, tf.height);
        this.graphics.endFill();

        tf.text = "1";
 
        document.addEventListener("keydown", function (evt: KeyboardEvent) {
            console.log(evt.keyCode);

            if (evt.keyCode == 13) {
                //回车发送消息
                tf.type = egret.TextFieldType.DYNAMIC;

            }else if(evt.keyCode == 65){
                //key `a`
                tf.type = egret.TextFieldType.INPUT;

            }
        });

    }
}