#include <windows.h>
#include <stdio.h>
#include <time.h>

// 交易盘口句柄  结构体
// 
typedef struct {
	HWND hwnd;          // 主句柄
	RECT rect;          // 主句柄 rectangle
	HWND jydhHwnd;      // 交易对话句柄，父句柄是 主句柄
	RECT jydhRect;      // 交易对话 rectangle
	HWND gpdmHwnd;      // 股票代码句柄, 父句柄是 交易对话句柄
	HWND maiRuJiaGeHwnd;      // 买入价格句柄, 父句柄是 交易对话句柄
	HWND maiRuShuLiangHwnd;   // 买入数量句柄, 父句柄是 交易对话句柄
	HWND maiRuAnNiuHwnd;      // 买入按钮
	HWND maiChuJiaGeHwnd;     // 卖出价格句柄, 父句柄是 交易对话句柄
	HWND maiChuShuLiangHwnd;  // 卖出数量句柄, 父句柄是 交易对话句柄
	HWND maiChuAnNiuHwnd;     // 卖出按钮

} JiaoYiPanKou;

// 主窗口句柄
typedef struct {
	HWND mainHwnd;
	RECT rect;
	char* title;

	HWND ziJinYuEHwnd;       // 资金余额
	HWND keYongJinEHwnd;     // 可用金额
	HWND keQuJinEHwnd;       // 可取金额
	HWND zongZiChanHwnd;     // 总资产
	HWND canKaoShiZhiHwnd;   // 参考市值
	HWND chiCangYingKuiHwnd; // 持仓盈亏
	HWND cangWeiHwnd;        // 仓位

	HWND chicangHwnd;        // 持仓表格
	HWND weituoHwnd;         // 委托表格
	HWND chengjiaoHwnd;      // 成交表格


	JiaoYiPanKou pankou1;    // 交易盘口1
	JiaoYiPanKou pankou2;    // 交易盘口2
	JiaoYiPanKou pankou3;    // 交易盘口3
	JiaoYiPanKou pankou4;    // 交易盘口4
	JiaoYiPanKou pankou5;    // 交易盘口5
	JiaoYiPanKou pankou6;    // 交易盘口6
	JiaoYiPanKou pankou7;    // 交易盘口7
	JiaoYiPanKou pankou8;    // 交易盘口8
	JiaoYiPanKou pankou9;    // 交易盘口9
	JiaoYiPanKou pankou10;   // 交易盘口10
} ZhuanYeXiaDan;




// 查询“专业版下单”窗口
static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	wchar_t className[256];
	wchar_t title[256];

	// 获取窗口类名
	if (GetClassNameW(hwnd, className, 256) == 0)
		return TRUE; // 继续枚举

	// 检查是否为Dialog类
	if (wcscmp(className, L"#32770") != 0)
		return TRUE;

	// 获取窗口标题
	if (GetWindowTextW(hwnd, title, 256) == 0)
		return TRUE;

	// 检查标题是否匹配
	if (wcscmp(title, L"专业版下单") == 0) {
		*reinterpret_cast<HWND*>(lParam) = hwnd;
		return FALSE; // 找到目标，停止枚举
	}
	return TRUE; // 继续枚举
}




// 交易盘口相对“专业版下单”主句柄的位置： 2x5 双向模式
// 使用相对“右上角”的距离和大小，确定交易盘口的句柄
INT panKouWidth = 224; INT panKouHeight = 435;
INT panKouTopDistance = 35; INT panKouTopDistance2 = 490;
INT panKouMostRightDistance = 4;
const INT pankou5RightDistance = panKouMostRightDistance;
const INT pankou4RightDistance = panKouWidth * 1 + panKouMostRightDistance;
const INT pankou3RightDistance = panKouWidth * 2 + panKouMostRightDistance;
const INT pankou2RightDistance = panKouWidth * 3 + panKouMostRightDistance;
const INT pankou1RightDistance = panKouWidth * 4 + panKouMostRightDistance;

