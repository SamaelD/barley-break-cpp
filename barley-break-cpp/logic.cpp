#include <QVector>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include <QDebug>

#include "logic.h"

Logic *Logic::pInst = nullptr;

Logic::Logic(QObject *parent) : QObject(parent) {
  loadScores();

  for (unsigned short i = 1; i < 16; ++i) {
    m_list << QString::number(i);
  }
  m_list << QString::number(0);
  connect(this, &Logic::listChanged, this, &Logic::movement);
}

Logic::~Logic() {}

int Logic::moveCounter() const {
  return m_moveCounter;
}

void Logic::refresh() {
  auto size = m_list.size();
  QVector<int> arr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  auto getIdentifier = [&arr]() {
    int value = arr[qrand() % arr.size()];
    arr.removeOne(value);
    return QString::number(value);
  };

  for (unsigned short i = 0; i < size; ++i) {
    m_list[i] = getIdentifier();
  }
  if (countInvertion() % 2 != 0) {
    m_list.move(size - 2, size - 1);
  }

  emit listChanged(m_list);
  setmoveCounter(0);
  emit victoryChanged(m_victory = false);
}

void Logic::move(int currentIndex) {
  if (checkOutOfRange(currentIndex + 1)
      && checkBorder(currentIndex, true)
      && m_list[currentIndex + 1] == "0") {
    m_list.move(currentIndex, currentIndex + 1); listChanged(m_list);
  }
  else if (checkOutOfRange(currentIndex - 1)
           && checkBorder(currentIndex, false)
           && m_list[currentIndex - 1] == "0") {
    m_list.move(currentIndex, currentIndex - 1); listChanged(m_list);
  }
  else if (checkOutOfRange(currentIndex + 4)
           && m_list[currentIndex + 4] == "0") {
    m_list.move(currentIndex, currentIndex + 4);
    m_list.move(currentIndex + 3, currentIndex); listChanged(m_list);
  }
  else if (checkOutOfRange(currentIndex - 4)
           && m_list[currentIndex - 4] == "0") {
    m_list.move(currentIndex, currentIndex - 4);
    m_list.move(currentIndex - 3, currentIndex); listChanged(m_list);
  }

  if (checkWin()) {
    this->saveScores();
    emit victoryChanged(m_victory = true);
  }
}

bool Logic::checkWin() const {
  auto size = m_list.size();
  int i = 1;
  for (auto it : m_list) {
    if (i < size && it != QString::number(i++)) {
      return false;
    }
  }
  return true;
}

QString Logic::identifier(int index) const {
  return m_list[index];
}

int Logic::countInvertion() const {
  auto size = m_list.size();
  int counter = 0;
  int row = 0;

  for (int i = 0, j = 1; i < size - 1; ++i, ++j) {
    if (m_list[i] > m_list[j]) {
      ++counter;
    }
    if (m_list[j] == "0") {
      row = j / 4;
    }
  }
  return counter + row;
}

bool Logic::checkOutOfRange(int value) {

  if (value < 0 || value >= m_list.size()) {
    return false;
  }
  return true;
}

bool Logic::checkBorder(int value, bool right) {
  auto size = m_list.size();
  for (int i = (right ? 3 : 0); i < size; i += 4) {
    if (value == i) {
      return false;
    }
  }
  return true;
}

void Logic::loadScores() {
  QFile loadFile(QStringLiteral("scores.json"));

  if (!loadFile.open(QIODevice::ReadOnly)) {
    qWarning("can't load file");
    setBestScore(0);
    return;
  }
  QByteArray data = loadFile.readAll();
  QJsonDocument document(QJsonDocument::fromJson(data));
  auto json = document.object();
  setBestScore(json["scores"].toInt());
}

void Logic::saveScores() {
  QFile saveFile(QStringLiteral("scores.json"));

  if (!saveFile.open(QIODevice::WriteOnly)) {
    qWarning("Could't open file to saving data!");
    return;
  }

  setBestScore(m_moveCounter);
  QJsonObject dataToSave { QPair<QString, QJsonValue>("scores", m_BestScore) };
  QJsonDocument document(dataToSave);
  saveFile.write(document.toJson());
}


QObject *Logic::singletone_provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)

  if (!pInst) {
    pInst = new Logic();
  }
  return pInst;
}

bool Logic::victory() const {
  return m_victory;
}

int Logic::BestScore() const {
  return m_BestScore;
}

QStringList Logic::list() const {
  return m_list;
}

void Logic::setmoveCounter(int moveCounter) {
  if (m_moveCounter == moveCounter) {
    return;
  }

  m_moveCounter = moveCounter;
  emit moveCounterChanged(moveCounter);
}

void Logic::setList(QStringList list) {
  if (m_list == list)
    return;

  m_list = list;
  emit listChanged(list);
}

void Logic::setBestScore(int BestScore) {
  if (m_BestScore == BestScore || (m_BestScore < BestScore && m_BestScore != 0)) {
    return;
  }

  m_BestScore = BestScore;
  emit BestScoreChanged(BestScore);
}

void Logic::movement() {
  setmoveCounter(m_moveCounter + 1);
}
