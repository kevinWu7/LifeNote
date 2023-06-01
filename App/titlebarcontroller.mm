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
    window.backgroundColor = [NSColor colorWithRed:red green:green blue:blue alpha:0.1];
}

void setWindowTitleMerged(WId winId) {
    if (winId == 0) return;
    NSView* view = (NSView*)winId;
    NSWindow* window = [view window];
    NSWindowStyleMask windowMask = NSWindowStyleMaskFullSizeContentView
            | NSWindowStyleMaskBorderless
            | NSWindowStyleMaskTitled
            | NSWindowStyleMaskClosable
            | NSWindowStyleMaskMiniaturizable
            | NSWindowStyleMaskResizable;
    [window setStyleMask: windowMask];
   // NSWindowStyleMask windowMask = NSWindowStyleMaskFullSizeContentView ;
    [window setStyleMask:windowMask];
}


//该方法修改的实际上只是window本身的高度，并没有修改到titlebar的高度
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
