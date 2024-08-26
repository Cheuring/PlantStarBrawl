#include <graphics.h>
 
 
 
// 实现文本框控件
class EasyTextBox
{
private:
	int left = 0, top = 0, right = 0, bottom = 0;	// 控件坐标
	char* text = NULL;							// 控件内容
	size_t maxlen = 0;									// 文本框最大内容长度
 
public:
	void Create(int x1, int y1, int x2, int y2, int max)
	{
		maxlen = max;
		text = new char[maxlen];
		text[0] = 0;
		left = x1, top = y1, right = x2, bottom = y2;
 
		// 绘制用户界面
		Show();
	}
 
	~EasyTextBox()
	{
		if (text != NULL)
			delete[] text;
	}
 
	char* Text()
	{
		return text;
	}
 
	bool Check(int x, int y)
	{
		return (left <= x && x <= right && top <= y && y <= bottom);
	}
 
	// 绘制界面
	void Show()
	{
		// 备份环境值
		int oldlinecolor = getlinecolor();
		int oldbkcolor = getbkcolor();
		int oldfillcolor = getfillcolor();
 
		setlinecolor(LIGHTGRAY);		// 设置画线颜色
		setbkcolor(0xeeeeee);			// 设置背景颜色
		setfillcolor(0xeeeeee);			// 设置填充颜色
		fillrectangle(left, top, right, bottom);
		outtextxy(left + 10, top + 5, text);
 
		// 恢复环境值
		setlinecolor(oldlinecolor);
		setbkcolor(oldbkcolor);
		setfillcolor(oldfillcolor);
	}
 
	void OnMessage()
	{
		// 备份环境值
		int oldlinecolor = getlinecolor();
		int oldbkcolor = getbkcolor();
		int oldfillcolor = getfillcolor();
 
		setlinecolor(BLACK);			// 设置画线颜色
		setbkcolor(WHITE);				// 设置背景颜色
		setfillcolor(WHITE);			// 设置填充颜色
		fillrectangle(left, top, right, bottom);
		outtextxy(left + 10, top + 5, text);
 
		int width = textwidth(text);	// 字符串总宽度
		int counter = 0;				// 光标闪烁计数器
		bool binput = true;				// 是否输入中
 
		ExMessage msg;
		while (binput)
		{
			while (binput && peekmessage(&msg, EX_MOUSE | EX_CHAR, false))	// 获取消息，但不从消息队列拿出
			{
				if (msg.message == WM_LBUTTONDOWN)
				{
					// 如果鼠标点击文本框外面，结束文本输入
					if (msg.x < left || msg.x > right || msg.y < top || msg.y > bottom)
					{
						binput = false;
						break;
					}
				}
				else if (msg.message == WM_CHAR)
				{
					size_t len = strlen(text);
					switch (msg.ch)
					{
					case '\b':				// 用户按退格键，删掉一个字符
						if (len > 0)
						{
							text[len - 1] = 0;
							width = textwidth(text);
							counter = 0;
							clearrectangle(left + 10 + width, top + 1, right - 1, bottom - 1);
						}
						break;
					case '\r':				// 用户按回车键，结束文本输入
					case '\n':
						binput = false;
						break;
					default:				// 用户按其它键，接受文本输入
						if (len < maxlen - 1)
						{
							text[len++] = msg.ch;
							text[len] = 0;
 
							clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);	// 清除画的光标
							width = textwidth(text);				// 重新计算文本框宽度
							counter = 0;
							outtextxy(left + 10, top + 5, text);		// 输出新的字符串
						}
					}
				}
				peekmessage(NULL, EX_MOUSE | EX_CHAR);				// 从消息队列抛弃刚刚处理过的一个消息
			}
 
			// 绘制光标（光标闪烁周期为 20ms * 32）
			counter = (counter + 1) % 32;
			if (counter < 16)
				line(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);				// 画光标
			else
				clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);		// 擦光标
 
