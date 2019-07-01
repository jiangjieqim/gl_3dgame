/**
 * 获取文字的字节数
 */
function getByteLen(val: string) {
    var len = 0;
    for (var i = 0; i < val.length; i++) {
        var a = val.charAt(i);
        if (a.match(/[^\x00-\xff]/ig) != null) {
            len += 2;
        } else {
            len += 1;
        }
    }
    return len;
}
class Foo extends egret.Sprite {
    constructor(name: string, width: number, height: number) {
        super();
        this.name = name;
        let g = this.graphics;
        g.beginFill(0xff0000, 0.2);
        g.drawRect(0, 0, width * 2, height * 2);
        g.endFill();
        this.touchEnabled = true;
        this.addEventListener(egret.TouchEvent.TOUCH_END, this.onTouchHandle, this);
        //        this.addEventListener(egret.TextEvent.LINK,this.onTouchHandle,this);


        document.addEventListener("keydown", function (evt: KeyboardEvent) {
            // console.log(evt.KeyCode);
            if (evt.keyCode == 13) {
                //回车发送消息
                // this.onSendMsgHandle();
                //dispatch(EventConst.EnterKeyMsg);
            }
        });
    }
    onTouchHandle(e: egret.Event) {
        e.stopPropagation();//阻止向上冒泡
        //        e.stopImmediatePropagation();
        console.log(this.name + " onTouchHandle ");
    }


}
/**
 *
 * @author 
 *
 */
class TestClick extends egret.Sprite {
    public constructor() {
        super();
        let tf: egret.TextField = new egret.TextField();
        tf.touchEnabled = true;
        this.addChild(tf);
        //        tf.textFlow
        tf.textFlow = <Array<egret.ITextElement>>[                  //转换成元素为 egret.ITextElement 的数组
            { text: "ABC", style: { "size": 12 } }       //ts 语法看久了容易混成 C#.. 对象怎么这样? 才想起是 ts 脚本 
            , { text: "测试", style: { "href": "event:text event triggered", "size": 16, "textColor": 0xff0000 } }

        ];

        tf.addEventListener(egret.TextEvent.LINK, this.onLinkHandle, this);
        tf.addEventListener(egret.TouchEvent.TOUCH_END, this.onTouchHandle, this);

        console.log(tf.textWidth, tf.textHeight);

        let b: Foo = new Foo("foo", tf.textWidth, tf.textHeight);
        b.addChild(tf);
        this.addChild(b);
    }
    onTouchHandle(e: egret.TextEvent) {
        e.stopPropagation();
        console.log("tf onTouchHandle " + e.type);
    }

    onLinkHandle(e: egret.Event) {
        //        console.log(e);   
        e.stopPropagation();

        //        let evt: Event = new Event(egret.TouchEvent.TOUCH_END);
        //        evt.stopPropagation();//终止tf的父对像的点击事件

        console.log('tf onLinkHandle ' + e.type);
    }
}
