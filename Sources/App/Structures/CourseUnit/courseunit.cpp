#include "CourseUnit.h"

CourseUnit::CourseUnit(QObject* pobj): QObject(pobj), income(), outcome(), embedded_units(), linked_units(){

}

CourseUnit::CourseUnit(size_t width, size_t height, size_t x, size_t y, QObject *pobj): QObject(pobj),
                                                                                        height(height),
                                                                                        x(x), y(y),
                                                                                        width(width),
                                                                                        income(),
                                                                                        outcome(),
                                                                                        embedded_units(),
                                                                                        linked_units()
{

}

void CourseUnit::saveCourseUnit(QFile *dest) {
     qDebug() << "Saving CourseUnit" << objectName();

     QFileInfo in = QFileInfo(*dest);
     QDir dr = in.dir();

     if (!dr.exists()) {
         dr.mkpath(dr.absolutePath());
     }

     lastFilePath = in.absoluteFilePath();

     if (dest->open(QIODevice::WriteOnly)) {
         QTextStream stream(dest);

         stream << objectName() << FIELDSSEPARATOR;
         stream << width << FIELDSSEPARATOR;
         stream << height << FIELDSSEPARATOR;
         stream << x << FIELDSSEPARATOR;
         stream << y << FIELDSSEPARATOR;
         stream << colour << FIELDSSEPARATOR;

         for(int i = 0; i < income.size(); ++i){
             stream << income[i].first << ":" << income[i].second << VECTORSEPARATOR;
         }
         stream << FIELDSSEPARATOR;

         for(int i = 0; i < outcome.size(); ++i){
             stream << outcome[i].first << ":" << outcome[i].second << VECTORSEPARATOR;
         }
         stream << FIELDSSEPARATOR;

         stream << description << FIELDSSEPARATOR;

         for(int i = 0; i < linked_units.size(); ++i){
             stream << linked_units[i] << VECTORSEPARATOR;
         }
         stream << FIELDSSEPARATOR;
         stream << field_height << FIELDSSEPARATOR;
         stream << field_width;
         stream << UNITSEPARATOR;

         for (int i = 0; i < embedded_units.size(); ++i) {
             QString CourseUnitFileName = embedded_units[i]->objectName() + COURSE_UNIT_FILE_EXTENSION;

             QFile CourseUnitFile = QFile(dr.filePath(CourseUnitFileName));

             try {
                 embedded_units[i]->saveCourseUnit(&CourseUnitFile);
             } catch (QString err) {
                 throw QString("Saving [" + embedded_units[i] -> objectName() + "] with error: " + err);
             }

             stream << CourseUnitFileName << UNITSEPARATOR;
         }

         dest->close();
     } else {
         throw QString("Can't open course file [" + dest->fileName() + "]");
     }

}


void CourseUnit::loadCourseUnit(QFile *res){
    if (!res->exists()) {
        throw QString("CourseUnit file [" + res->fileName() + "] not exists");
    }

    QFileInfo info = QFileInfo(*res);

    qDebug() <<  "Loading CourseUnit from" << info.absoluteFilePath();

    QString data;

    lastFilePath = info.absoluteFilePath();

    if (res->open(QIODevice::ReadOnly)) {
        QTextStream stream(res);
        data = stream.readAll();
        res->close();
    } else {
        throw QString("Can't open CourseUnit file [" + res->fileName() + "]");
    }

    QStringList unit_data = data.split(UNITSEPARATOR, Qt::SkipEmptyParts);


    if (unit_data.empty()) {
        throw QString("Empty CourseUnit [" + res->fileName() + "]");
    }

    QStringList fields_data = unit_data[0].split(FIELDSSEPARATOR);
    this->setObjectName(fields_data[0]);

    width = fields_data[1].toULong();
    height = fields_data[2].toULong();
    x = fields_data[3].toULong();
    y = fields_data[4].toULong();
    colour = fields_data[5].toInt();

    QStringList vector_data = fields_data[6].split(VECTORSEPARATOR, Qt::SkipEmptyParts);
    for(int i = 0; i < vector_data.size(); ++i){
        QStringList pairs = vector_data[i].split(":");
        income.push_back(std::make_pair(pairs[0], pairs[1].toULongLong()));
    }

    vector_data = fields_data[7].split(VECTORSEPARATOR, Qt::SkipEmptyParts);
    for(int i = 0; i < vector_data.size(); ++i){
        QStringList pairs = vector_data[i].split(":");
        outcome.push_back(std::make_pair(pairs[0], pairs[1].toULongLong()));
    }

    description = fields_data[8];

    vector_data = fields_data[9].split(VECTORSEPARATOR, Qt::SkipEmptyParts);
    for(int i = 0; i < vector_data.size(); ++i){
        linked_units.push_back(vector_data[i]);
    }

    field_height = fields_data[10].toULong();
    field_width = fields_data[11].toULong();

    embedded_units.reserve(unit_data.size() - 1);

    QDir dr = info.dir();

    for (int i = 1; i < unit_data.size(); i++) {
        QString CourseUnitFileName = unit_data[i];
        QFile CourseUnitFile = dr.filePath(CourseUnitFileName);
        CourseUnit *unit = new CourseUnit(this);
        try {
            unit->loadCourseUnit(&CourseUnitFile);
        } catch (QString err) {
            throw QString("Error while loading CourseUnit [" + CourseUnitFileName + "] with error: " + err);
        }

        embedded_units.push_back(unit);
    }
}

