#pragma once
#include <vector>
#include "DS/Vec2.h"
#include "gdiplus.h"
#include <Windows.h>
#include <string>
#include <unordered_map>

// 吴灿杰
// 备注：afx_msg是系统自己生成的，没有afx_msg的，是我额外编写的。
class Graph;
struct Vertex;
struct Edge;

class CEGMain final : public CDialogEx{
protected:
    struct GraphInfo{
        Graph graph;
        std::vector<int> escBegin;
        std::vector<int> escEnd;
        int   minX, minY, maxX, maxY;
        GraphInfo() = default;
        GraphInfo(Graph&& g, int minx, int miny, int maxx, int maxy);
    };
    struct PathInfo{
        double length;
        // 开头到终末
        std::vector<int> path;
        std::vector<Edge> edges;
        PathInfo(std::vector<int>&& path, std::vector<Edge>&& edges, double len);
    };
    struct MFCUI{
        CButton		*btnStart	      = nullptr;
        CButton     *btnLoadGraph     = nullptr;
        CButton     *btnDelGraph      = nullptr;
        CButton		*checkRoadLen     = nullptr;
        CButton     *checkSafety      = nullptr;
        CButton     *checkCrowding    = nullptr;
        CButton     *checkOnFire      = nullptr;
        CButton     *checkRandTag     = nullptr;
        CButton     *checkVertexName  = nullptr;
        CComboBox   *choiceEscBegin   = nullptr;
        CComboBox   *choiceEscEnd     = nullptr;
        CStatic     *picctrlPaintArea = nullptr;
        CTabCtrl	*tabMap		      = nullptr;
        // 设置开始按钮
    }mfc;

    bool      isDarkMode = false;
    bool      isOnFire   = false;
    int       currentIdx = -1;
    HICON     m_hIcon;
    ULONG_PTR m_gdiplusToken = 0;
    std::vector<GraphInfo> graphInfos;
    std::vector<PathInfo> paths;

    const wchar_t *kVERTEX_DORMITORY = L".\\res\\Vertex_Dormitory.png"       ;
    const wchar_t *kVERTEX_EXIT      = L".\\res\\Vertex_Exit.png"            ;
    const wchar_t *kVERTEX_FLOORNODE = L".\\res\\Vertex_FloorNode.png"       ;
    const wchar_t *kVERTEX_TBUILDING = L".\\res\\Vertex_TeachingBuilding.png";
    const wchar_t *kVERTEX_UNKNOWN   = L".\\res\\Vertex_Unknown.png"         ;
    const wchar_t *kSTR_VERTEX_NONE     = L"--无--";
    const wchar_t* kSTR_VERTEX_ALL_EXIT = L"--所有出口--";
    const COLORREF kCOLOR_ON_FIRE     = RGB(255,   0,   0);
    // 白色
    const COLORREF kCOLOR_DORM_W      = RGB(150,   0,   0);
    const COLORREF kCOLOR_EXIT_W      = RGB(  0, 150,   0);
    const COLORREF kCOLOR_FLOORNODE_W = RGB(200,   0, 200);
    const COLORREF kCOLOR_TBUILDING_W = RGB(  0,   0, 255);
    const COLORREF kCOLOR_UNKNOWN_W   = RGB(  0,   0,   0);
    const COLORREF kCOLOR_BKG_W       = RGB(255, 255, 255);
    const COLORREF kCOLOR_PATH_HINT   = RGB(  0, 255,   0);
    // 黑色
    const COLORREF kCOLOR_DORM_D      = RGB(200,   0,   0);
    const COLORREF kCOLOR_EXIT_D      = RGB(  0, 200,   0);
    const COLORREF kCOLOR_FLOORNODE_D = RGB(200,   0, 200);
    const COLORREF kCOLOR_TBUILDING_D = RGB(150, 150, 255);
    const COLORREF kCOLOR_UNKNOWN_D   = RGB(150, 150, 150);
    const COLORREF kCOLOR_BKG_D       = RGB( 64,  64,  64);
    
    const int      kLENGTH_MARGIN     = 40;
    const UINT_PTR kTIMER_UPDATE_GRAPH_LATER = 1;

protected:
    // 生成的消息映射函数
    static CString BrowseForFile(LPCTSTR title = _T("选择文件夹"));
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnDestroy();
    afx_msg virtual BOOL OnInitDialog();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTcnSelchangeTabMap(NMHDR* pNMHDR, LRESULT* pResult);
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    // 加载图于此程序。由于目前图结构只服务于当前程序，这将调用移动语义
    void AddNewGraph(Graph& g, const wchar_t* name);
    // 立即绘制图。
    void UpdateGraph();
    // 延迟一段时间后再绘制图。本函数用于程序显示窗口期间。
    void UpdateGraphLater();
    // 绘制边。
    // @param begin 起点。
    // @param end   终点。
    // @param color 绘制色彩
    void PaintEdge(Vec2Int begin, Vec2Int end, COLORREF color, int stroke);
    // 绘制点。
    // @param pos  绘制位置。
    // @param type 绘制节点类型。
    void PaintVertex(Vec2Int pos, VertexType type, bool isOnFire, bool isSelected);
    // 绘制多个文字。使用居中对其。
    // @param pos    绘制位置。
    // @param textSize 文字大小。
    // @param texts  要绘制的文本数组。
    // @param color  对应颜色的数组。
    // @param num    要绘制的文本数量。
    void PaintText(
        Vec2Int pos, int textSize, bool doRemoveBkg,
        const char** texts, const COLORREF colors[], int num,
        Gdiplus::StringAlignment aHorizonal = Gdiplus::StringAlignmentCenter,
        Gdiplus::StringAlignment aVertical = Gdiplus::StringAlignmentCenter
    );
    // 绘制所有的点
    void PaintVertexes();
    // 绘制所有的边
    void PaintEdges();
    // 清除绘图内容。
    void ClearGraphDisplay();
    Vec2Int GetTransformedPosition(
        Vec2Int oPos, const GraphInfo& info, const CRect& rectTM
    );
    int GetTabHeight() const;
    DECLARE_MESSAGE_MAP()
public:
#	ifdef AFX_DESIGN_TIME
    enum { IDD = kMFC_DLG_MAIN };
#	endif
    CEGMain(CWnd* pParent = nullptr);	// 标准构造函数
    afx_msg void OnClickedBtnLoad();
    // 设置当前显示页面，如果不显示任何页面，直接使用负数（比如-1.）
    // 同时会自动刷新地图显示界面（可以设置为false）。
    void SetCurrentPage(int page);
    int  GetCurrentPage() const;
    afx_msg void OnCickedBtnDel();
    afx_msg void OnChangeChoiceEscBegin();
    static std::wstring GbkToUtf16(const char* gbkStr);
    afx_msg void OnChangeChoiceEscEnd();
    afx_msg void OnCheckOnFire();
    afx_msg void OnBnClickedCheckSafety();
    afx_msg void OnClickedCheckRoadlen();
    afx_msg void OnBnClickedCheckCrowding();
    afx_msg void OnClickedCheckVertexName();
    static Gdiplus::Color ToGDIPColor(const COLORREF& color, BYTE a = 255);
    afx_msg void OnClickedCheckRandTag();
};
