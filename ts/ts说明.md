## 添加滤镜
tf.filters = [new egret.GlowFilter(0x4D3C29, 1, 5, 5, 1)];  

## 设置itemRender
```
bag.fla

面板

ui.bag.BagPanel

ui.bosszhijia.BossZJRenderView
export class BossZJRender extends ListItemRenderer<BossZhiJiaCfg , BossZJRenderView>  {
    constructor() {
      super();
      this.skin = BossZJRenderView();
      this._noCheckSame = true;
    }
    bindComponent() {}
    handleView() {}
}
renderlist: PageList<LianXuChongZhiCfg, LxczViewRender>
this.renderlist = new PageList(LxczViewRender, { vgap: 0, staticSize: true, type: ScrollDirection.Vertical, con: list });

export class ZhongLiMediator  extends Mediator {
    afterAllReady(){
    }
}
```