const INT pankou10RightDistance = panKouMostRightDistance;
const INT pankou9RightDistance = panKouWidth * 1 + panKouMostRightDistance;
const INT pankou8RightDistance = panKouWidth * 2 + panKouMostRightDistance;
const INT pankou7RightDistance = panKouWidth * 3 + panKouMostRightDistance;
const INT pankou6RightDistance = panKouWidth * 4 + panKouMostRightDistance;

// 遍历专业版下单窗口的子窗口，组装成 ZhuanYeXiaDan 结构
static BOOL CALLBACK EnumZhuanYeXiaDanChildProc(HWND hwnd, LPARAM lParam) {
	ZhuanYeXiaDan* zhuan = (ZhuanYeXiaDan*)lParam;
	RECT parentRect = zhuan->rect;

	RECT childRect;
	GetWindowRect(hwnd, &childRect);

	// 计算子窗口的位置参数
	int rightDistance = parentRect.right - childRect.right;
	int topDistance = childRect.top - parentRect.top;
	int width = childRect.right - childRect.left;
	int height = childRect.bottom - childRect.top;
	// 误差
	int wucha = 1;
	// todo 资金股份



	// 遍历所有条件, 交易盘口是固定大小， 先判断大小
	if (width >= (panKouWidth - wucha) && width <= (panKouWidth + wucha) && height >= (panKouHeight - wucha) && height <= (panKouHeight + wucha)) {
		if (topDistance >= panKouTopDistance - wucha && topDistance <= panKouTopDistance + wucha) {
			if (rightDistance >= pankou1RightDistance - wucha && rightDistance <= pankou1RightDistance + wucha) {
				JiaoYiPanKou pankou;
				pankou.hwnd = hwnd;
				pankou.rect = childRect;
				zhuan->pankou1 = pankou;
				return TRUE;
			}
			if (rightDistance >= pankou2RightDistance - wucha && rightDistance <= pankou2RightDistance + wucha) {
				JiaoYiPanKou pankou;
				pankou.hwnd = hwnd;
				pankou.rect = childRect;
				zhuan->pankou2 = pankou;
				return TRUE;
			}
			if (rightDistance >= pankou3RightDistance - wucha && rightDistance <= pankou3RightDistance + wucha) {
				JiaoYiPanKou pankou;
				pankou.hwnd = hwnd;
				pankou.rect = childRect;
				zhuan->pankou3 = pankou;
				return TRUE;
			}
			if (rightDistance >= pankou4RightDistance - wucha && rightDistance <= pankou4RightDistance + wucha) {
				JiaoYiPanKou pankou;
				pankou.hwnd = hwnd;
				pankou.rect = childRect;
				zhuan->pankou4 = pankou;
				return TRUE;
			}
			if (rightDistance >= pankou5RightDistance - wucha && rightDistance <= pankou5RightDistance + wucha) {
				JiaoYiPanKou pankou;
				pankou.hwnd = hwnd;
				pankou.rect = childRect;
				zhuan->pankou5 = pankou;
				return TRUE;
			}
		}
		else if (topDistance >= panKouTopDistance2 - wucha && topDistance <= panKouTopDistance2 + wucha) {
			if (rightDistance >= pankou6RightDistance - wucha && rightDistance <= pankou6RightDistance + wucha) {
				JiaoYiPanKou pankou;
				pankou.hwnd = hwnd;
				pankou.rect = childRect;
				zhuan->pankou6 = pankou;
				return TRUE;
			}
			if (rightDistance >= pankou7RightDistance - wucha && rightDistance <= pankou7RightDistance + wucha) {
				JiaoYiPanKou pankou;
				pankou.hwnd = hwnd;
				pankou.rect = childRect;
				zhuan->pankou7 = pankou;
				return TRUE;
			}
			if (rightDistance >= pankou8RightDistance - wucha && rightDistance <= pankou8RightDistance + wucha) {
				JiaoYiPanKou pankou;
				pankou.hwnd = hwnd;
				pankou.rect = childRect;
				zhuan->pankou8 = pankou;
				return TRUE;
			}
			if (rightDistance >= pankou9RightDistance - wucha && rightDistance <= pankou9RightDistance + wucha) {
				JiaoYiPanKou pankou;
				pankou.hwnd = hwnd;
				pankou.rect = childRect;
				zhuan->pankou9 = pankou;
				return TRUE;
			}
			if (rightDistance >= pankou10RightDistance - wucha && rightDistance <= pankou10RightDistance + wucha) {
				JiaoYiPanKou pankou;
				pankou.hwnd = hwnd;
				pankou.rect = childRect;
				zhuan->pankou10 = pankou;
				return TRUE;
			}
		}
	}


	return TRUE; // 继续枚举子窗口
}




