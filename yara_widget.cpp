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
#include "yara_widget.h"

#include "ui_yara_widget.h"

YARA_Widget::YARA_Widget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::YARA_Widget)
{
    ui->setupUi(this);

    g_pdStruct = XBinary::createPdStruct();

    connect(&g_watcher, SIGNAL(finished()), this, SLOT(on_scanFinished()));

    clear();

    ui->progressBarScan->hide();
}

YARA_Widget::~YARA_Widget()
{
    if (g_bProcess) {
        stop();
        g_watcher.waitForFinished();
    }

    delete ui;
}

void YARA_Widget::setData(const QString &sFileName, bool bScan)
{
    g_sFileName = sFileName;

    if (bScan) {
        process();
    }
}
#ifdef USE_YARA
void YARA_Widget::setResultToTreeView(QTreeView *pTreeView, XYara::SCAN_RESULT *pScanResult)
{
    // TODO delete old model
    qint32 nNumberOfRecords = pScanResult->listRecords.count();

    QStandardItemModel *pModel = new QStandardItemModel;

    QMap<QString, QStandardItem *> mapParents;

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        QStandardItem *pItemParent = nullptr;

        if (!mapParents.contains(pScanResult->listRecords.at(i).sRulesFile)) {
            pItemParent = new QStandardItem;
            pItemParent->setText(pScanResult->listRecords.at(i).sRulesFile);
            pModel->appendRow(pItemParent);
            mapParents.insert(pScanResult->listRecords.at(i).sRulesFile, pItemParent);
        } else {
            pItemParent = mapParents.value(pScanResult->listRecords.at(i).sRulesFile);
        }

        QStandardItem *pItem = new QStandardItem;
        pItem->setText(pScanResult->listRecords.at(i).sRule);
        pItem->setData(pScanResult->listRecords.at(i).sUUID, Qt::UserRole + 1);

        pItemParent->appendRow(pItem);
    }

    pTreeView->setModel(pModel);
    pTreeView->expandAll();
}
#endif
void YARA_Widget::adjustView()
{
    // TODO
}

void YARA_Widget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    // TODO
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void YARA_Widget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
    // TODO
}

void YARA_Widget::on_pushButtonYaraScan_clicked()
{
    process();
}

void YARA_Widget::clear()
{
    g_bProcess = false;
#ifdef USE_YARA
    g_scanResult = {};
#endif
}

void YARA_Widget::process()
{
    if (!g_bProcess) {
        g_bProcess = true;
        enableControls(false);

        ui->pushButtonYaraScan->setText(tr("Stop"));

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QFuture<void> future = QtConcurrent::run(&YARA_Widget::scan, this);
#else
        QFuture<void> future = QtConcurrent::run(this, &YARA_Widget::scan);
#endif

        g_watcher.setFuture(future);

//        YARA_Widget::scan();
    } else {
        ui->pushButtonYaraScan->setEnabled(false);
        stop();
        g_watcher.waitForFinished();
        ui->pushButtonYaraScan->setText(tr("Scan"));
        enableControls(true);
    }
}

void YARA_Widget::scan()
{
    emit scanStarted();
#ifdef USE_YARA
    g_pdStruct = XBinary::createPdStruct();

    //    g_xyara.addRulesFile("C:\\tmp_build\\qt5\\Detect-It-Easy\\yara\\packer.yar");
    g_xyara.setData(g_sFileName, getGlobalOptions()->getValue(XOptions::ID_SCAN_YARARULESPATH).toString());
    g_xyara.setPdStruct(&g_pdStruct);

    g_xyara.process();

    g_scanResult = g_xyara.getScanResult();
#endif
    emit scanFinished();
}

void YARA_Widget::stop()
{
    g_pdStruct.bIsStop = true;
}

void YARA_Widget::on_scanFinished()
{
    enableControls(true);
#ifdef USE_YARA
    QAbstractItemModel *pOldModel = ui->treeViewResult->model();

    setResultToTreeView(ui->treeViewResult, &g_scanResult);

    deleteOldAbstractModel(&pOldModel);

    ui->lineEditElapsedTime->setText(QString("%1 %2").arg(QString::number(g_scanResult.nScanTime), tr("msec")));
#endif
    g_bProcess = false;

    ui->pushButtonYaraScan->setEnabled(true);
    ui->pushButtonYaraScan->setText(tr("Scan"));
}

void YARA_Widget::enableControls(bool bState)
{
    // TODO
    if (bState) {
        ui->progressBarScan->hide();
    } else {
        ui->progressBarScan->show();
    }

    ui->treeViewResult->setEnabled(bState);
    ui->pushButtonRules->setEnabled(bState);
    ui->pushButtonYaraExtraInformation->setEnabled(bState);
    ui->pushButtonYaraInfo->setEnabled(bState);
}

void YARA_Widget::on_pushButtonYaraExtraInformation_clicked()
{
    DialogTextInfo dialogInfo(this);

    dialogInfo.setText(XOptions::getTreeModelText(ui->treeViewResult->model()));

    dialogInfo.exec();
}

void YARA_Widget::on_pushButtonRules_clicked()
{
    QString sDirectory = getGlobalOptions()->getValue(XOptions::ID_SCAN_YARARULESPATH).toString();
    sDirectory = XBinary::convertPathName(sDirectory);
    XOptions::showFolder(sDirectory);
}

void YARA_Widget::on_pushButtonYaraInfo_clicked()
{
    DialogYARAWidgetAdvanced dialogYara(this);
    dialogYara.setGlobal(getShortcuts(), getGlobalOptions());
    dialogYara.setData(g_sFileName, true);

    dialogYara.exec();
}

