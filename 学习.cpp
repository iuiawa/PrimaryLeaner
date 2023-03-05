#include <stdio.h>
#include <easyx.h>
#include <conio.h>
#include <iostream>
using namespace std;

//创建图片变量（全局变量）
IMAGE 图片;

/*role:玩家*/

IMAGE Img_role[2];
IMAGE Img_bullet[2];
IMAGE ENEMI_A[2][2];

int GameTicks = 0;
int LastBullTick = 0;

//防止以后的变量冲突，加上自定义类型
enum Window_Settings
{
    WIDTH = 640,//窗口宽度
    HEIGHT = 640,//窗口高度
    MAX_PLAYER_LEFTH = -30,//最大玩家可达到的左侧位置x
    MAX_PLAYER_HIGTH = -30,//最大玩家可达到的高度y
    MAX_PLAYER_Y_DOWN = HEIGHT - 30,//最大玩家可到达的最低点
    MAX_PLAYER_X_RIGHT = WIDTH - 30,//最大玩家可达到的最右边
    max_bullet_num = 114,//最大子弹数目
    Bullet_Speed = 15,//子弹飞行速度

    MAX_ENEMI_NUM = 50,//最大敌人数目
    BIG_PLANE = 1,//大飞机状态TYPE值，不用修改
    SMALL_PLANE = 0,//小飞机状态TYPE值
    BIG_PLANE_DEFAULT_HP = 100,//大飞机默认HP
    SMALL_PLANE_DEFAULT_HP = 10,//小飞机默认HP
    BIG_PLANE_IMAGE_WIDTH=50,//大飞机碰撞箱与图像宽度
    SMALL_PLANE_IMAGE_WIDTH=25,//小飞机碰撞箱与图像宽度
    BIG_PLANE_IMAGE_HEIGHT = 50,//大飞机碰撞箱与图像高度
    SMALL_PLANE_IMAGE_HEIGHT = 25,//小飞机碰撞箱与图像高度
};

//实体数据结构体
struct Enities
{
    int x = 0;//玩家的x坐标
    int y=0;//玩家的y坐标
    bool alive=true;//玩家是死是活
    int speed_PX = 5;
    int health = 114;
    int Width = 50;//宽度（碰撞箱px）
    int Height = 50;//高度（碰撞箱px）
    int type = 0;//类型

};
Enities player;
Enities bull[max_bullet_num];
Enities enemies[MAX_ENEMI_NUM];

struct Keys
{
    //虚拟键码可以在WindowsAPI查找 网址：https://learn.microsoft.com/zh-cn/windows/win32/inputdev/virtual-key-codes
    char w = 'w';
    char a = 'a';

}Key;



//启动时的LoadImage
void Startup_LoadImage() {
    cout << "开始加载图片\n";
    //加载变量内容并且resize为窗口大小
    loadimage(&图片, "Image.png", WIDTH, HEIGHT, true);
    
    //加载玩家图片
    loadimage(&Img_role[0], "images\\player_a.png");
    loadimage(&Img_role[1], "images\\player_b.jpg");
    /*0为主要图像，1为橡皮擦*/

    loadimage(&Img_bullet[0], "images\\bullet_a.png");
    loadimage(&Img_bullet[1], "images\\bullet_b.jpg");


    //加载敌机图片
    loadimage(&ENEMI_A[0][0], "images\\Flight_Chicken.jpg");
    loadimage(&ENEMI_A[0][1], "images\\Flight_Chicken.png");
    loadimage(&ENEMI_A[1][0], "images\\Flight_Chicken_SMALL.jpg");
    loadimage(&ENEMI_A[1][1], "images\\Flight_Chicken_SMALL.png");

    //在(0 ,0)坐标显示图片
    putimage(0, 0, &图片);
    
    cout << "加载结束！\n";
}
//生成大飞机和小飞机的函数在这里！！！
void enemyhelth(int i) {
    if (rand()%100 < 10)
    {
        enemies[i].type = BIG_PLANE;
        enemies[i].health = BIG_PLANE_DEFAULT_HP;
        enemies[i].Width = BIG_PLANE_IMAGE_WIDTH;
        enemies[i].Height = BIG_PLANE_IMAGE_HEIGHT;
    }
    else {
        enemies[i].type = SMALL_PLANE;
        enemies[i].health = SMALL_PLANE_DEFAULT_HP;
        enemies[i].Width = SMALL_PLANE_IMAGE_WIDTH;
        enemies[i].Height = SMALL_PLANE_IMAGE_HEIGHT;
    }

}
void GameStartUp() {
    //创建窗口，并且设置为显示控制台
    initgraph(WIDTH, HEIGHT, EX_SHOWCONSOLE);

    //初始化玩家
    player.x = WIDTH/2.25;
    player.y = HEIGHT-75;

    //初始化子弹
    for (int i = 0; i < max_bullet_num; i++)
    {
        bull[i].x = bull[i].y = -114;
        bull[i].alive = false;
        //让所有子弹状态设置为挂掉了[不存在]
    }

    //初始化敌机
    for (int i = 0; i < MAX_ENEMI_NUM; i++)
    {
        enemies[i].alive = false;
        enemyhelth(i);//这行代码不仅用来设置血量，也用来设置飞机状态
    }
}

