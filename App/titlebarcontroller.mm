#include <Cocoa/Cocoa.h>
#include <QWidget>
#include <QString>

namespace Cocoa
{
double extractValueFromQString(const QString& qstr,int index)
{
    // Remove the "rgb(" and ")" parts from the QString
    QString trimmedStr = qstr.mid(4, qstr.length() - 5);

    // Split the QString into a list of substrings based on the comma separator
    QStringList parts = trimmedStr.split(',');

    if (parts.size() != 3) {
        qWarning() << "Invalid input format. Expected format: 'rgb(r, g, b)'";
        return 0.0;  // or any appropriate default value
    }

    bool conversionOk = false;
    double value = parts[index].toDouble(&conversionOk);

    if (!conversionOk) {
        qWarning() << "Failed to convert value to double.";
        return 0.0;  // or any appropriate default value
    }
    return value/255;
}

//这也也是个半调子，方法，通过设置整个window的background来设置的，不是很好
void changeTitleBarColor(WId winId, QString color)
{
    if (winId == 0) return;
    NSView* view = (NSView*)winId;
    NSWindow* window = [view window];
    window.titlebarAppearsTransparent = YES;
    double red=extractValueFromQString(color,0);
    double green=extractValueFromQString(color,1);
    double blue=extractValueFromQString(color,2);
    window.backgroundColor = [NSColor colorWithRed:red green:green blue:blue alpha:1.];
}

//这个方法不生效，todo
void changeTitleBarTextColor(WId winId, QString color)
{
    if (winId == 0) return;
    NSView* view = (NSView*)winId;
    NSWindow* window = [view window];

    // 从 QString 中提取颜色分量
    double red = extractValueFromQString(color, 0);
    double green = extractValueFromQString(color, 1);
    double blue = extractValueFromQString(color, 2);

    // 创建新的颜色对象
    NSColor* textColor = [NSColor colorWithRed:red green:green blue:blue alpha:1.0];

    // 获取标题栏按钮的标题属性字典
    NSAttributedString* attributedTitle = [window standardWindowButton:NSWindowCloseButton].attributedTitle;
    NSRange effectiveRange;
    NSMutableDictionary* titleAttributes = [NSMutableDictionary dictionaryWithDictionary:[attributedTitle attributesAtIndex:0 effectiveRange:&effectiveRange]];

    // 设置标题栏文字颜色
    [titleAttributes setObject:textColor forKey:NSForegroundColorAttributeName];

    // 创建新的带有修改属性的 NSAttributedString
    NSMutableAttributedString* newAttributedTitle = [[NSMutableAttributedString alloc] initWithAttributedString:attributedTitle];
    [newAttributedTitle setAttributes:titleAttributes range:effectiveRange];

    // 设置新的标题栏文字
    [window standardWindowButton:NSWindowCloseButton].attributedTitle = newAttributedTitle;
}



void changeTitleBarHeight(WId winId, double height)
{
    if (winId == 0) return;
    NSView* view = (NSView*)winId;
    NSWindow* window = [view window];

    NSRect frame = [window frame];
    NSRect contentRect = [window contentRectForFrameRect:frame];
    CGFloat titleBarHeight = NSHeight(frame) - NSHeight(contentRect);

    // Calculate new frame
    NSRect newFrame = frame;
    newFrame.size.height = height + titleBarHeight;

    // Set new frame for the window
    [window setFrame:newFrame display:YES];
}








}
