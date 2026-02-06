
// EmergencyGatewayDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "AppEG.h"
#include "CEGDlg.h"
#include "afxdialogex.h"

#pragma comment(lib,"gdiplus.lib")
#include "gdiplus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <windows.h>
#include <string>

std::wstring CEGMain::GbkToUtf16(const char* gbkStr){
    if(!gbkStr) return {};

    // 使用代码页 936 明确指定 GBK
    int len = MultiByteToWideChar(936, MB_ERR_INVALID_CHARS, gbkStr, -1, nullptr, 0);
    if(len == 0) {
        throw std::runtime_error("Invalid GBK string or conversion failed.");
    }

    std::wstring wstr(len - 1, L'\0'); // -1 去掉末尾的 \0（构造函数会自动加）
    MultiByteToWideChar(936, MB_ERR_INVALID_CHARS, gbkStr, -1, &wstr[0], len);

    return wstr;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx{
public:
    CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = kMFC_DLG_ABOUT };
#endif

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(kMFC_DLG_ABOUT){}

void CAboutDlg::DoDataExchange(CDataExchange* pDX){
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEGMain 对话框



CEGMain::CEGMain(CWnd* pParent /*=nullptr*/)
    : CDialogEx(kMFC_DLG_MAIN, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(kMFC_ICO_MAIN);
}

void CEGMain::OnDestroy(){
    Gdiplus::GdiplusShutdown(m_gdiplusToken);

    CDialogEx::OnDestroy();
}

void CEGMain::DoDataExchange(CDataExchange* pDX){
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEGMain, CDialogEx)
    ON_WM_TIMER()
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DESTROY()
    ON_NOTIFY(TCN_SELCHANGE, kMFC_TAB_MAP, &CEGMain::OnTcnSelchangeTabMap)
    ON_BN_CLICKED(kMFC_BTN_LOAD, &CEGMain::OnClickedBtnLoad)
    ON_BN_CLICKED(kMFC_BTN_DEL, &CEGMain::OnCickedBtnDel)
    ON_CBN_SELCHANGE(kMFC_CHOICE_ESC_BEGIN, &CEGMain::OnChangeChoiceEscBegin)
    ON_CBN_SELCHANGE(kMFC_CHOICE_ESC_END, &CEGMain::OnChangeChoiceEscEnd)
    ON_BN_CLICKED(kMFC_CHECK_ON_FIRE, &CEGMain::OnCheckOnFire)
    ON_BN_CLICKED(kMFC_CHECK_SAFETY, &CEGMain::OnBnClickedCheckSafety)
    ON_BN_CLICKED(kMFC_CHECK_ROADLEN, &CEGMain::OnClickedCheckRoadlen)
    ON_BN_CLICKED(kMFC_CHECK_CROWDING, &CEGMain::OnBnClickedCheckCrowding)
    ON_BN_CLICKED(kMFC_CHECK_VERTEXNAME, &CEGMain::OnClickedCheckVertexName)
    ON_BN_CLICKED(kMFC_CHECK_RANDTAG, &CEGMain::OnClickedCheckRandTag)
END_MESSAGE_MAP()

// CEGMain 消息处理程序

BOOL CEGMain::OnInitDialog(){
    std::cout << "[CEGMain::OnInitDialog]程序已启动！" << std::endl;
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu){
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(kMFC_STRING_TITLE_ABOUT);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty()){
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    this->mfc.btnDelGraph	   = static_cast<CButton*  >(GetDlgItem(kMFC_BTN_DEL)           );
    this->mfc.btnLoadGraph     = static_cast<CButton*  >(GetDlgItem(kMFC_BTN_LOAD)          );
    this->mfc.checkCrowding    = static_cast<CButton*  >(GetDlgItem(kMFC_CHECK_CROWDING)    );
    this->mfc.checkOnFire      = static_cast<CButton*  >(GetDlgItem(kMFC_CHECK_ON_FIRE)     );
    this->mfc.checkRandTag	   = static_cast<CButton*  >(GetDlgItem(kMFC_CHECK_RANDTAG)     );
    this->mfc.checkRoadLen     = static_cast<CButton*  >(GetDlgItem(kMFC_CHECK_ROADLEN)     );
    this->mfc.checkSafety      = static_cast<CButton*  >(GetDlgItem(kMFC_CHECK_SAFETY)      );
    this->mfc.checkVertexName  = static_cast<CButton*  >(GetDlgItem(kMFC_CHECK_VERTEXNAME)  );
    this->mfc.choiceEscBegin   = static_cast<CComboBox*>(GetDlgItem(kMFC_CHOICE_ESC_BEGIN)  );
    this->mfc.choiceEscEnd     = static_cast<CComboBox*>(GetDlgItem(kMFC_CHOICE_ESC_END)    );
    this->mfc.picctrlPaintArea = static_cast<CStatic*  >(GetDlgItem(kMFC_PICCTRL_PAINT_AREA));
    this->mfc.tabMap	       = static_cast<CTabCtrl* >(GetDlgItem(kMFC_TAB_MAP)           );
    this->SetCurrentPage(-1);
    this->UpdateGraphLater();

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEGMain::OnSysCommand(UINT nID, LPARAM lParam){
    if ((nID & 0xFFF0) == IDM_ABOUTBOX){
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }else{
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEGMain::OnPaint(){
    if (IsIconic()){
        CPaintDC dc(this); // 用于绘制的设备上下文
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        Vec2Int cIcon(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));
        CRect rect;
        this->GetClientRect(&rect);
        Vec2Int pos( (rect.Width() - cIcon.x + 1) / 2, (rect.Height() - cIcon.y + 1) / 2 );

        // 绘制图标
        dc.DrawIcon(pos.x, pos.y, this->m_hIcon);
    }else{
        CDialogEx::OnPaint();
        this->UpdateGraph();
    }
}

void CEGMain::OnTimer(UINT_PTR nIDEvent){
    this->CDialogEx::OnTimer(nIDEvent);
    if(nIDEvent == CEGMain::kTIMER_UPDATE_GRAPH_LATER){
        this->KillTimer(CEGMain::kTIMER_UPDATE_GRAPH_LATER);
        this->UpdateGraph();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEGMain::OnQueryDragIcon(){
    return static_cast<HCURSOR>(this->m_hIcon);
}

void CEGMain::OnTcnSelchangeTabMap(NMHDR* pNMHDR, LRESULT* pResult){
    this->SetCurrentPage(this->mfc.tabMap->GetCurSel());
    *pResult = TRUE;
}

void CEGMain::AddNewGraph(Graph& g, const wchar_t* name){
    TCITEM item;

    item.mask    = TCIF_TEXT;
    item.pszText = const_cast<wchar_t*>(name);  // 允许const wchar_t* 赋值给 wchar_t*

    std::cout << "[CEGMain::AddNewGraph]为新地图检索绘制边界……" << std::endl;
    double minX = (std::numeric_limits<double>::max)(), 
           minY = (std::numeric_limits<double>::max)(), 
           maxX = (std::numeric_limits<double>::min)(), 
           maxY = (std::numeric_limits<double>::min)(),
           vSize = g.getVertexCount();
    for(int i = 0; i < vSize; ++i){
        Vertex* v = g.getVertex(i+1);
        if(!v){
            std::cout << "[CEGMain::AddNewGraph]读取了无效的边。id是" << i+1 << "。\n";
            continue;
        }
        minX = (std::min)(v->position.x, minX);
        minY = (std::min)(v->position.y, minY);
        maxX = (std::max)(v->position.x, maxX);
        maxY = (std::max)(v->position.y, maxY);
    }
    std::cout << "[CEGMain::AddNewGraph]检索绘制边界完成。范围:" << std::endl
              << "t\t\t\t"<< "minX\tminY\tmaxX\tmaxY" << std::endl
              << "t\t\t\t"<< minX << "\t" << minY << "\t" << maxX << "\t" << maxY << std::endl;

    mfc.tabMap->InsertItem((int)this->graphInfos.size(), &item);
    this->graphInfos.emplace_back(
        std::move(g), minX, minY, maxX, maxY
    );
}

void CEGMain::PaintEdge(Vec2 begin, Vec2 end, COLORREF color, int stroke){
    RECT rectArea;
    this->mfc.tabMap->GetClientRect(&rectArea);

    // 获取绘制上下文
    CClientDC dc(this->mfc.picctrlPaintArea);
    CPen      pen(PS_SOLID, stroke, color);
    CBrush    brush(color);

    // 保存绘制上下文原来使用的的画笔和画刷。
    // SelectObject()返回指针，指向原来的画笔和画刷。
    CPen*   originalPen   = dc.SelectObject(&pen);
    CBrush* originalBrush = dc.SelectObject(&brush);

    // 绘制直线
    dc.MoveTo(begin.x, begin.y);
    dc.LineTo(end.x  , end.y);
    
    // 使用原来的画笔和画刷
    dc.SelectObject(originalPen);
    dc.SelectObject(originalBrush);

}

void CEGMain::PaintVertex(Vec2 pos, VertexType type, bool isOnFire, bool isSelected){
    CClientDC dc(this->mfc.picctrlPaintArea);
    COLORREF color;
    const wchar_t* result;
    switch(type){
    case VertexType::DORMITORY:
        result = CEGMain::kVERTEX_DORMITORY;
        color  = this->isDarkMode ? CEGMain::kCOLOR_DORM_D         : CEGMain::kCOLOR_DORM_W         ;
        break;
    case VertexType::EXIT:
        result = CEGMain::kVERTEX_EXIT;
        color  = this->isDarkMode ? CEGMain::kCOLOR_EXIT_D         : CEGMain::kCOLOR_EXIT_W         ;
        break;
    case VertexType::FLOOR_NODE:
        result = CEGMain::kVERTEX_FLOORNODE;
        color  = this->isDarkMode ? CEGMain::kCOLOR_FLOORNODE_D    : CEGMain::kCOLOR_FLOORNODE_W    ;
        break;
    case VertexType::TEACHING_BUILDING:
        result = CEGMain::kVERTEX_TBUILDING;
        color  = this->isDarkMode ? CEGMain::kCOLOR_TBUILDING_D    : CEGMain::kCOLOR_TBUILDING_W    ;
        break;
    case VertexType::INTERMEDIATE:
        result = CEGMain::kVERTEX_INTERM;
        color = this->isDarkMode ? CEGMain::kCOLOR_INTERM_D        : CEGMain::kCOLOR_INTERM_W       ;
        break;
    case VertexType::UNKNOWN:
    default:
        result = CEGMain::kVERTEX_UNKNOWN;
        color  = this->isDarkMode ? CEGMain::kCOLOR_UNKNOWN_D      : CEGMain::kCOLOR_UNKNOWN_W      ;
        break;
    }
    Gdiplus::Graphics   graph(dc);
    Gdiplus::SolidBrush brush(CEGMain::ToGDIPColor(color));
    int inflate = isSelected ? 8 : 0;

    Gdiplus::Image image(result);
    Gdiplus::Rect rectBtm(pos.x-18, pos.y-18, 36, 36);
    rectBtm.Inflate(inflate, inflate);
    if(isOnFire){
        const Gdiplus::SolidBrush brushFire(CEGMain::ToGDIPColor(kCOLOR_ON_FIRE));
        Gdiplus::Rect rectOnFire(rectBtm);
        rectOnFire.Inflate(5, 5);
        graph.FillRectangle(&brushFire, rectOnFire);
    }
    graph.FillRectangle(&brush, rectBtm);

    graph.DrawImage(
        &image, pos.x-16 - inflate/2, 
        pos.y-16 - inflate/2, 
        32 + inflate, 
        32 + inflate
    );
}

CString CEGMain::BrowseForFile(LPCTSTR lpszTitle){
    CString strSelectedPath;
    static CString s_lastUsedPath; // 静态变量保存上一次成功路径
    TCHAR szPath[MAX_PATH] = {0};

    // 获取默认路径：优先使用上一次成功路径，否则使用程序目录
    CString strDefaultPath;
    if (!s_lastUsedPath.IsEmpty()) {
        strDefaultPath = s_lastUsedPath;
    } else {
        // 获取当前可执行文件的完整路径
        if(GetModuleFileName(NULL, szPath, MAX_PATH) > 0){
            // 去除文件名，只保留目录
            TCHAR* pszLastSlash = _tcsrchr(szPath, _T('\\'));
            if(pszLastSlash != NULL){
                *pszLastSlash = _T('\0');  // 截断，只保留目录部分
            }
        }
        strDefaultPath = CString(szPath);
    }
    std::cout << "[CEGMain::BrowseForFile]使用上一次目录访问："
              << std::string(CW2A(s_lastUsedPath)) << std::endl;

    IFileDialog* pFileDlg = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pFileDlg));

    if(SUCCEEDED(hr)){
        // 设置为文件选择模式并添加过滤
        DWORD dwOptions;
        pFileDlg->GetOptions(&dwOptions);
        pFileDlg->SetOptions(dwOptions & ~FOS_PICKFOLDERS | FOS_FILEMUSTEXIST); // 移除文件夹选择，添加文件必须存在

        // 设置文件类型过滤
        COMDLG_FILTERSPEC fileTypes[] = {
            { L"专属地图文件 (*.map)", L"*.map" }
        };
        pFileDlg->SetFileTypes(_countof(fileTypes), fileTypes);
        pFileDlg->SetDefaultExtension(L"map"); // 设置默认扩展名

        // 设置标题
        if(lpszTitle != NULL){
            pFileDlg->SetTitle(lpszTitle);
        }

        // 设置默认文件夹为程序运行目录
        if(!strDefaultPath.IsEmpty()){
            IShellItem* pDefaultFolder = NULL;
            HRESULT hrPath = SHCreateItemFromParsingName(
                strDefaultPath,
                NULL,
                IID_PPV_ARGS(&pDefaultFolder)
            );

            if(SUCCEEDED(hrPath)){
                pFileDlg->SetFolder(pDefaultFolder);
                pDefaultFolder->Release();
            }
        }

        // 显示对话框
        if(SUCCEEDED(pFileDlg->Show(NULL))){
            IShellItem* shellItem = NULL;
            if(SUCCEEDED(pFileDlg->GetResult(&shellItem))){
                PWSTR pszPath = NULL;
                if(SUCCEEDED(shellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath))){
                    strSelectedPath = pszPath;
                    CoTaskMemFree(pszPath);

                    // 更新上一次使用路径（保存文件所在目录）
                    s_lastUsedPath = strSelectedPath;
                    TCHAR* pszLastSlash = _tcsrchr(s_lastUsedPath.GetBuffer(), _T('\\'));
                    if (pszLastSlash != NULL) {
                        *pszLastSlash = _T('\0');
                    }
                    strSelectedPath.ReleaseBuffer();
                }
                shellItem->Release();
            }
        }
        pFileDlg->Release();
    }

    std::cout << "[CEGMain::BrowseForFile]用户选择的路径为："
        << std::string(CW2A(strSelectedPath)) << std::endl;

    return strSelectedPath;
}

void CEGMain::PaintText(
    Vec2 pos, int textSize, bool doRemoveBkg,
    const char** texts, const COLORREF colors[], int num,
    Gdiplus::StringAlignment aHorizonal, 
    Gdiplus::StringAlignment aVertical
){
    CClientDC dc(this->mfc.picctrlPaintArea);
    Gdiplus::Graphics graph(dc);
    Gdiplus::Font     font(L"SimSum", textSize/2.4f, Gdiplus::FontStyleBold);
    Gdiplus::SolidBrush brush(Gdiplus::Color::Chocolate); 
    Gdiplus::StringFormat format;
    format.SetAlignment(aHorizonal);
    format.SetLineAlignment(aVertical);
    for(int i = 0; i < num; ++i){
        const std::wstring    text  = CEGMain::GbkToUtf16(texts[i]);
        const Gdiplus::Color  color = CEGMain::ToGDIPColor(colors[i]);
        const Gdiplus::PointF point(pos.x, pos.y + textSize * (2 * i - num + 1) / 2 );
        brush.SetColor(color);
        if(doRemoveBkg){
            COLORREF colorbkg = this->isDarkMode
                ? CEGMain::kCOLOR_BKG_D
                : CEGMain::kCOLOR_BKG_W;
            Gdiplus::SolidBrush brushBkg(CEGMain::ToGDIPColor(colorbkg, 200));
            Gdiplus::RectF      rectBox;
            graph.MeasureString(text.c_str(), text.length(), &font, point, &rectBox);
            rectBox.X -= rectBox.Width  / 2.0f;
            rectBox.Y -= rectBox.Height / 2.0f;
            graph.FillRectangle(&brushBkg, rectBox);
        }
        graph.DrawString(text.c_str(), text.length(), &font, point, &format, &brush);
    }
}

void CEGMain::PaintVertexes(){
    std::cout << "[CEGMain::PaintVertexes]绘制所有顶点……";
    CRect rectPaint;
    this->mfc.picctrlPaintArea->GetClientRect(&rectPaint);
    rectPaint.DeflateRect(
        CEGMain::kLENGTH_MARGIN, CEGMain::kLENGTH_MARGIN,
        CEGMain::kLENGTH_MARGIN, CEGMain::kLENGTH_MARGIN
    );
    rectPaint.top += this->GetTabHeight();
    const auto& gInfo      = this->graphInfos.at(this->currentIdx);
    const int&  vSize      = gInfo.graph.getVertexCount();
    int selBegin = this->mfc.choiceEscBegin->GetCurSel();
        selBegin = (selBegin > 0 ) ? gInfo.escBegin[selBegin - 1] : -1;
    int selEnd   = this->mfc.choiceEscEnd->GetCurSel();
        selEnd   = (selEnd > 1   ) ? gInfo.escEnd[selEnd - 2    ] : -1;
    for(int i = 1; i <= vSize; i++){
        std::cout << i << ", ";
        Vec2 rPos = this->GetTransformedPosition(
            gInfo.graph.getVertex(i)->position, gInfo, rectPaint
        );
        const Vertex*             v = gInfo.graph.getVertex(i);
        const bool isSelected     = i == selBegin || i == selEnd,
                   isSurelyOnFire = v->isBurning && this->isOnFire;

        this->PaintVertex(
            rPos, v->type, 
            isSurelyOnFire,
            isSelected
        );
        if(this->mfc.checkVertexName->GetCheck() == BST_CHECKED){
            const char* texts[1] = {v->name.c_str()};
            COLORREF   colors[1] = {RGB(255, 255, 255)};
            switch(v->type){
            case VertexType::TEACHING_BUILDING:
                colors[0] = this->isDarkMode ? CEGMain::kCOLOR_TBUILDING_D  : CEGMain::kCOLOR_TBUILDING_W   ;
                break;
            case VertexType::DORMITORY:
                colors[0] = this->isDarkMode ? CEGMain::kCOLOR_DORM_D       : CEGMain::kCOLOR_DORM_W        ;
                break;
            case VertexType::EXIT:
                colors[0] = this->isDarkMode ? CEGMain::kCOLOR_EXIT_D       : CEGMain::kCOLOR_EXIT_W        ;
                break;
            case VertexType::FLOOR_NODE:
                colors[0] = this->isDarkMode ? CEGMain::kCOLOR_FLOORNODE_D : CEGMain::kCOLOR_FLOORNODE_W    ;
                break;
            case VertexType::INTERMEDIATE:
                colors[0] = this->isDarkMode ? CEGMain::kCOLOR_INTERM_D    : CEGMain::kCOLOR_INTERM_W       ;
                break;
            case VertexType::UNKNOWN:
            default:
                colors[0] = this->isDarkMode ? CEGMain::kCOLOR_UNKNOWN_D : CEGMain::kCOLOR_UNKNOWN_W;
                break;
            }
            bool isBeneathMidTop = rPos.y < (rectPaint.top * 3 + rectPaint.bottom) / 4;
            Vec2 pText(0, (isBeneathMidTop ? 1 : -1) * ((isSelected ? 40 : 30) + (isSurelyOnFire ? 6:0)) );
            this->PaintText(rPos + pText, (isSelected ? 24 : 18), true, texts, colors, 1,
                Gdiplus::StringAlignmentCenter
            );
        }
        std::cout << "\b\b[V], ";
    }
    std::cout <<"\b\b。"<< std::endl;
}

void CEGMain::PaintEdges(){
    std::cout << "[CEGMain::PaintEdges]绘制所有边……";
    CRect rectPaint, rectTab;
    this->mfc.picctrlPaintArea->GetClientRect(&rectPaint);
    rectPaint.DeflateRect(
        CEGMain::kLENGTH_MARGIN, CEGMain::kLENGTH_MARGIN,
        CEGMain::kLENGTH_MARGIN, CEGMain::kLENGTH_MARGIN
    );

    rectPaint.top += this->GetTabHeight();
    const GraphInfo& gInfo = this->graphInfos.at(this->currentIdx);
    const size_t&    vSize = gInfo.graph.getVertexCount();

    // 编写哈希函数，用于unordered_set，检查当前边是否已绘制。
    //struct PairHash{
    //    size_t operator()(const std::pair<int, int>& p) const {
    //        return (static_cast<size_t>(p.first) < 32) ^ static_cast<size_t>(p.second);
    //    }
    //};
    //std::unordered_set<std::pair<int, int>, PairHash> checkEdge;

    auto _PaintEdge = [/*&checkEdge,*/ this, &gInfo, &rectPaint](
        const Edge& e, const COLORREF& color, int stroke, bool doCheckPainted = true
    ) ->void {	
        const int &from = e.from, &to = e.to;

        std::pair<int, int> edge = (from < to) ? std::make_pair(from, to) : std::make_pair(to, from);
        //if(doCheckPainted && checkEdge.find(edge) != checkEdge.end())return;
        std::cout << from << "->" << to << ", ";

        Vec2 begin = this->GetTransformedPosition(
            gInfo.graph.getVertex(from)->position, gInfo, rectPaint
        );
        Vec2 end = this->GetTransformedPosition(
            gInfo.graph.getVertex(to)->position, gInfo, rectPaint
        );

        this->PaintEdge(begin, end, color, stroke);

        char** texts = new char*[3]{
            new char[26]{}, new char[26]{}, new char[26]{}
        };
        COLORREF colors[3] = {};
        int totalLen = 0;
        if(this->mfc.checkRoadLen->GetCheck() == BST_CHECKED){
            std::sprintf(texts[totalLen], "路径长：%.2f米", e.distance);
            colors[totalLen] = RGB(128, 128, 128);
            ++totalLen;
        }
        if(this->mfc.checkSafety->GetCheck() == BST_CHECKED){
            std::sprintf(texts[totalLen], "安全度：%d", e.riskLevel);
            colors[totalLen] = RGB(128, 128, 128);
            ++totalLen;
        }if(this->mfc.checkCrowding->GetCheck() == BST_CHECKED){
            std::sprintf(texts[totalLen], "拥挤度：%.1f", e.congestion);
            colors[totalLen] = RGB(128, 128, 128);
            ++totalLen;
        }
        Vec2 drawPnt((begin.x + end.x) / 2, (begin.y + end.y) / 2);

        if(this->mfc.checkRandTag->GetCheck() == BST_CHECKED){
            srand(time(nullptr) + rand()*rand() + 3);
            int Rand = rand() % 7 + 2; // (2, 8)
            drawPnt.x = (begin.x * Rand + end.x*(10 - Rand)) / 10;
            drawPnt.y = (begin.y * Rand + end.y*(10 - Rand)) / 10;
        }
        this->PaintText(drawPnt, 16, true, const_cast<const char**>(texts), colors, totalLen);

        //checkEdge.insert(edge);
        std::cout << "\b\b[V], ";

        for(int i = 0; i < 3; i++)delete[] texts[i];
        delete[] texts;
    };

    // 漏洞：逃生路可能会给正常路径覆盖。
    // 绘制正常路径.

    std::cout << "正常路径：";
    for(int i = 1; i <= vSize; i++){
        const std::vector<Edge>& edges = gInfo.graph.getEdgesFrom(i);
        for(const auto& edge : edges){
            _PaintEdge(edge, RGB(128, 128, 128), 2);
        }
    }
    std::cout << "\b\b。" << std::endl;

    // 绘制逃脱路径。
    std::cout << "逃生路径：";
    int stIdx = this->mfc.choiceEscBegin->GetCurSel(),
        edIdx = this->mfc.choiceEscEnd->GetCurSel();
    if(stIdx != 0 && edIdx != 0){
        if(edIdx != 1){	// 不是全部绘制
            const auto& edges = this->paths[edIdx - 2].edges;
            for(const auto& edge : edges){
                _PaintEdge(edge, CEGMain::kCOLOR_PATH_HINT, 4);
            }
        }else{			// 绘制全部路径
            for(int i = 0; i < this->paths.size(); ++i){
                const auto& edges = this->paths[i].edges;
                const int stroke = (i == 0)? 8 : 4;
                for(const auto& edge : edges){
                    _PaintEdge(edge, CEGMain::kCOLOR_PATH_HINT, stroke);
                }
            }
        }
    }
}
void CEGMain::UpdateGraph(){
    std::cout << "[CEGMain::UpdateGraph]更新当前绘图内容……" << std::endl;
    CClientDC dc(this->mfc.picctrlPaintArea);
    // CRect     rectPaintArea;
    // CDC       memDC;
    // CBitmap   bitmap;
    // this->mfc.picctrlPaintArea.GetP
    // memDC.CreateCompatibleDC(&dc);
    // bitmap.CreateCompatibleBitmap(&dc, 

    this->ClearGraphDisplay();

    if(this->graphInfos.empty()){
        std::cout << "[CEGMain::UpdateGraph]无图可绘制。" << std::endl;
        this->mfc.tabMap->ShowWindow(SW_HIDE);
        CRect rectPaint;
        this->mfc.picctrlPaintArea->GetClientRect(&rectPaint);

        CFont font;
        font.CreatePointFont(120, _T("微软雅黑"));
        CFont* originalFont = dc.SelectObject(&font);

        dc.SetTextColor(RGB(128, 128, 128));
        dc.SetBkMode(TRANSPARENT);
        dc.DrawText(_T("请加载一个图！"), &rectPaint, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        dc.SelectObject(originalFont);
        return;
    }else{
        this->mfc.tabMap->ShowWindow(SW_SHOW);
        // 修改标签栏高度
        CRect rectTab, rectPaint;
        this->mfc.tabMap->GetWindowRect(&rectTab);
        ScreenToClient(&rectTab);
        rectTab.bottom = rectTab.top + this->GetTabHeight();
        this->mfc.tabMap->MoveWindow(&rectTab);

        this->PaintEdges();
        this->PaintVertexes();
    }
}

void CEGMain::UpdateGraphLater(){
    this->SetTimer(CEGMain::kTIMER_UPDATE_GRAPH_LATER, 1, NULL);
}

void CEGMain::ClearGraphDisplay(){
    std::cout << "[CEGMain::ClearGraphDisplay]清除当前绘图内容……" << std::endl;
    CClientDC dc(mfc.picctrlPaintArea);
    CRect rectPaint, rectTab;

    int tabHeight = this->mfc.tabMap->GetItemRect(0, rectTab) ? rectTab.Height() : 0;
    this->mfc.picctrlPaintArea->GetClientRect(&rectPaint);
    rectPaint.DeflateRect(1, tabHeight + 1, 1, 1);

    dc.FillSolidRect(&rectPaint, this->isDarkMode ? kCOLOR_BKG_D : kCOLOR_BKG_W);
}

Vec2 CEGMain::GetTransformedPosition(
    Vec2 oPos, const GraphInfo& gInfo,const CRect& rectTM
){
    oPos.x = (oPos.x - gInfo.minX)*(rectTM.right - rectTM.left) 
           / (gInfo.maxX - gInfo.minX) + rectTM.left;
    oPos.y = (oPos.y - gInfo.minY)*(rectTM.bottom - rectTM.top) 
           / (gInfo.maxY - gInfo.minY) + rectTM.top;
    return oPos;
}

int CEGMain::GetTabHeight() const{
    CRect rect;
    return this->mfc.tabMap->GetItemRect(0, &rect) ? rect.Height() : 0;
}

void CEGMain::OnClickedBtnLoad(){
    std::cout << "[CEGMain::OnCLickedBtnLoad]加载地图文件" << std::endl;
    // TODO: 在此添加控件添加地图加载代码.
    CString strFilePath = BrowseForFile(_T("选择地图文件"));
    bool isPreviousGraphInfoEmpty = this->graphInfos.empty();

    if(strFilePath.IsEmpty())return;
    Graph g;

    if(g.generateList(CW2A(strFilePath)) ){
        std::wstring name = strFilePath.Mid(strFilePath.ReverseFind('\\') + 1);
        this->AddNewGraph(g, name.c_str());
        if(isPreviousGraphInfoEmpty){
            this->SetCurrentPage(0);
        }
    }else{
        MessageBox(
            _T("当前文件夹下未加载任何图。"), _T("加载校园图错误"), MB_ICONERROR | MB_OK
        );
    }
    strFilePath.ReleaseBuffer();
}

void CEGMain::SetCurrentPage(int page){
    if(this->currentIdx == page)return;
    else this->currentIdx = page;
    std::cout << "[CEGMain::SetCurrentPage]切换到索引界面" << page << "。" << std::endl;

    if(this->currentIdx != -1){
        const GraphInfo& info = this->graphInfos.at(page);

        this->mfc.choiceEscBegin->ResetContent();
        this->mfc.choiceEscBegin->AddString(CEGMain::kSTR_VERTEX_NONE);	// 0
        for(const int& vid : info.escBegin){
            const Vertex* v = info.graph.getVertex(vid);
            this->mfc.choiceEscBegin->AddString(
                this->GbkToUtf16(v->name.c_str()).c_str()
            );
        }
        this->mfc.choiceEscBegin->SetCurSel(0);

        this->mfc.choiceEscEnd->ResetContent();
        this->mfc.choiceEscEnd->AddString(CEGMain::kSTR_VERTEX_NONE);		// 0
        this->mfc.choiceEscEnd->AddString(CEGMain::kSTR_VERTEX_ALL_EXIT);	// 1
        for(const int& vid : info.escEnd){
            const Vertex* v = info.graph.getVertex(vid);
            std::string name = ((vid == info.escEnd.front()) ? "★" : "") + v->name;
            this->mfc.choiceEscEnd->AddString(
                this->GbkToUtf16(name.c_str()).c_str()
            );
        }
        this->mfc.choiceEscEnd->SetCurSel(0);
        
        this->mfc.btnDelGraph->EnableWindow(SW_SHOW);
        this->mfc.choiceEscBegin->EnableWindow(SW_SHOW);
        this->mfc.checkOnFire->EnableWindow(SW_SHOW);
    }else{
        this->mfc.choiceEscBegin->ResetContent();
        this->mfc.choiceEscEnd->ResetContent();
        this->mfc.btnDelGraph->EnableWindow(SW_HIDE);
        this->mfc.choiceEscBegin->EnableWindow(SW_HIDE);
        this->mfc.checkOnFire->EnableWindow(SW_HIDE);
    }
    std::cout << "（以下为自动调用）" << std::endl;
    this->OnChangeChoiceEscBegin();
    //this->UpdateGraph();  // 这个会在OnChangeChoiceEscBegin中调用。
    std::cout << "（结束自动调用）" << std::endl;
}

int CEGMain::GetCurrentPage() const{
    return this->currentIdx;
}

CEGMain::GraphInfo::GraphInfo(Graph&& g, double minx, double miny, double maxx, double maxy)
    : graph(std::move(g)), minX(minx), minY(miny), maxX(maxx), maxY(maxy)
{
    this->escEnd = this->graph.getVerticesByType(VertexType::EXIT);
    int vCnt     = this->graph.getVertexCount();
    this->escBegin.resize(vCnt - this->escEnd.size());
    for(int i = 0, j = 0; i < vCnt; ++i){
        if(this->graph.getVertex(i + 1)->type == VertexType::EXIT)continue;
        this->escBegin[j] = i+1;
        ++j;
    }
}

void CEGMain::OnCickedBtnDel(){
    std::cout << "删除当前图：" << this->currentIdx << std::endl;
    this->mfc.tabMap->DeleteItem(this->currentIdx);
    this->graphInfos.erase(this->graphInfos.begin() + this->currentIdx);
    this->SetCurrentPage(this->currentIdx <= 0 // 一开始选择的是第一个图吗？
        ? (this->graphInfos.empty() ? -1 : 0)  // 如果还有图，就保持为0，否则为无效
        : this->currentIdx - 1				   // 自动设置为上一个图
    );
}

void CEGMain::OnChangeChoiceEscBegin(){
    std::cout << "[CEGMain::OnChangeChoiceEscBegin]修改了目前的起点" 
              << this->mfc.choiceEscBegin->GetCurSel()
              << "。"<<std::endl;
    if(this->currentIdx != -1){
        this->paths.clear();
        const int curSel = this->mfc.choiceEscBegin->GetCurSel();
        if(curSel == 0){
            this->mfc.choiceEscEnd->EnableWindow(SW_HIDE);
        }else{
            // 获取当前点到所有出口的路径+长度，并将最短路径防止于开头。
            this->mfc.choiceEscEnd->EnableWindow(SW_SHOW);
            const GraphInfo& info = this->graphInfos.at(this->currentIdx);
        
            auto exitID_len_Path = info.graph.findMultipleExits(
                info.escBegin[curSel-1], this->isOnFire);

            this->paths.reserve(exitID_len_Path.size());
            for(auto& item : exitID_len_Path){
                this->paths.emplace_back(
                    std::move(item.path), 
                    std::move(item.edges), 
                    item.length
                );
            }
        }
    }else{
        this->mfc.choiceEscBegin->EnableWindow(SW_HIDE);
        this->mfc.choiceEscEnd->EnableWindow(SW_HIDE);
        this->mfc.choiceEscEnd->SetCurSel(0);
    }
    this->UpdateGraph();
}

void CEGMain::OnChangeChoiceEscEnd(){
    std::cout << "[CEGMain::OnChangeChoiceEscEnd]修改了目前的终点" 
              << this->mfc.choiceEscEnd->GetCurSel()
              << "。" << std::endl;
    this->UpdateGraph();
}

void CEGMain::OnCheckOnFire(){
    this->isOnFire = this->mfc.checkOnFire->GetCheck();
    std::cout << "[CEGMain::OnCheckOnFire]设置着火为：" 
              << this->isOnFire << "。"<<std::endl;

    std::cout << "（以下为自动调用）" << std::endl;
    this->OnChangeChoiceEscBegin();
    std::cout << "（结束自动调用）" << std::endl;
}

CEGMain::PathInfo::PathInfo(std::vector<int>&& path, std::vector<Edge>&& edges, double len)
    : path(std::move(path)), edges(std::move(edges)), length(len){}

void CEGMain::OnBnClickedCheckSafety(){
    std::cout << "[CEGMain::OnCheckOnFire]设置着火为："
              << this->isOnFire << "。" << std::endl;
    this->UpdateGraph();
}

void CEGMain::OnClickedCheckRoadlen(){
    bool isChecked = this->mfc.checkRoadLen->GetCheck() == BST_CHECKED;
    std::cout << "[CEGMain::OnClickedCheckRoadlen]边属性——显示路径长度："
              << isChecked << "。" << std::endl;
    this->UpdateGraph();
}

void CEGMain::OnBnClickedCheckCrowding(){
    bool isChecked = this->mfc.checkCrowding->GetCheck() == BST_CHECKED;
    std::cout << "[CEGMain::OnClickedCheckRoadlen]边属性——显示拥挤程度1："
              << isChecked << "。" << std::endl;
    this->UpdateGraph();
}

void CEGMain::OnClickedCheckVertexName(){
    bool isChecked = this->mfc.checkVertexName->GetCheck() == BST_CHECKED;
    std::cout << "[CEGMain::OnClickedCheckRoadlen]点属性——显示节点名称："
              << isChecked << "。" << std::endl;
    this->UpdateGraph();
}

Gdiplus::Color CEGMain::ToGDIPColor(const COLORREF& color, BYTE a){
    return Gdiplus::Color(
        a,
        GetRValue(color),
        GetGValue(color),
        GetBValue(color)
    );
}

void CEGMain::OnClickedCheckRandTag(){
    bool isChecked = this->mfc.checkRandTag->GetCheck() == BST_CHECKED;
    std::cout << "[CEGMain::OnClickedCheckRoadlen]点属性——显示节点名称："
        << isChecked << "。" << std::endl;
    this->UpdateGraph();
}