//创建子弹
void create_bullet() {
    for (int i = 0; i < max_bullet_num && LastBullTick < GameTicks; i++)
    {
        if (!bull[i].alive)
        {
            bull[i].x = player.x + 34;
            bull[i].y = player.y;
            bull[i].alive = true;

            LastBullTick = GameTicks + 5;
            break;
        }

    }
}

void GameDraw() {
    //绘制背景、玩家（玩家透明背景）
    putimage(0, 0, &图片);
    putimage(player.x, player.y, &Img_role[1], NOTSRCERASE);
    putimage(player.x, player.y, &Img_role[0], SRCINVERT);

    //绘制子弹
    for (int i = 0; i < max_bullet_num; i++)
    {
        if (bull[i].alive) {
            putimage(bull[i].x, bull[i].y, &Img_bullet[1], NOTSRCERASE);
            putimage(bull[i].x, bull[i].y, &Img_bullet[0], SRCINVERT);
        }

    }

    //绘制敌机
    for (int i = 0; i < MAX_ENEMI_NUM; i++)
    {
        if (enemies[i].alive) {
            if (enemies[i].type==BIG_PLANE) {
                putimage(enemies[i].x, enemies[i].y, &ENEMI_A[1][0], NOTSRCERASE);
                putimage(enemies[i].x, enemies[i].y, &ENEMI_A[1][1], SRCINVERT);

            }
            else
            {
                putimage(enemies[i].x, enemies[i].y, &ENEMI_A[0][0], NOTSRCERASE);
                putimage(enemies[i].x, enemies[i].y, &ENEMI_A[0][1], SRCINVERT);
            }
            
        }

    }

}

void PlayerMove(int speed) {
    /*
    * 方法A :获取Char实现控制，但是不好用。建议打字、输入文本时再考虑类似的方案
    
    if (_kbhit()) {
        char key = _getch();
        switch (key)
        {
        case 'a':case'A':
            player.x -= speed;
            break;
        case 'd':case'D':
            player.x += speed;
            break;
        case 'w':case'W':
            player.y -= speed;
            break;
        case 's':case'S':
            player.y += speed;
            break;
        default:
            break;
        }
    }
*/
    
    /*异步非阻塞函数，不会在调用时停滞，最推荐*/
    if (GetAsyncKeyState('A') && player.x > MAX_PLAYER_LEFTH) {
        player.x -= speed;
    } 
    if (GetAsyncKeyState('D') && player.x < MAX_PLAYER_X_RIGHT) {
        player.x += speed;
    }
    if (GetAsyncKeyState('W') && player.y > MAX_PLAYER_HIGTH) {
        player.y -= speed;
    } 
    if (GetAsyncKeyState('S') && player.y < MAX_PLAYER_Y_DOWN) {
        player.y += speed;
    }
    //检测子弹发射键 空格 是否按下
    if (GetAsyncKeyState(VK_SPACE))
    {
        //创建一个子弹
        create_bullet();
    }
}

void BulletMove(int speed) {
    for (int i = 0; i < max_bullet_num; i++)
    {
        if (bull[i].alive&&bull[i].y>-15) {
            bull[i].y -= speed;
        }
        else
        {
            bull[i].alive = false;
        }

    }
}

int main()
{
    GameStartUp();
    Startup_LoadImage();
    //输出Hello World!
    cout << "Hello World!\n";
    //显示玩家图像并且擦掉黑边
    putimage(player.x, player.y, &Img_role[1], NOTSRCERASE);
    putimage(player.x, player.y, &Img_role[0],SRCINVERT);
    
    //启用了双缓冲绘图的绘制
    BeginBatchDraw();//启动双缓冲绘图

    //主进程循环(指调用循环)
    while (true)
    {
        Sleep(10);
        BulletMove(Bullet_Speed);
        
        PlayerMove(player.speed_PX);

        GameDraw();
        FlushBatchDraw();//刷新绘制
        GameTicks += 1;
    }
    EndBatchDraw();//结束双缓冲绘图
}