			// 延时 20ms
			Sleep(20);
		}
 
		clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);	// 擦光标
 
		// 恢复环境值
		setlinecolor(oldlinecolor);
		setbkcolor(oldbkcolor);
		setfillcolor(oldfillcolor);
 
		Show();
	}
};
 
 
 
// 实现按钮控件
class EasyButton
{
private:
	int left = 0, top = 0, right = 0, bottom = 0;	// 控件坐标
	char* text = NULL;							// 控件内容
	void (*userfunc)() = NULL;						// 控件消息
 
public:
	void Create(int x1, int y1, int x2, int y2, const char* title, void (*func)())
	{
		text = new char[strlen(title) + 1];
		strcpy_s(text, strlen(title) + 1, title);
		left = x1, top = y1, right = x2, bottom = y2;
		userfunc = func;
 
		// 绘制用户界面
		Show();
	}
 
	~EasyButton()
	{
		if (text != NULL)
			delete[] text;
	}
 
	bool Check(int x, int y)
	{
		return (left <= x && x <= right && top <= y && y <= bottom);
	}
 
	// 绘制界面
	void Show()
	{
		int oldlinecolor = getlinecolor();
		int oldbkcolor = getbkcolor();
		int oldfillcolor = getfillcolor();
 
		setlinecolor(BLACK);			// 设置画线颜色
		setbkcolor(WHITE);				// 设置背景颜色
		setfillcolor(WHITE);			// 设置填充颜色
		fillrectangle(left, top, right, bottom);
		outtextxy(left + (right - left - textwidth(text) + 1) / 2, top + (bottom - top - textheight(text) + 1) / 2, text);
 
		setlinecolor(oldlinecolor);
		setbkcolor(oldbkcolor);
		setfillcolor(oldfillcolor);
	}
 
	void OnMessage()
	{
		if (userfunc != NULL)
			userfunc();
	}
};
 
 
 
// // 定义控件
// EasyTextBox txtName;
// EasyTextBox txtPwd;
// EasyButton btnOK;
 
 
 
// 按钮 btnOK 的点击事件
// void On_btnOk_Click()
// {
// 	if (strcmp("123456", txtPwd.Text()))
// 		MessageBox(GetHWnd(), "密码错误", "错误", MB_OK);
// 	else
// 	{
// 		char s[100] = "Hello, ";
// 		strcat_s(s, 100, txtName.Text());
// 		MessageBox(GetHWnd(), s, "Hello", MB_OK);
// 	}
// }
 
 
 
// 主函数
// int main()
// {
// 	// 创建图形窗口
// 	initgraph(640, 480,SHOWCONSOLE);
 
// 	// 简单绘制界面
// 	setbkcolor(0xeeeeee);
// 	cleardevice();
// 	settextcolor(BLACK);
// 	outtextxy(50, 55, "用户名：");
// 	txtName.Create(120, 50, 400, 75, 10);						// 创建用户名文本框控件
// 	outtextxy(50, 105, "密 码：");
// 	txtPwd.Create(120, 100, 400, 125, 10);						// 创建密码文本框控件
// 	btnOK.Create(320, 150, 400, 175, "OK", On_btnOk_Click);	// 创建按钮控件
 
// 	ExMessage msg;
// 	while (true)
// 	{
// 		msg = getmessage(EX_MOUSE);			// 获取消息输入
 
// 		if (msg.message == WM_LBUTTONDOWN)
// 		{
// 			// 判断控件
// 			if (txtName.Check(msg.x, msg.y))	txtName.OnMessage();
 
// 			// 判断控件
// 			if (txtPwd.Check(msg.x, msg.y))		txtPwd.OnMessage();
 
// 			// 判断控件
// 			if (btnOK.Check(msg.x, msg.y))		btnOK.OnMessage();
// 		}
// 	}
 
// 	// 关闭绘图窗口
// 	closegraph();
// 	return 0;
// }