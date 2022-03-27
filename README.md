# tankgame_cpp

#### 介绍
c++ 写的终端版坦克大战,仅可在windows上编译运行

#### 进度
完成了基本的图形和键盘输入框架,以及炮弹射出动画
#### 编译运行
使用visual stdio 2017编译即可

#### 后续计划
1. 添加炮弹毁伤效果
2. 添加更复杂的敌人ai
3. 添加墙体等障碍物
4. 增加开始，结束界面，菜单界面


#### 演示

通过在代码中增加主角坦克和敌人

其中主角坦克通过按键控制:

目前支持五个按键，键盘上的↑ ↓ ← →来移动主角坦克,空格键来进行射击.

敌人在左上角用简单的逻辑转一个矩形圈.

```
	/* 初始化场景的代码 */
	Engine e;
	e.set_fresh_time(25);
	auto tank = std::unique_ptr<Object>(new mytank("1")); // 创建一物品,其使用编号1,为受控坦克.
	tank->set_draw("tank.draw");// 设置其精灵图为 tank.draw
	auto enemy = std::unique_ptr<Object>(new enemy_tank("2"));// 创建一物品,其使用编号2,为敌军坦克.
	enemy->set_visualable();
	enemy->set_draw("tank.draw");
	e.add_obj(std::move(tank));//往场景中插入1号物品.
	e.add_obj(std::move(enemy));//往场景中加入2号物品.
	e.run();//运行
```