// 从交易盘口的子句柄找到真正的 交易对话句柄，交易对话句柄 相对于交易盘口的位置
INT jydhWidth = 219, jydhHeight = 134, jydhLeft = 1, jydhTop = 44;   // 交易对话大小、位置
INT gpdmWidth = 55, gpdmHeight = 17, gpdmLeft = 30, gpdmTop = 50;  // 股票代码输入框，大小、位置

INT jgslWidth = 79, jgslHeight = 21; // 价格输入框、数量输入框、买入卖出按钮的大小
INT maiRuLeft = 29, maiChuLeft = 130, jiaGeTop = 73, shuLiangTop = 125, anNiuTop = 151; // 价格、数量输入框的位置


// 遍历专业版下单窗口的子窗口，组装成 ZhuanYeXiaDan 结构
static BOOL CALLBACK EnumJiaoYiPanKouChildProc(HWND hwnd, LPARAM lParam) {
	JiaoYiPanKou* panKou = (JiaoYiPanKou*)lParam;
	RECT parentRect = panKou->rect;

	RECT childRect;
	GetWindowRect(hwnd, &childRect);

	// 计算子窗口的位置参数
	int leftDistance = childRect.left - parentRect.left;
	int topDistance = childRect.top - parentRect.top;
	int width = childRect.right - childRect.left;
	int height = childRect.bottom - childRect.top;
	// 误差
	int wucha = 1;

	// 交易对话（包含了股票代码、买入卖出价格数量）
	if (width >= (jydhWidth - wucha) && width <= (jydhWidth + wucha) &&
		height >= (jydhHeight - wucha) && height <= (jydhHeight + wucha) &&
		leftDistance >= jydhLeft - wucha && leftDistance <= jydhLeft + wucha &&
		topDistance >= jydhTop - wucha && topDistance <= jydhTop + wucha) {
		panKou->jydhHwnd = hwnd;
		panKou->jydhRect = childRect;
		return TRUE;
	}

	// 股票代码句柄
	if (width >= (gpdmWidth - wucha) && width <= (gpdmWidth + wucha) &&
		height >= (gpdmHeight - wucha) && height <= (gpdmHeight + wucha) &&
		leftDistance >= gpdmLeft - wucha && leftDistance <= gpdmLeft + wucha &&
		topDistance >= gpdmTop - wucha && topDistance <= gpdmTop + wucha) {
		panKou->gpdmHwnd = hwnd;
		return TRUE;
	}
	// 价格、数量输入框
	if (width >= (jgslWidth - wucha) && width <= (jgslWidth + wucha) &&
		height >= (jgslHeight - wucha) && height <= (jgslHeight + wucha)) {
		if (leftDistance >= maiRuLeft - wucha && leftDistance <= maiRuLeft + wucha &&
			topDistance >= jiaGeTop - wucha && topDistance <= jiaGeTop + wucha) {
			panKou->maiRuJiaGeHwnd = hwnd;
			return TRUE;
		}
		if (leftDistance >= maiRuLeft - wucha && leftDistance <= maiRuLeft + wucha &&
			topDistance >= shuLiangTop - wucha && topDistance <= shuLiangTop + wucha) {
			panKou->maiRuShuLiangHwnd = hwnd;
			return TRUE;
		}
		if (leftDistance >= maiRuLeft - wucha && leftDistance <= maiRuLeft + wucha &&
			topDistance >= anNiuTop - wucha && topDistance <= anNiuTop + wucha) {
			panKou->maiRuAnNiuHwnd = hwnd;
			return TRUE;
		}
		if (leftDistance >= maiChuLeft - wucha && leftDistance <= maiChuLeft + wucha &&
			topDistance >= jiaGeTop - wucha && topDistance <= jiaGeTop + wucha) {
			panKou->maiChuJiaGeHwnd = hwnd;
			return TRUE;
		}
		if (leftDistance >= maiChuLeft - wucha && leftDistance <= maiChuLeft + wucha &&
			topDistance >= shuLiangTop - wucha && topDistance <= shuLiangTop + wucha) {
			panKou->maiChuShuLiangHwnd = hwnd;
			return TRUE;
		}
		if (leftDistance >= maiChuLeft - wucha && leftDistance <= maiChuLeft + wucha &&
			topDistance >= anNiuTop - wucha && topDistance <= anNiuTop + wucha) {
			panKou->maiChuAnNiuHwnd = hwnd;
			return TRUE;
		}
	}

	return TRUE; // 继续枚举子窗口
}


