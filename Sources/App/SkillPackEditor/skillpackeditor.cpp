#include "skillpackeditor.h"
#include "../Help/smarthelper.h"
#include "ui_skillpackeditor.h"

SkillPackEditor::SkillPackEditor() :
    QMainWindow(nullptr),
    ui(new Ui::SkillPackEditor),
    skillPackFile(nullptr),
    model(new QStandardItemModel(0, 1, this)),
    skillsSelection(new QItemSelectionModel(model, this)),
    levelsSelection(new QItemSelectionModel(model, this))
{
	qInfo() << "SkillpackEditor init started";

    ui->setupUi(this);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Skills, levels, descriptions"));

    ui->tree->setModel(model);
    ui->tree->setEditTriggers(QAbstractItemView::DoubleClicked);

    timerId = startTimer(1000);

    qInfo() << "SkillpackEditor init finished";
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
    killTimer(timerId);
}

void SkillPackEditor::on_AddSkill_clicked()
{
    if (ensureFileIsValid()) {
        return;
    }

    // Add new skill
    int row = model->rowCount();

    model->insertRows(row, 1);

    model->setData(model->index(row, 0), "Skill " + QString::number(row + 1));

    // Add new subdir for level
    model->item(row, 0)->insertColumns(0, 1);

    ui->statusbar->showMessage("New skill added!");
    ui->totalSkills->setText(QString::number(model->rowCount()));

    ui->tree->setCurrentIndex(model->index(row, 0));

    on_AddLevel_clicked();
}

void SkillPackEditor::on_AddLevel_clicked()
{
    if (ensureFileIsValid()) {
        return;
    }

    QModelIndex ind = ui->tree->currentIndex();

    if (getTreeItemLevel(ind) == 3) {
    	ind = ind.parent();
    }
    if (getTreeItemLevel(ind) == 2) {
    	ind = ind.parent();
    }

    if (ind.isValid() && getTreeItemLevel(ind) == 1) {
        // Add new list
        QStandardItem * skill = model->item(ind.row(), 0);

        int row = skill->rowCount();

        skill->insertRows(row, 1);

        QModelIndex levelInd = model->index(row, 0, skill->index());

        QStandardItem * level = model->itemFromIndex(levelInd);
        level->setData("Level " + QString::number(levelInd.row() + 1), Qt::EditRole);

        // Add subdir for text
        level->insertColumns(0, 1);
        level->insertRows(0, 1);
        QStandardItem * desc = model->itemFromIndex(model->index(0, 0, level->index()));
        desc->setData("Empty description", Qt::EditRole);

        ui->tree->expand(ind);

        ui->statusbar->showMessage("New level added!");
    }
}

