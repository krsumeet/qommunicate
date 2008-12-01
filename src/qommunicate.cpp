#include <QMessageBox>
#include <QCloseEvent>
#include <QMenu>
#include <QTreeWidget>

#include "about.h"
#include "settings.h"
#include "messagedialog.h"
#include "qommunicate.h"

Qommunicate::Qommunicate(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
        
    createTrayIcon();
    
    populateTree();
}

void Qommunicate::on_searchEdit_textChanged(const QString &text)
{
    filterModel->setFilterFixedString(text);
}

void Qommunicate::on_action_About_triggered()
{
    AboutDialog(this).exec();
}

void Qommunicate::on_action_Settings_triggered()
{
    SettingsDialog(this).exec();
}

void Qommunicate::on_actionBroadcast_triggered()
{
    MessageDialog dlg(tr("Broadcast"));
    dlg.setModal(false);
    dlg.exec();
}

void Qommunicate::on_actionQuit_triggered()
{
    qApp->quit();
}

void Qommunicate::closeEvent(QCloseEvent * event)
{
    setVisible(false);
    event->ignore();
}

void Qommunicate::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if( reason == QSystemTrayIcon::Trigger )
        setVisible(!isVisible());
}

void Qommunicate::createTrayIcon()
{    
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(windowIcon());
    
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    
    QMenu *menu = new QMenu;
    menu->addAction(ui.actionBroadcast);
    menu->addAction(ui.actionQuit);
    trayIcon->setContextMenu(menu);
    
    trayIcon->show();
}

// void Qommunicate::on_memberTree_itemDoubleClicked(QTreeWidgetItem * item, int col)
// {
//     // TODO: subclass item and set custom types for groups and users
//     MessageDialog dlg(tr("Chat with %1").arg(item->data(0, Qt::DisplayRole).toString()));
//     dlg.setModal(false);
//     dlg.exec();
// }

void Qommunicate::populateTree()
{
    model = new MemberModel(this);
    filterModel = new MemberFilter(this);
    
    QStandardItem *parent = model->invisibleRootItem();
    for(int i = 0; i < 5 ; i++) {
        parent->appendRow( new QStandardItem(QString("group %1").arg(i)) );
    }
    
    model->item(2)->appendRow( new QStandardItem(QString("member 2")));
    model->item(0)->appendRow( new QStandardItem(QString("member 5")));
    
    filterModel->setSourceModel(model);
    filterModel->setDynamicSortFilter(true);
    
    ui.memberTree->setSelectionMode(ui.memberTree->ExtendedSelection);
    ui.memberTree->setModel(filterModel);
    ui.memberTree->setHeaderHidden(true);
    ui.memberTree->expandAll();
}