// 将字符转换为虚拟键码
bool CharToVKey(UINT ch, WORD& vk)
{
	SHORT state = VkKeyScanW(ch);
	if (state == -1) return false;

	vk = LOBYTE(state);
	return true;
}
// 模拟键盘输入完整字符串
bool SendKeyboardInput(HWND hEdit, const wchar_t* text)
{
	if (!IsWindow(hEdit)) return false;

	// 激活目标窗口（重要！）
	//DWORD targetThread = GetWindowThreadProcessId(hEdit, NULL);
	//DWORD currentThread = GetCurrentThreadId();
	//AttachThreadInput(currentThread, targetThread, TRUE);

	// 设置焦点到Edit控件
	SetFocus(hEdit);
	SetForegroundWindow(GetParent(hEdit));  // 确保父窗口在前台

	// 发送键盘输入
	INPUT inputs[2] = {};
	for (const wchar_t* p = text; *p; ++p)
	{
		WORD vk;
		if (!CharToVKey(*p, vk)) continue;

		// 按下键
		inputs[0].type = INPUT_KEYBOARD;
		inputs[0].ki.wVk = vk;
		inputs[0].ki.dwFlags = 0;
		// 松开键
		inputs[1] = inputs[0];
		inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(2, inputs, sizeof(INPUT));
	}

	//AttachThreadInput(currentThread, targetThread, FALSE);
	return true;
}