void SkillPackEditor::fromGui() {
    try {
        if (skillPackFile == nullptr) {
            return;
        }
        SkillPack skp;
        QFileInfo in = QFileInfo(*skillPackFile);
        transferToSkillPackStructure(&skp);
        skp.save(skillPackFile);
        ui->totalSkills->setText(QString::number(model->rowCount()));
        ui->statusbar->showMessage("SkillPack " + skp.objectName() + " is saved into file " + in.baseName() + "!");
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

    skp->setObjectName(ui->skillPackName->text());
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

    ui->totalSkills->setText(QString::number(model->rowCount()));
    ui->skillPackName->setText(skp->objectName());
}

int SkillPackEditor::getTreeItemLevel(QModelIndex ind)
{
    if (!ind.isValid()) {
        return -1;
    }
    if (ind.parent() == QModelIndex()) {
        return 1;//skill
    }
    if (ind.parent().parent() == QModelIndex()) {
        return 2;//level
    }
    if (ind.parent().parent().parent() == QModelIndex()) {
        return 3;//description
    }

    return -1;
}

void SkillPackEditor::setSkillPackFile(QString path, bool mayExist)
{
    ensureFileIsNull();

    QFileInfo info(path);
    skillPackFile = new QFile(info.path() + "/" + info.completeBaseName() + SKILL_PACK_FILE_EXTENSION);

    if (!skillPackFile->exists() && mayExist) {
        ui->statusbar->showMessage("Opened file not exists");
        delete skillPackFile;
        skillPackFile = nullptr;
        return;
    }

    model->clear();
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Skills, levels, descriptions"));
    model->insertColumns(0, 1);
    ui->skillPackName->setReadOnly(false);
    ui->currentFileName->setText(QFileInfo(*skillPackFile).baseName());
}

bool SkillPackEditor::ensureFileIsValid()
{
    if (skillPackFile == nullptr || !skillPackFile->exists()) {
         ui->statusbar->showMessage("No skill pack file! Create it!");
         return 1;
    }

    return 0;
}

void SkillPackEditor::ensureFileIsNull()
{
    if (skillPackFile != nullptr) {
        if (QMessageBox::question(this, "Save file or not",
                              "Would you like to save your file before closing it?",
                              QMessageBox::Save | QMessageBox::Discard) == QMessageBox::Save) {
            on_actionSave_triggered();
        }
        delete skillPackFile;
        skillPackFile = nullptr;
        ui->currentFileName->clear();
        ui->skillPackName->setReadOnly(true);
    }
}

void SkillPackEditor::on_actionCreate_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Create skill pack file", QString(), QString("(*") + SKILL_PACK_FILE_EXTENSION + QString(")"));

    if (path.size() == 0) {
        return;
    }

    setSkillPackFile(path, false);

    fromGui();
}

void SkillPackEditor::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Select skill pack file", QString(), QString("(*") + SKILL_PACK_FILE_EXTENSION + QString(")"));

    if (path.size() == 0) {
        return;
    }

    setSkillPackFile(path, true);

    toGui();
}


void SkillPackEditor::on_actionSave_triggered()
{
    fromGui();
}


void SkillPackEditor::on_remove_clicked()
{
    if (ensureFileIsValid()) {
        return;
    }

    QModelIndex ind = ui->tree->currentIndex();
    int level = getTreeItemLevel(ind);

    qDebug() << getTreeItemLevel(ind) << ind.row();

    QString dat = model->data(ind).toString();

    if (level == 2 && model->itemFromIndex(ind)->parent()->rowCount() < 2) {
    	ui->statusbar->showMessage("Nothing to remove!");
    	return;
    }

    if (level == 1 || level == 2) {
        model->removeRow(ind.row(), ind.parent());
        ui->statusbar->showMessage("[" + dat + "] was removed!");
    } else {
         ui->statusbar->showMessage("Nothing to remove!");
    }
}


void SkillPackEditor::on_actionClose_triggered()
{
    ensureFileIsNull();
    model->clear();
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Skills, levels, descriptions"));
    ui->totalSkills->clear();
    ui->currentFileName->clear();
    ui->skillPackName->clear();
}


void SkillPackEditor::on_actionHelp_me_triggered()
{
	SmartHelper help(":/help/Help/SkillPackEditorHelp.txt", this);
	help.exec();
}


void SkillPackEditor::on_actionReturn_to_launcher_triggered()
{
    on_actionClose_triggered();
    emit onClose();
}


void SkillPackEditor::on_actionSet_style_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Select CSS file", QString(), QString("(*") + ".css" + QString(")"));
    QFile st = QFile(path);
    if (!st.exists()) {
        ui->statusbar->showMessage("CSS file not exists!");
    }
    if (st.open(QIODevice::ReadOnly)) {
        QString css = st.readAll();
        this->setStyleSheet(css);
        st.close();
    } else {
        ui->statusbar->showMessage("CSS file can not be opened!");
    }
}

void SkillPackEditor::on_autoSave_stateChanged(int v) {
}

void SkillPackEditor::timerEvent(QTimerEvent *event) {
	if (ui->autoSave->isChecked()) {
		fromGui();
	}
}
