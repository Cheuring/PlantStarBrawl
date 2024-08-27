#include "EasyWidget.h"

EasyTextBox::~EasyTextBox() {
    if (text != NULL) {
        delete[] text;
    }
}

void EasyTextBox::Create(int x1, int y1, int x2, int y2, int max) {
    maxlen = max;
    text = new char[maxlen];
    text[0] = 0;
    left = x1, top = y1, right = x2, bottom = y2;

    // 绘制用户界面
    Show();
}

auto EasyTextBox::Text() const -> char* {
    return text;
}

auto EasyTextBox::Check(int x, int y) const -> bool {
    return (left <= x && x <= right && top <= y && y <= bottom);
}

void EasyTextBox::Show(){
    // 备份环境值
    int oldlinecolor = getlinecolor();
    int oldbkcolor = getbkcolor();
    int oldfillcolor = getfillcolor();

    setlinecolor(LIGHTGRAY);		// 设置画线颜色
    setbkcolor(0xeeeeee);			// 设置背景颜色
    setfillcolor(0xeeeeee);			// 设置填充颜色
    fillrectangle(left, top, right, bottom);
    outtextxy(left + 10, top + 5, text);

    if(binput){
        int width = textwidth(text);
        counter = (counter + 1) % 40;
        if (counter < 20)
            line(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);				// 画光标
        // else
        // 	clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);		// 擦光标
    }

    // 恢复环境值
    setlinecolor(oldlinecolor);
    setbkcolor(oldbkcolor);
    setfillcolor(oldfillcolor);
}

void EasyTextBox::OnMessage() {
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
    counter = 0;				// 光标闪烁计数器
    binput = true;				// 是否输入中

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
        // counter = (counter + 1) % 32;
        // if (counter < 16)
        // 	line(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);				// 画光标
        // else
        // 	clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);		// 擦光标

        // // 延时 20ms
        // Sleep(20);
    }

    clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);	// 擦光标

    // 恢复环境值
    setlinecolor(oldlinecolor);
    setbkcolor(oldbkcolor);
    setfillcolor(oldfillcolor);

    Show();
}


EasyButton::~EasyButton() {
    if(text != NULL){
        delete[] text;
    }
}

void EasyButton::Create(int x1, int y1, int x2, int y2, const char* title, void (*func)()) {
    text = new char[strlen(title) + 1];
    strcpy_s(text, strlen(title) + 1, title);
    left = x1, top = y1, right = x2, bottom = y2;
    userfunc = func;

    // 绘制用户界面
    Show();
}

auto EasyButton::Check(int x, int y) const -> bool {
    return (left <= x && x <= right && top <= y && y <= bottom);
}


void EasyButton::Show() const {
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

void EasyButton::OnMessage() const {
    if (userfunc != NULL){
        userfunc();
    }
}