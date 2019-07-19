#include "cgetprocessdialog.h"
#include "ui_cgetprocessdialog.h"
#include "common/cappenv.hpp"
#include "cprocesstablemodel.h"

CGetProcessDialog::CGetProcessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGetProcessDialog)
{
    ui->setupUi(this);

    auto *model = new CProcessTableModel(this);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);

    ui->tableView->setColumnWidth(0, 100);
    ui->tableView->setColumnWidth(1, 200);
}

CGetProcessDialog::~CGetProcessDialog()
{
    delete ui;
}

void CGetProcessDialog::on_tableView_doubleClicked(const QModelIndex &index)
{
    m_selectProcessName = index.data(Qt::DisplayRole).toString();
    this->accept();
}

QString CGetProcessDialog::selectProcessName() const
{
    return m_selectProcessName;
}
