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
#include "yarawidgetadvanced.h"
#include "ui_yarawidgetadvanced.h"

YARAWidgetAdvanced::YARAWidgetAdvanced(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::YARAWidgetAdvanced)
{
    ui->setupUi(this);
#ifdef USE_YARA
    g_scanResult = {};
#endif
    ui->tableWidgetMatches->setColumnCount(4);
    ui->tableWidgetMatches->setRowCount(0);
    // TODO set Title
    QStringList listHeaders;
    listHeaders.append(tr("Offset"));
    listHeaders.append(tr("Size"));
    listHeaders.append(tr("Name"));
    listHeaders.append(QString(""));

    ui->tableWidgetMatches->setHorizontalHeaderLabels(listHeaders);

    ui->tableWidgetMatches->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableWidgetMatches->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->tableWidgetMatches->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidgetMatches->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);

    ui->tableWidgetMatches->setColumnWidth(0, 120);  // TODO Consts
    ui->tableWidgetMatches->setColumnWidth(1, 80);
    ui->tableWidgetMatches->setColumnWidth(3, 20);
}

YARAWidgetAdvanced::~YARAWidgetAdvanced()
{
    delete ui;
}

void YARAWidgetAdvanced::setData(const QString &sFileName, bool bScan)
{
    g_sFileName = sFileName;

    if (bScan) {
        process();
    }
}

void YARAWidgetAdvanced::adjustView()
{
    // TODO
}

void YARAWidgetAdvanced::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    // TODO
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void YARAWidgetAdvanced::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
    // TODO
}

void YARAWidgetAdvanced::process()
{
#ifdef USE_YARA
    XYara xyara;

    QString sRulesPath = getGlobalOptions()->getValue(XOptions::ID_SCAN_YARARULESPATH).toString();

    xyara.loadRulesFromFolder(sRulesPath);

    XYaraDialogProcess dialogStaticScanProcess(this, &xyara);
    dialogStaticScanProcess.setData(g_sFileName);
    dialogStaticScanProcess.showDialogDelay();

    g_scanResult = xyara.getScanResult();

    YARA_Widget::setResultToTreeView(ui->treeViewResult, &g_scanResult);

    connect(ui->treeViewResult->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(onSelectionChanged(QItemSelection, QItemSelection)));
#endif
}

void YARAWidgetAdvanced::on_pushButtonSave_clicked()
{
    QString sSaveFileName = XBinary::getResultFileName(g_sFileName, QString("%1.txt").arg(QString("YARA")));

    QString _sFileName = QFileDialog::getSaveFileName(this, tr("Save"), sSaveFileName, QString("%1 (*.txt);;%2 (*)").arg(tr("Text files"), tr("All files")));

    if (!_sFileName.isEmpty()) {
        if (!XOptions::saveTreeView(ui->treeViewResult, sSaveFileName)) {
            QMessageBox::critical(XOptions::getMainWidget(this), tr("Error"), QString("%1: %2").arg(tr("Cannot save file"), _sFileName));
        }
    }
}

void YARAWidgetAdvanced::onSelectionChanged(const QItemSelection &itemSelected, const QItemSelection &itemDeselected)
{
    Q_UNUSED(itemDeselected)

    ui->tableWidgetMatches->setRowCount(0);
    ui->plainTextEdit->clear();
    ui->lineEditRuleName->clear();

    QModelIndexList listSelected = itemSelected.indexes();
#ifdef USE_YARA
    if (listSelected.count() >= 1) {
        QString sUUID = listSelected.at(0).data(Qt::UserRole + 1).toString();

        qint32 nNumberOfRecords = g_scanResult.listRecords.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            if (g_scanResult.listRecords.at(i).sUUID == sUUID) {
                XYara::SCAN_STRUCT scanStruct = g_scanResult.listRecords.at(i);

                qint32 nNumberOfMatches = scanStruct.listScanMatches.count();
                ui->tableWidgetMatches->setRowCount(nNumberOfMatches);

                for (qint32 j = 0; j < nNumberOfMatches; j++) {
                    {
                        QTableWidgetItem *pItem = new QTableWidgetItem;
                        pItem->setTextAlignment(Qt::AlignRight);
                        pItem->setText(XBinary::valueToHexEx(scanStruct.listScanMatches.at(j).nOffset));
                        ui->tableWidgetMatches->setItem(j, 0, pItem);
                    }
                    {
                        QTableWidgetItem *pItem = new QTableWidgetItem;
                        pItem->setTextAlignment(Qt::AlignRight);
                        pItem->setText(XBinary::valueToHexEx(scanStruct.listScanMatches.at(j).nSize));
                        ui->tableWidgetMatches->setItem(j, 1, pItem);
                    }
                    {
                        QTableWidgetItem *pItem = new QTableWidgetItem;
                        pItem->setTextAlignment(Qt::AlignLeft);
                        pItem->setText(scanStruct.listScanMatches.at(j).sName);
                        ui->tableWidgetMatches->setItem(j, 2, pItem);
                    }
                    {
                        // TODO flag
                        QPushButton *pPushButton = new QPushButton;
                        pPushButton->setText(QString(">"));
                        pPushButton->setProperty("OFFSET", scanStruct.listScanMatches.at(j).nOffset);
                        pPushButton->setProperty("SIZE", scanStruct.listScanMatches.at(j).nSize);
                        ui->tableWidgetMatches->setCellWidget(j, 3, pPushButton);

                        connect(pPushButton, SIGNAL(clicked(bool)), this, SLOT(pushButtonSlot()));
                    }
                }

                QString sCurrentRulesFile = scanStruct.sRulesFullFileName;

                QByteArray baData = XBinary::readFile(sCurrentRulesFile);
                ui->plainTextEdit->setPlainText(baData);
                QTextCursor newCursor = ui->plainTextEdit->textCursor();
                newCursor.movePosition(QTextCursor::End);
                ui->plainTextEdit->setTextCursor(newCursor);

                ui->plainTextEdit->find(QString("rule %1").arg(scanStruct.sRule), QTextDocument::FindWholeWords | QTextDocument::FindBackward);
                ui->lineEditRuleName->setText(scanStruct.sRulesFile);

                break;
            }
        }
    }
#endif
}

void YARAWidgetAdvanced::pushButtonSlot()
{
    QPushButton *pPushButton = qobject_cast<QPushButton *>(sender());

    if (pPushButton) {
        qint64 nOffset = pPushButton->property("OFFSET").toLongLong();
        qint64 nSize = pPushButton->property("SIZE").toLongLong();

        emit showHex(nOffset, nSize);
    }
}

void YARAWidgetAdvanced::on_pushButtonScan_clicked()
{
    process();
}

void YARAWidgetAdvanced::on_pushButtonRules_clicked()
{
    QString sDirectory = getGlobalOptions()->getValue(XOptions::ID_SCAN_YARARULESPATH).toString();
    sDirectory = XBinary::convertPathName(sDirectory);
    XOptions::showFolder(sDirectory);
}
