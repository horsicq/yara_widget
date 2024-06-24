/* Copyright (c) 2023-2024 hors<horsicq@gmail.com>
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
#ifndef YARAWIDGETADVANCED_H
#define YARAWIDGETADVANCED_H

#include "xshortcutswidget.h"
#include "yara_widget.h"
#ifdef USE_YARA
#include "xyaradialogprocess.h"
#endif

namespace Ui {
class YARAWidgetAdvanced;
}

class YARAWidgetAdvanced : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit YARAWidgetAdvanced(QWidget *pParent = nullptr);
    ~YARAWidgetAdvanced();

    void setData(const QString &sFileName, bool bScan = false);
    virtual void adjustView();
    void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);

signals:
    void showHex(qint64 nOffset, qint64 nSize);

protected:
    virtual void registerShortcuts(bool bState);

private slots:
    void process();
    void on_pushButtonSave_clicked();
    void onSelectionChanged(const QItemSelection &itemSelected, const QItemSelection &itemDeselected);
    void pushButtonSlot();
    void on_pushButtonScan_clicked();
    void on_pushButtonRules_clicked();

private:
    Ui::YARAWidgetAdvanced *ui;
    QString g_sFileName;
#ifdef USE_YARA
    XYara::SCAN_RESULT g_scanResult;
#endif
};

#endif  // YARAWIDGETADVANCED_H
