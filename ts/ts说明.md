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
    
    $setSelected(value: boolean) {
        super.$setSelected(value);
        this.skin.selectMC.visible = value;//设置选择状态
    }
}
renderlist: PageList<LianXuChongZhiCfg, LxczViewRender>
this.renderlist = new PageList(LxczViewRender, { vgap: 0, staticSize: true, type: ScrollDirection.Vertical, con: list });

export class ZhongLiMediator  extends Mediator {
    afterAllReady(){
    }
}
```

## 设置不可滚动的列表
```
petPageTurning: PageTurning<ChongWuBiaoCfg, PetHeadRender>;
this.petPageTurning = getSizedPageTurning<ChongWuBiaoCfg, PetHeadRender>(PetHeadRender, { showCount: 4, type: ScrollDirection.Horizon, con: listSize, prevBtn: leftBtn, nextBtn: rightBtn, hgap: -4, vgap: 1 });
```

## 根据缩放比设置坐标
代码片段  
```
    let stage = egret.sys.$TempStage;
    let {txtInputContext} = this;
    let sw = stage.stageWidth, sh = stage.stageHeight;
    let base: BasefaceMediator = (facade.getMediator(ModuleId.BaseFace) as BasefaceMediator)
    let view = base.$view;
    let {bg} = this.$view;
    let percent: number = view.$getConcatenatedMatrix().a;
    realSW = sw / percent;
    realSH = sh / percent;
    bg.width = realSW - CHAT_GAP;
    this.refreshWidth();
```
