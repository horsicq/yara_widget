/* Copyright (c) 2023 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef YARA_WIDGET_H
#define YARA_WIDGET_H

#include <QFutureWatcher>
#include <QWidget>
#include <QtConcurrent>
#include "xshortcutswidget.h"
#ifdef USE_YARA
#include "xyaradialogprocess.h"
#endif
#include "dialogtextinfo.h"
#include "dialogyarawidgetadvanced.h"

namespace Ui {
class YARA_Widget;
}

class YARA_Widget : public XShortcutsWidget {
    Q_OBJECT

public:
    struct OPTIONS {
        bool bHandleInfo;
    };

    explicit YARA_Widget(QWidget *pParent = nullptr);
    ~YARA_Widget();

    void setData(const QString &sFileName, OPTIONS options, bool bScan = false);
#ifdef USE_YARA
    static void setResultToTreeView(QTreeView *pTreeView, XYara::SCAN_RESULT *pScanResult);
#endif
    void adjustView();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);

protected:
    virtual void registerShortcuts(bool bState);

signals:
    void scanStarted();
    void scanFinished();
    void showInfo();

private slots:
    void on_pushButtonYaraScan_clicked();
    void clear();
    void process();
    void scan();
    void stop();
    void on_scanFinished();
    void enableControls(bool bState);
    void on_pushButtonYaraExtraInformation_clicked();
    void on_pushButtonRules_clicked();
    void on_pushButtonYaraInfo_clicked();

private:
    Ui::YARA_Widget *ui;
#ifdef USE_YARA
    XYara g_xyara;
    XYara::SCAN_RESULT g_scanResult;
#endif
    QString g_sFileName;
    OPTIONS g_options;
    QFutureWatcher<void> g_watcher;
    XBinary::PDSTRUCT g_pdStruct;
    bool g_bProcess;
};

#endif  // YARA_WIDGET_H