void mouse_click(int x, int y) {
	// 设置鼠标位置（使用屏幕绝对坐标）
	SetCursorPos(x, y);

	// 生成鼠标左键按下事件
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	//Sleep(20);
	// 生成鼠标左键释放事件
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

bool SendTabToWindowEx(HWND hWnd) {
	// 获取目标窗口线程ID
	// 发送Tab键输入
	INPUT inputs[2] = { 0 };
	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_TAB;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_TAB;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
	bool success = (SendInput(2, inputs, sizeof(INPUT)) == 2);
	return success;
}

int main() {

	HWND targetHwnd = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)&targetHwnd);

	if (targetHwnd != NULL) {
		printf("找到窗口句柄：0x%p\n", targetHwnd);
	}
	else {
		printf("未找到目标窗口。\n");
		exit(1);
	}

	SetForegroundWindow(targetHwnd);

	RECT rect;
	GetWindowRect(targetHwnd, &rect);

	// 初始化专业下单结构体
	ZhuanYeXiaDan zhuanYe = { 0 };
	zhuanYe.mainHwnd = targetHwnd;
	zhuanYe.rect = rect;

	EnumChildWindows(targetHwnd, EnumZhuanYeXiaDanChildProc, (LPARAM)&zhuanYe);

	// 遍历交易盘口
	EnumChildWindows(zhuanYe.pankou1.hwnd, EnumJiaoYiPanKouChildProc, (LPARAM)&zhuanYe.pankou1);
	EnumChildWindows(zhuanYe.pankou2.hwnd, EnumJiaoYiPanKouChildProc, (LPARAM)&zhuanYe.pankou2);
	EnumChildWindows(zhuanYe.pankou3.hwnd, EnumJiaoYiPanKouChildProc, (LPARAM)&zhuanYe.pankou3);
	EnumChildWindows(zhuanYe.pankou4.hwnd, EnumJiaoYiPanKouChildProc, (LPARAM)&zhuanYe.pankou4);
	EnumChildWindows(zhuanYe.pankou5.hwnd, EnumJiaoYiPanKouChildProc, (LPARAM)&zhuanYe.pankou5);
	EnumChildWindows(zhuanYe.pankou6.hwnd, EnumJiaoYiPanKouChildProc, (LPARAM)&zhuanYe.pankou6);
	EnumChildWindows(zhuanYe.pankou7.hwnd, EnumJiaoYiPanKouChildProc, (LPARAM)&zhuanYe.pankou7);
	EnumChildWindows(zhuanYe.pankou8.hwnd, EnumJiaoYiPanKouChildProc, (LPARAM)&zhuanYe.pankou8);
	EnumChildWindows(zhuanYe.pankou9.hwnd, EnumJiaoYiPanKouChildProc, (LPARAM)&zhuanYe.pankou9);
	EnumChildWindows(zhuanYe.pankou10.hwnd, EnumJiaoYiPanKouChildProc, (LPARAM)&zhuanYe.pankou10);

	// SetWindowText(zhuanYe.pankou1.gpdmHwnd, L"000001");                           // SetWindowText 不生效
	// SendMessage(zhuanYe.pankou1.gpdmHwnd, WM_SETTEXT, 0, (LPARAM)L"000001");     // SendMessage 发送 WM_SETTEXT 也不生效 


	// todo 先走通下单 

	mouse_click(zhuanYe.pankou1.rect.left + 5, zhuanYe.pankou1.rect.top + 5);
	//SendKeyboardInput(zhuanYe.pankou1.gpdmHwnd, L"000001");
	SendKeyboardInput(zhuanYe.pankou1.hwnd, L"000001");
	Sleep(80);


	//SendKeyboardInput(zhuanYe.pankou1.maiRuJiaGeHwnd, L"9.93");

	RECT rect2;
	GetWindowRect(zhuanYe.pankou1.maiRuShuLiangHwnd, &rect2);
	//SendTabToWindowEx(zhuanYe.pankou1.hwnd);
	//SetFocusToWindow(zhuanYe.pankou1.maiRuShuLiangHwnd);
	mouse_click((rect2.left + rect2.right) / 2, (rect2.top + rect2.bottom) / 2);
	//Sleep(20);
	// 输入事件应该传给 交易盘口的整个句柄，而不是输入框Edit的句柄
	SendKeyboardInput(zhuanYe.pankou1.hwnd, L"200");

	Sleep(20);
	SendMessage(zhuanYe.pankou1.maiRuAnNiuHwnd, BM_CLICK, 0, 0);
	// SetFocusToWindow(zhuanYe.pankou1.hwnd);
	// SendLeftClickToWindow(zhuanYe.pankou1.hwnd, 1, 1);
	//Sleep(1000);
	//mouse_click(zhuanYe.pankou1.rect.left + 5, zhuanYe.pankou1.rect.top + 5);
	//SendTabToWindowEx(zhuanYe.pankou1.hwnd);
	//SendKeyboardInput(zhuanYe.mainHwnd, L"b");

	return 0;
}