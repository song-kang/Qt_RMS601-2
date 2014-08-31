
#ifndef __SKT_SVG_DEFINE_H__
#define __SKT_SVG_DEFINE_H__


//定义SVG窗口可能的各种操作
enum SVG_TOOLS
{
	TOOLS_NONE,			//无操作  
	TOOLS_SELECT,		//对象选择
	TOOLS_ZOOMIN,		//画布放大
	TOOLS_ZOOMOUT,	//画布缩小
	TOOLS_PAN,			//画布平移
	TOOLS_RECT,			//绘制区域
	TOOLS_TEXT,			//绘制文本
	TOOLS_ELLIPSE,	//绘制椭圆
	TOOLS_LINE,			//绘制直线
	TOOLS_POLYLINE,	//绘制折线
	TOOLS_PATH,			//绘制路径
	TOOLS_PIC				//绘制图片
};

//定义控件的可通知事件
#define UM_SVG_OBJECT_LCLICKED		WM_USER+1024
#define UM_SVG_OBJECT_RCLICKED		WM_USER+1025
#define UM_SVG_OBJECT_DBLCLICKED	WM_USER+1026


#endif