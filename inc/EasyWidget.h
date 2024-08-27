#include <graphics.h>
 
 
 
// 实现文本框控件
class EasyTextBox
{
private:
	int left = 0, top = 0, right = 0, bottom = 0;	// 控件坐标
	char* text = NULL;							// 控件内容
	size_t maxlen = 0;									// 文本框最大内容长度
	bool binput = false;							// 是否输入中
	int counter = 0;								// 光标闪烁计数器
 
public:
	~EasyTextBox();

	void Create(int x1, int y1, int x2, int y2, int max);
	auto Text() const -> char*;
	auto Check(int x, int y) const -> bool;
	void Show();
	void OnMessage();
};
 
 
 
// 实现按钮控件
class EasyButton
{
private:
	int left = 0, top = 0, right = 0, bottom = 0;	// 控件坐标
	char* text = NULL;							// 控件内容
	void (*userfunc)() = NULL;						// 控件消息
 
public:
	~EasyButton();
	void Create(int x1, int y1, int x2, int y2, const char* title, void (*func)());
	auto Check(int x, int y) const -> bool;
	void Show() const;
	void OnMessage() const;

};
 
 