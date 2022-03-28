# tankgame_cpp

#### 介绍
c++ 写的终端版坦克大战,仅可在windows上编译运行

#### 进度
完成了基本的图形和键盘输入框架,以及炮弹射出动画
#### 编译运行
使用visual stdio 2017编译即可

#### 特色
核心类:
```图形对象Object类```,拥有设置精灵图,调整精灵图方向等方法,其可以重新实现key_event方法来获得输入.

```Engine类```是一个简单的命令行终端图像的小引擎,实现了简单的事件分发机制,其可以接收键盘事件并分发给图形对象;
键盘的事件通过另一个单独的线程进行监听,与主线程通过线程安全的队列相连接,主线程通过队列来获取键盘事件并分发;
实现了一些简单的服务,如查询图形对象，添加图形对象,删除图形对象,阻塞式获取键盘输入等;


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
![showcase](https://github.com/hfh1999/tankgame_cpp_refactoring/blob/master/showcase-back.gif)
