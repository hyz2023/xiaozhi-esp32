#ifndef BASE_WIDGET_H
#define BASE_WIDGET_H

#include <lvgl.h>
#include <string>

// 前向声明
struct IdleTheme;
struct LayoutConfig;

// 组件基类
class BaseWidget {
public:
    BaseWidget(lv_obj_t* parent, const std::string& name);
    virtual ~BaseWidget();
    
    // 核心接口
    virtual void Update() = 0;
    virtual void SetVisible(bool visible);
    virtual void ApplyTheme(const IdleTheme& theme);
    virtual void SetPosition(int x, int y);
    virtual void SetSize(int width, int height);
    
    // 状态查询
    bool IsVisible() const { return is_visible_; }
    const std::string& GetName() const { return name_; }
    lv_obj_t* GetContainer() const { return container_; }
    
    // 布局相关
    virtual void UpdateLayout(const LayoutConfig& layout);
    virtual int GetPreferredWidth() const = 0;
    virtual int GetPreferredHeight() const = 0;
    
    // 工具方法
    void SetBorder(bool enabled, uint32_t color = 0xCCCCCC, int width = 1);
    void SetBackground(uint32_t color);
    void SetPadding(int left, int top, int right, int bottom);

protected:
    // 子类可访问的成员
    lv_obj_t* container_ = nullptr;
    lv_obj_t* parent_ = nullptr;
    std::string name_;
    bool is_visible_ = true;
    
    // 布局信息
    int x_ = 0;
    int y_ = 0;
    int width_ = 0;
    int height_ = 0;
    
    // 主题相关 - 改为指针类型
    IdleTheme* current_theme_ = nullptr;
    
    // 工具方法
    void CreateContainer();
    void DestroyContainer();
    void UpdateContainerStyle();
    
    // 样式管理
    lv_style_t* GetWidgetStyle();
    void ApplyWidgetStyle();
    
private:
    lv_style_t widget_style_;
    bool style_initialized_ = false;
};

#endif // BASE_WIDGET_H 