#include "skillpackeditor.h"
#include "ui_skillpackeditor.h"

const char * DEFAULT_SKILL_NAME = "New skill";

SkillPackEditor::SkillPackEditor() :
    QMainWindow(nullptr),
    skp(nullptr),
    ui(new Ui::SkillPackEditor),
    skillPackFile(nullptr),
    model(new QStandardItemModel(this)),
    skillsSelection(new QItemSelectionModel(model, this)),
    levelsSelection(new QItemSelectionModel(model, this))
{
    ui->setupUi(this);

    model->insertColumn(0);

    ui->skillsList->setModel(model);
    ui->levelsList->setModel(model);

    ui->skillsList->setSelectionModel(skillsSelection);
    ui->levelsList->setSelectionModel(levelsSelection);

    ui->skillsList->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->levelsList->setEditTriggers(QAbstractItemView::DoubleClicked);
}

SkillPackEditor::~SkillPackEditor()
{
    if (skp != nullptr) {
        delete skp;
    }
    if (skillPackFile != nullptr) {
        delete skillPackFile;
    }
    delete model;
    delete skillsSelection;
    delete levelsSelection;
    delete ui;
}

void SkillPackEditor::on_actionCreate_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Create skill pack file");
    skp = new SkillPack(this);

    QFileInfo info(path);
    skillPackFile = new QFile(info.path() + "/" + info.completeBaseName() + SKILL_PACK_FILE_EXTENSION);
    skp->setObjectName(info.baseName());

    resave();
}

void SkillPackEditor::resave() {
    try {
        if (skp != nullptr && skillPackFile != nullptr) {
            QFileInfo in = QFileInfo(*skillPackFile);
            skp->save(in.dir());
            ui->statusbar->showMessage("SkillPack " + skp->objectName() + " is saved!");
        }
    } catch (QString err) {
        ui->statusbar->showMessage("Error while saving file: " + err);
    }
}

void SkillPackEditor::on_AddSkill_clicked()
{
    // Add new skill
    int row = model->rowCount();

    model->insertRow(row);
    model->setData(model->index(row, 0), "New skill");

    // Add new subdir for level
    model->item(row, 0)->insertColumns(0, 1);

    // Switch level list for this skill
    ui->levelsList->setRootIndex(model->index(row, 0));

    // highlight new skill
    skillsSelection->clear();
    skillsSelection->select(model->index(row, 0), QItemSelectionModel::Select);

    // setting new current skill
    ui->skillsList->setCurrentIndex(model->index(row, 0));
}

void SkillPackEditor::on_AddLevel_clicked()
{
    QModelIndex ind = ui->skillsList->currentIndex();
    if (ind.isValid()) {
        // Add new list
        QStandardItem * skill = model->item(ind.row(), 0);

        int row = skill->rowCount();

        skill->insertRows(row, 1);

        QModelIndex levelInd = model->index(row, 0, skill->index());

        QStandardItem * level = model->itemFromIndex(levelInd);
        level->setData("New level", Qt::EditRole);

        // Add subdir for text
        level->insertColumns(0, 1);
        level->insertRows(0, 1);
        QStandardItem * desc = model->itemFromIndex(model->index(0, 0, level->index()));
        desc->setData("Empty description", Qt::EditRole);

        // highlight new level
        levelsSelection->clear();
        levelsSelection->select(levelInd, QItemSelectionModel::Select);

        // setting new current level
        ui->levelsList->setCurrentIndex(levelInd);

        // update descr pane
        on_descrEdit_textChanged();
    }
}

void SkillPackEditor::on_levelsList_clicked(const QModelIndex &index)
{
    ui->descrEdit->setText(model->data(model->index(0, 0, index)).toString());
}


void SkillPackEditor::on_descrEdit_textChanged()
{
    model->setData(model->index(0, 0, ui->levelsList->currentIndex()), ui->descrEdit->toPlainText());
}


void SkillPackEditor::on_RemoveSkill_clicked()
{
    QModelIndex ind = ui->skillsList->currentIndex();

    if (!ind.isValid()) {
        return;
    }

    int row = ind.row();

    // remove old
    model->removeRow(row);

    QModelIndex newInd = model->index(row - 1, 0);

    // set new root for level
    ui->levelsList->setRootIndex(newInd);

    // set new current selected skill
    skillsSelection->clear();
    skillsSelection->select(newInd, QItemSelectionModel::Select);
    ui->skillsList->setCurrentIndex(newInd);

    // updating descr
    on_levelsList_clicked(ui->levelsList->currentIndex());
}


void SkillPackEditor::on_RemoveLevel_clicked()
{
    QModelIndex levelInd = ui->levelsList->currentIndex();
    QModelIndex skillInd = ui->skillsList->currentIndex();

    if (!levelInd.isValid() || !skillInd.isValid()) {
        return;
    }

    QStandardItem * skill = model->item(skillInd.row(), 0);

    // remove level
    skill->removeRow(levelInd.row());

    QModelIndex newLevelInd = model->index(levelInd.row() - 1, 0, skill->index());

    // set new selected current skill
    levelsSelection->clear();
    levelsSelection->select(newLevelInd, QItemSelectionModel::Select);
    ui->levelsList->setCurrentIndex(newLevelInd);

    // updating descr
    on_levelsList_clicked(ui->levelsList->currentIndex());
}

