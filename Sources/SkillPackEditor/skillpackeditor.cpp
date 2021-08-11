#include "skillpackeditor.h"
#include "ui_skillpackeditor.h"

const char * DEFAULT_SKILL_NAME = "New skill";

SkillPackEditor::SkillPackEditor() :
    QMainWindow(nullptr),
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
    if (skillPackFile != nullptr) {
        delete skillPackFile;
    }
    delete model;
    delete skillsSelection;
    delete levelsSelection;
    delete ui;
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
    if (index.isValid()) {
        ui->descrEdit->setText(model->data(model->index(0, 0, index)).toString());
    } else {
        ui->descrEdit->setText("");
    }
}


void SkillPackEditor::on_descrEdit_textChanged()
{
    if (ui->levelsList->currentIndex().isValid()) {
        model->setData(model->index(0, 0, ui->levelsList->currentIndex()), ui->descrEdit->toPlainText());
    }
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

void SkillPackEditor::fromGui() {
    try {
        if (skillPackFile == nullptr) {
            return;
        }
        SkillPack skp;
        QFileInfo in = QFileInfo(*skillPackFile);
        transferToSkillPackStructure(&skp);
        skp.save(in.dir());
        ui->statusbar->showMessage("SkillPack " + skp.objectName() + " is saved!");
    } catch (QString err) {
        ui->statusbar->showMessage("Error while saving file: " + err);
    }
}

void SkillPackEditor::toGui() {
    try {
        if (skillPackFile == nullptr) {
            return;
        }
        SkillPack skp;
        skp.load(skillPackFile);
        transferFromSkillPackStructure(&skp);
    } catch (QString err) {
        ui->statusbar->showMessage("Error while opening file: " + err);
    }
}

void SkillPackEditor::transferToSkillPackStructure(SkillPack * skp)
{
    for (int i = 0; i < model->rowCount(); i++) {
        Skill * sk = new Skill();
        sk->setObjectName(model->data(model->index(i, 0)).toString());

        QStandardItem * it = model->item(i, 0);
        for (int j = 0; j < it->rowCount(); j++) {
            sk->addLevel(model->data(model->index(0, 0, model->index(j, 0, it->index()))).toString());
        }
        skp->addSkill(sk);
    }

    skp->setObjectName(ui->currentFileName->text());
}

void SkillPackEditor::transferFromSkillPackStructure(SkillPack * skp)
{
    for (int i = 0; i < skp->getSkillsCount(); i++) {

        Skill * sk = skp->getSkill(i);
        assert(sk != nullptr);
        model->insertRows(i, 1);
        model->setData(model->index(i, 0), sk->objectName(), Qt::EditRole);

        QStandardItem * it = model->item(i, 0);
        assert(it != nullptr);

        model->insertColumns(0, 1, it->index());
        for (int j = 0; j < sk->getLevelsCount(); j++) {
            model->insertRows(j, 1, it->index());
            QStandardItem * le = model->itemFromIndex(model->index(j, 0, it->index()));
            assert(le != nullptr);
            model->setData(le->index(), QString::number(j + 1));
            model->insertRows(0, 1, le->index());
            model->insertColumns(0, 1, le->index());
            model->setData(model->index(0, 0, le->index()), sk->getLevelDescription(j));
        }
    }

    ui->currentFileName->setText(skp->objectName());
}

void SkillPackEditor::on_actionCreate_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Create skill pack file");

    if (skillPackFile != nullptr) {
        delete skillPackFile;
    }

    QFileInfo info(path);
    skillPackFile = new QFile(info.path() + "/" + info.completeBaseName() + SKILL_PACK_FILE_EXTENSION);

    model->clear();
    model->insertColumns(0, 1);
    ui->currentFileName->setText(info.baseName());

    fromGui();
}

void SkillPackEditor::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Select skill pack file");

    if (skillPackFile != nullptr) {
        delete skillPackFile;
    }
    skillPackFile = new QFile(path);

    if (!skillPackFile->exists()) {
        ui->statusbar->showMessage("Opened file not exists");
        delete skillPackFile;
        skillPackFile = nullptr;
        return;
    }

    model->clear();
    model->insertColumns(0, 1);
    ui->currentFileName->setText(QFileInfo(*skillPackFile).baseName());

    toGui();
}


void SkillPackEditor::on_actionSave_triggered()
{
    fromGui();
}

