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

//防止以后的变量冲突，加上自定义类型
enum Window_Settings
{
    WIDTH = 640,//窗口宽度
    HEIGHT = 480,//窗口高度
    MAX_PLAYER_LEFTH = -30,//最大玩家可达到的左侧位置x
    MAX_PLAYER_HIGTH = -30,//最大玩家可达到的高度y
    MAX_PLAYER_Y_DOWN = HEIGHT - 30,//最大玩家可到达的最低点
    MAX_PLAYER_X_RIGHT = WIDTH - 30,//最大玩家可达到的最右边
    max_bullet_num = 114,//最大子弹数目
};

//玩家的飞只因与子弹结构体
struct Player_Plane
{
    int x = 0;//玩家的x坐标
    int y=0;//玩家的y坐标
    bool alive=true;//玩家是死是活
    int speed_PX = 5;

}player,bull[max_bullet_num];

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

    //在(0 ,0)坐标显示图片
    putimage(0, 0, &图片);
    
    cout << "加载结束！\n";
}


void GameStartUp() {
    //创建窗口，并且设置为显示控制台
    initgraph(WIDTH, HEIGHT, EX_SHOWCONSOLE);

    //初始化子弹
    for (int i = 0; i < max_bullet_num; i++)
    {
        bull[i].x = bull[i].y = -114;
        bull[i].alive = false;
        //让所有子弹状态设置为挂掉了[不存在]
    }
}

//创建子弹
void create_bullet() {
    for (int i = 0; i < max_bullet_num; i++)
    {
        if (!bull[i].alive)
        {
            bull[i].x = player.x + 34;
            bull[i].y = player.y;
            bull[i].alive = true;
            break;
        }

    }
}

void GameDraw() {
    //绘制背景、玩家（玩家透明背景）
    putimage(0, 0, &图片);
    putimage(player.x, player.y, &Img_role[1], NOTSRCERASE);
    putimage(player.x, player.y, &Img_role[0], SRCINVERT);

    for (int i = 0; i < max_bullet_num; i++)
    {
        if (bull[i].alive) {
            putimage(bull[i].x, bull[i].y, &Img_bullet[1], NOTSRCERASE);
            putimage(bull[i].x, bull[i].y, &Img_bullet[0], SRCINVERT);
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
    while (114514)
    {
        Sleep(10);
        GameDraw();
        PlayerMove(player.speed_PX);
        FlushBatchDraw();//刷新绘制
    }
    EndBatchDraw();//结束双缓冲绘图
}