void CourseUnit::setColour(int colour_){
    colour = colour_;
}

int CourseUnit::getColour(){
    return colour;
}


void CourseUnit::setFieldSize(size_t nwidth, size_t nheight){
    field_height = nheight;
    field_width = nwidth;
}

std::pair<size_t, size_t> CourseUnit::getFieldSize() const{
    return std::make_pair(field_width, field_height);
}


void CourseUnit::setSize(size_t new_width, size_t new_height){
    width = new_width;
    height = new_height;
}

std::pair<size_t, size_t> CourseUnit::getSize() const{
    return std::make_pair(width, height);
}

void CourseUnit::setCoords(size_t new_x, size_t new_y){
    x = new_x;
    y = new_y;
}

std::pair<size_t, size_t> CourseUnit::getCoords() const{
    return std::make_pair(x, y);
}


void CourseUnit::addIncome(const std::pair<QString, size_t>& skill){
    income.push_back(skill);
}

void CourseUnit::addOutcome(const std::pair<QString, size_t>& skill){
    outcome.push_back(skill);
}

const QVector<std::pair<QString, size_t>>& CourseUnit::getIncome() const{
    return income;
}

const QVector<std::pair<QString, size_t>>& CourseUnit::getOutcome() const{
    return outcome;
}

void CourseUnit::setDescription(const QString& desc){
    description = desc;
}

const QString& CourseUnit::getDescription() const{
    return description;
}

const QVector<QString>& CourseUnit::getConnections() const{
    return linked_units;
}

const QVector<CourseUnit*>& CourseUnit::getEmbedded() const{
    return embedded_units;
}


void CourseUnit::addConnection(CourseUnit* unit){
    linked_units.push_back(unit -> objectName());
}

void CourseUnit::addConnection(const QString& objname){
    linked_units.push_back(objname);
}


void CourseUnit::addEmbedded(CourseUnit* unit){
    embedded_units.push_back(unit);
}


QString CourseUnit::print(){
    QString buff;
    buff += objectName();
    buff += "width = " + QString::number(width);
    buff += "height = " + QString::number(height);
    buff += "x = " + QString::number(x) ;
    buff +=  "y = " + QString::number(y);
    buff +=  "description = " + description;
    buff +=  "income skills: \n";
    for (int i = 0; i < income.size(); ++i){
        buff +=  income[i].first + " : " + QString::number(income[i].second);
    }
    buff +=  "outcome skills: \n";
    for (int i = 0; i < outcome.size(); ++i){
        buff +=  outcome[i].first + " : " + QString::number(outcome[i].second);
    }
    buff +=  "linked units:";
    for (int i = 0; i < linked_units.size(); ++i){
        buff +=  linked_units[i] + " ";
    }
    buff +=  "embedded units:";
    for(int i = 0; i < embedded_units.size(); ++i){
        buff +=  embedded_units[i] -> objectName() + " ";
    }
    buff +=  "\n\n";
    for(int i = 0; i < embedded_units.size(); ++i){
        buff += embedded_units[i] -> print();
    }

    return buff;
}

QString CourseUnit::getLastFilePath() const {
	return lastFilePath;
}

void CourseUnit::setLastFilePath(QString lastFilePath) {
	this->lastFilePath = lastFilePath;
}

bool CourseUnit::containsInSkill(QString name) {
	for (int i = 0; i < income.size(); i++) {
		if (name == income[i].first) {
			return 1;
		}
	}
	return 0;
}

bool CourseUnit::containsOutSkill(QString name) {
	for (int i = 0; i < outcome.size(); i++) {
		if (name == income[i].first) {
			return 1;
		}
	}
	return 0;
}
