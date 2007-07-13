/*
 * ****** Set of useful classes to enforce VAC
 */
 
#include "tb2vacutils.hpp"
#include "tb2vac.hpp"

/**
 * VACValue:
 *   A class that stores useful information on values of variables to enforce VAC
 */

VACValue::VACValue () : removed(false), mark(false), killer(-1) {
  k = 0;
}

VACValue::~VACValue () {}

void VACValue::reset (const Value v) {
  value = v;
  removed = false;
  mark = false;
  killer = -1;
  k = 0;
}

bool VACValue::isMarked () const {
  return mark;
}

bool VACValue::isRemoved () const {
  return removed;
}

int VACValue::getKiller () const {
  return killer;
}

Cost VACValue::getK () const {
  return k;
}

Value VACValue::getValue () const {
  return value;
}

void VACValue::setMark () {
  mark = true;
}

void VACValue::remove () {
  removed = true;
}

void VACValue::setKiller (const int i) {
  killer = i;
}

void VACValue::addToK (const Cost c) {
  k += c;
}


/**
 * VACVariable:
 *   A class that stores useful information on variables to enforce VAC
 */

VACVariable::VACVariable (WCSP *wcsp, string n, Value iinf, Value isup) : EnumeratedVariable(wcsp, n, iinf, isup) {
  init();
}

VACVariable::VACVariable (WCSP *wcsp, string n, Value *d, int dsize) : EnumeratedVariable(wcsp, n, d, dsize) {
  init();
}

VACVariable::~VACVariable () {
  for (unsigned int i = 0; i < getDomainSize(); i++) {
    delete vacValues[i];
  }
  delete[] vacValues;
}

void VACVariable::init () {
  unsigned int i = 0;
  myThreshold = ToulBar2::costThreshold;
  nbValues = getDomainSize();
  nbVACValues = nbValues;
  vacValues = new VACValue*[nbValues];
  for (EnumeratedVariable::iterator it = begin(); it != end(); ++it, ++i) {
    vacValues[i] = new VACValue();
  }
  linkVACQueue.content.var = this;
  linkVACQueue.content.timeStamp = -1;
  linkVAC2Queue.content = this;
  //queueVAC();
  //queueVAC2();
}

void VACVariable::clear()
{
	for(int i=0;i<nbValues;i++) {
		delete vacValues[i];
		vacValues[i] = NULL;
	}
}


void VACVariable::reset () {
  int i = 0;
  nbVACValues = getDomainSize();
  if (ToulBar2::verbose > 5) cout << "variable " << name << endl;
  
  for (EnumeratedVariable::iterator it = begin(); it != end(); ++it, ++i) {
    vacValues[i]->reset(*it);
    if(!isNull(getCost(*it))) removeValue(i);
  }
}

bool VACVariable::isEmpty () const {
  return (nbVACValues == 0);
}

VACValue *VACVariable::getValue (const unsigned int i) const {
  assert(i < getDomainSize());
  return vacValues[i];
}

bool VACVariable::isRemoved (const unsigned int i) const {
  assert(i < getDomainSize());
  return (vacValues[i]->isRemoved());
}

void VACVariable::removeValue (const unsigned int i) {
  assert(i < getDomainSize());
  if (!vacValues[i]->isRemoved()) {
    vacValues[i]->remove();
    nbVACValues--;
  }
}

Cost VACVariable::getIniCost (const unsigned int i) {
  assert(i < getDomainSize());
  return getCost(vacValues[i]->getValue());
}


Cost VACVariable::getVACCost (const unsigned int i) {
  assert(i < getDomainSize());
  Cost c = getCost(vacValues[i]->getValue());
  if(isNull(c)) return 0;
  else return c;
}

void VACVariable::setCost (const unsigned int i, const Cost c) {
  assert(i < getDomainSize());
  assert(c >= 0);
  costs[toIndex(vacValues[i]->getValue())] = c + deltaCost;
  assert(getVACCost(i) == c);
}

void VACVariable::decreaseCost (const unsigned int i, const Cost c) {
  assert(i < getDomainSize());
  Cost cini = getCost(vacValues[i]->getValue());  
  assert((cini >= c));
  //  assert((wcsp->getLb() + cini >= wcsp->getUb()));
  if (wcsp->getLb() + cini < wcsp->getUb()) {
    costs[toIndex(vacValues[i]->getValue())] -= c;
  }
}

void VACVariable::increaseCost (const unsigned int i, const Cost c) {
  assert(i < getDomainSize());
  costs[toIndex(vacValues[i]->getValue())] += c;
}

void VACVariable::VACproject (const unsigned int i, const Cost c) {
  Cost newCost;
  Cost oldCost = getVACCost(i);
  increaseCost(i, c);
  newCost = getVACCost(i);
  if ((getValue(i)->getValue() == maxCostValue) || (newCost > maxCost) || (wcsp->getLb() + newCost >= wcsp->getUb())) {
    queueNC();
  }
  if ((oldCost == 0) && (c > 0)) {
    queueDAC();
    queueEAC1();
  }
  if ((isNull(oldCost)) && (!isNull(newCost))) {
    queueVAC2();
  }
}

void VACVariable::VACextend (const unsigned int i, const Cost c) {
  decreaseCost(i, c);
  if (getValue(i)->getValue() == maxCostValue) {
    queueNC();
  }
}


void VACVariable::setThreshold (Cost c) { myThreshold = c; } 
Cost VACVariable::getThreshold () { return myThreshold; } 


bool VACVariable::isNull (Cost c) 
{
	return (c < myThreshold);
}


void VACVariable::queueVAC() {
  if (ToulBar2::verbose > 5) {
    cout << "VAC queueing x" << wcspIndex << endl;
  }
  wcsp->vac->queueVAC(&linkVACQueue);
}

void VACVariable::queueVAC2() {
  if (ToulBar2::verbose > 5) {
    cout << "VAC2 queueing x" << wcspIndex << endl;
  }
  wcsp->vac->queueVAC2(&linkVAC2Queue);
}

void VACVariable::dequeueVAC2() {
  if (ToulBar2::verbose > 5) {
    cout << "VAC2 dequeueing x" << wcspIndex << endl;
  }
  wcsp->vac->dequeueVAC2(&linkVAC2Queue);
}

void VACVariable::extendAll(Cost cost) {
  VACVariable *xj;
  if (ToulBar2::vac) {
    for (ConstraintList::iterator iter = getConstrs()->begin(); iter != getConstrs()->end(); ++iter) {
      xj = (VACVariable *) (*iter).constr->getVar(1 - (*iter).scopeIndex);
      xj->queueVAC2();
    }
  }
  EnumeratedVariable::extendAll(cost);
}

void VACVariable::assign(Value newValue) {
  VACVariable *xj;
  if (ToulBar2::vac) {
    for (ConstraintList::iterator iter = getConstrs()->begin(); iter != getConstrs()->end(); ++iter) {
      xj = (VACVariable *) (*iter).constr->getVar(1 - (*iter).scopeIndex);
      xj->queueVAC2();
    }
  }
  EnumeratedVariable::assign(newValue);
}

void VACVariable::remove (Value value) {
  if (canbe(value)) {
    queueVAC2();
  }
  EnumeratedVariable::remove(value);
}

void VACVariable::removeFast (Value value) {
  if (canbe(value)) {
    queueVAC2();
  }
  EnumeratedVariable::removeFast(value);
}

void VACVariable::project (Value value, Cost cost) {
  assert(cost >= 0);
  Cost oldcost = getCost(value);
  Cost newcost = oldcost + cost;
  if ((isNull(oldcost)) && (!isNull(newcost))) {
    queueVAC2();
  }
  EnumeratedVariable::project(value, cost);
}

void VACVariable::extend (Value value, Cost cost) {
  VACVariable *xj;
  if (ToulBar2::vacAlternative) {
    queueVAC2();
  }
  if (ToulBar2::vac) {
    for (ConstraintList::iterator iter = getConstrs()->begin(); iter != getConstrs()->end(); ++iter) {
      xj = (VACVariable *) (*iter).constr->getVar(1 - (*iter).scopeIndex);
      xj->queueVAC2();
    }
  }
  EnumeratedVariable::extend(value, cost);
}

void VACVariable::increase(Value newInf) {
  if ((newInf > inf) && (newInf < sup)) {
    queueVAC2();
  }
  EnumeratedVariable::increase(newInf);
}

void VACVariable::decrease(Value newSup) {
  if ((newSup < sup) && (newSup > inf)) {
    queueVAC2();
  }
  EnumeratedVariable::decrease(newSup);
}

void VACVariable::repair () {
  for (unsigned int i = 0; i < getDomainSize(); i++) {
    costs[toIndex(vacValues[i]->getValue())] -= deltaCost;
  }
  deltaCost = 0;
}

void VACVariable::setTop (const Cost s) {
  for (unsigned int i = 0; i < getDomainSize(); i++) {
    if ((s + wcsp->getLb() + getVACCost(i) >= wcsp->getUb()) && (costs[toIndex(vacValues[i]->getValue())] < wcsp->getUb() + deltaCost - wcsp->getLb())) {
      costs[toIndex(vacValues[i]->getValue())] = wcsp->getUb() + deltaCost - wcsp->getLb();
      assert(wcsp->getLb() + getVACCost(i) == wcsp->getUb());
    }
  }
}


/**
 * VACConstraint:
 *   A class that stores information about a binary constraint
 */

VACConstraint::VACConstraint (WCSP *wcsp, EnumeratedVariable *xx, EnumeratedVariable *yy, vector<Cost> &tab, StoreStack<Cost, Cost> *storeCost) : BinaryConstraint(wcsp, xx, yy, tab, storeCost) {
  init(getX()->getDomainSize(), getY()->getDomainSize());
}

VACConstraint::VACConstraint (WCSP *wcsp, StoreStack<Cost, Cost> *storeCost) : BinaryConstraint(wcsp, storeCost) {
  init(wcsp->maxdomainsize, wcsp->maxdomainsize);
}

VACConstraint::~VACConstraint () {
  delete[] support[0];
  delete[] support[1];
  delete[] k[0];
  delete[] k[1];
}

void VACConstraint::init (int sizeX, int sizeY) {
  support[0] = new unsigned int[sizeX];
  k[0] = new Cost[sizeX];
  support[1] = new unsigned int[sizeY];
  k[1] = new Cost[sizeY];
  for (int i = 0; i < sizeX; i++) {
    support[0][i] = 0;
  }
  for (int j = 0; j < sizeY; j++) {
    support[1][j] = 0;
  }
}

void VACConstraint::reset () {
  for (unsigned int i = 0; i < getX()->getDomainSize(); i++) {
    k[0][i] = 0;
    if (support[0][i] >= getY()->getDomainSize()) {
      support[0][i] = 0;
    }
  }
  for (unsigned int j = 0; j < getY()->getDomainSize(); j++) {
    k[1][j] = 0;
    if (support[1][j] >= getX()->getDomainSize()) {
      support[1][j] = 0;
    }
  }
}

VACVariable *VACConstraint::getX () const {
  return (VACVariable *) x;
}

VACVariable *VACConstraint::getY () const {
  return (VACVariable *) y;
}

VACVariable *VACConstraint::getVariable (const int i) const {
  assert((i == 0) || (i == 1));

  if (i == 0) {
    return getX();
  }
  if (i == 1) {
    return getY();
  }
  return NULL;
}

void VACConstraint::VACproject (const int v, const unsigned int i, const Cost c) {
  assert((v == 0) || (v == 1));
  assert(i < getVariable(v)->getDomainSize());
  int index = getVariable(v)->toIndex(getVariable(v)->getValue(i)->getValue());

  if (v == 0) {
    deltaCostsX[index] += c;
  }
  if (v == 1) {
    deltaCostsY[index] += c;
  }
  getVariable(v)->VACproject(i, c);
}

void VACConstraint::VACextend(const int v, const unsigned int i, const Cost c) {
  assert((v == 0) || (v == 1));
  assert(i < getVariable(v)->getDomainSize());
  int index = getVariable(v)->toIndex(getVariable(v)->getValue(i)->getValue());

  if (v == 0) {
    deltaCostsX[index] -= c;
  }
  if (v == 1) {
    deltaCostsY[index] -= c;
  }
  getVariable(v)->VACextend(i, c);
  x->queueAC();
  y->queueDAC();
}

Cost VACConstraint::getVACCost (const unsigned int v, const unsigned int w, const int i) {
  assert((i == 0) || (i == 1));
  Cost c = 0;
  if (i == 0) c = getCost(getX()->getValue(v)->getValue(), getY()->getValue(w)->getValue());
  if (i == 1) c = getCost(getX()->getValue(w)->getValue(), getY()->getValue(v)->getValue());
  if(c < ToulBar2::costThreshold) c = 0;
  assert((c == 0) || (c >= ToulBar2::costThreshold));
  return c;
}

Cost VACConstraint::getIniCost (const unsigned int v, const unsigned int w, const int i) {
  assert((i == 0) || (i == 1));
  Cost c = 0;
  if (i == 0) c = getCost(getX()->getValue(v)->getValue(), getY()->getValue(w)->getValue());
  if (i == 1) c = getCost(getX()->getValue(w)->getValue(), getY()->getValue(v)->getValue());
  return c;
}

void VACConstraint::setCost (const unsigned int v, const unsigned int w, const int i, const Cost c) {
  Value valueX = getX()->getValue((i == 0)? v: w)->getValue();
  Value valueY = getY()->getValue((i == 0)? w: v)->getValue();
  int indexX = x->toIndex(valueX);
  int indexY = y->toIndex(valueY);
  assert(c >= 0);
  costs[indexX * sizeY + indexY] = c + deltaCostsX[indexX] + deltaCostsY[indexY];
  assert(getVACCost(v, w, i) == c);
}

void VACConstraint::decreaseCost (const unsigned int v, const unsigned int w, const int i, const Cost c) {
  Value valueX = getX()->getValue((i == 0)? v: w)->getValue();
  Value valueY = getY()->getValue((i == 0)? w: v)->getValue();
  int indexX = x->toIndex(valueX);
  int indexY = y->toIndex(valueY);
  assert((getVACCost(v, w, i) >= c) || (wcsp->getLb() + getVACCost(v, w, i) >= wcsp->getUb()));
  if (wcsp->getLb() + getVACCost(v, w, i) < wcsp->getUb()) {
    costs[indexX * sizeY + indexY] -= c;
  }
}

void VACConstraint::repair () {
  Value valueX;
  Value valueY;
  int indexX;
  int indexY;

  for (unsigned int v = 0; v < x->getDomainSize(); v++) {
    valueX = ((VACVariable *) x)->getValue(v)->getValue();
    indexX = ((VACVariable *) x)->toIndex(valueX);
    for (unsigned int w = 0; w < y->getDomainSize(); w++) {
      valueY = ((VACVariable *) y)->getValue(w)->getValue();
      indexY = ((VACVariable *) y)->toIndex(valueY);
      costs[indexX * sizeY + indexY] -= deltaCostsX[indexX] + deltaCostsY[indexY];
    }
  }
  for (unsigned int v = 0; v < x->getDomainSize(); v++) {
    valueX = ((VACVariable *) x)->getValue(v)->getValue();
    indexX = ((VACVariable *) x)->toIndex(valueX);
    deltaCostsX[indexX] = 0;
  }
  for (unsigned int w = 0; w < y->getDomainSize(); w++) {
    valueY = ((VACVariable *) y)->getValue(w)->getValue();
    indexY = ((VACVariable *) y)->toIndex(valueY);
    deltaCostsY[indexY] = 0;
  }
}

void VACConstraint::setTop (const Cost s) {
  Value valueX;
  Value valueY;
  int indexX;
  int indexY;

  for (unsigned int v = 0; v < x->getDomainSize(); v++) {
    valueX = ((VACVariable *) x)->getValue(v)->getValue();
    indexX = ((VACVariable *) x)->toIndex(valueX);
    for (unsigned int w = 0; w < y->getDomainSize(); w++) {
      valueY = ((VACVariable *) y)->getValue(w)->getValue();
      indexY = ((VACVariable *) y)->toIndex(valueY);
      if ((s + wcsp->getLb() + getVACCost(v, w, 0) >= wcsp->getUb()) && (costs[indexX * sizeY + indexY] < wcsp->getUb() + deltaCostsX[indexX] + deltaCostsY[indexY] - wcsp->getLb())) {
        costs[indexX * sizeY + indexY] = wcsp->getUb() + deltaCostsX[indexX] + deltaCostsY[indexY] - wcsp->getLb();
        assert(wcsp->getLb() + getVACCost(v, w, 0) == wcsp->getUb());
      }
    }
  }
}

Cost VACConstraint::getK (const int i, const unsigned int v) const {
  assert((i == 0) || (i == 1));
  assert(v < getVariable(i)->getDomainSize());

  return k[i][v];
}

bool VACConstraint::revise (const int i, const unsigned int v) {
  VACVariable *xj;
  Cost cost, minCost = wcsp->getUb();
  unsigned int minValue = support[i][v];
  assert((i == 0) || (i == 1));
  assert(v < getVariable(i)->getDomainSize());

  xj = getVariable(1-i);
  if ((!xj->getValue(support[i][v])->isRemoved()) && (!getVACCost(v, support[i][v], i))) {
    return false;
  }
  for (unsigned int w = ((support[i][v]+1) % xj->getDomainSize()); w != support[i][v]; w = (w+1) % xj->getDomainSize()) {
    if (!xj->getValue(w)->isRemoved()) {
      cost = getVACCost(v, w, i);
      if (!cost) {
        support[i][v] = w;
        return false;
      }
      else {
        if (cost < minCost) {
          minCost = cost;
          minValue = w;
        }
      }
    }
  }
  support[i][v] = minValue;
  return true;
}

void VACConstraint::setK (const int i, const unsigned int v, const Cost c) {
  assert((i == 0) || (i == 1));
  assert(v < getVariable(i)->getDomainSize());

  k[i][v] = c;
}


/**
 * VACOddsRecorder
 *   A class that stores odds about the number of variables used to enforce VAC
 */
VACOddsRecorder::VACOddsRecorder (int nv) : nbVariables(nv) {
  used = new bool[nbVariables];
  reset();
}

VACOddsRecorder::~VACOddsRecorder () {
  delete[] used;
}

void VACOddsRecorder::reset () {
  nbTotalUsed = 0;
  nbMinUsed = nbVariables+1;
  nbMaxUsed = 0;
  nbCalls = 0;
  nbRationalCalls = 0;
  nbIntegerCalls = 0;
  for (int i = 0; i < nbVariables; i++) {
    used[i] = false;
  }
}

void VACOddsRecorder::addVariable (const int i) {
  assert ((i >= 0) && (i < nbVariables));
  used[i] = true;
}

void VACOddsRecorder::addNbRationalCalls () {
  nbRationalCalls++;
}

void VACOddsRecorder::addNbIntegerCalls () {
  nbIntegerCalls++;
}

void VACOddsRecorder::computeOdds () {
  int nbUsed = 0;
  for (int i = 0; i < nbVariables; i++) {
    if (used[i]) {
      nbUsed++;
    }
  }
  nbTotalUsed += nbUsed;
  nbMinUsed = ((nbMinUsed < nbUsed)? nbMinUsed: nbUsed);
  nbMaxUsed = ((nbMinUsed > nbUsed)? nbMinUsed: nbUsed);
  nbCalls++;
}

float VACOddsRecorder::getMeanUsed () const {
  return ((float) nbTotalUsed / (float) nbCalls);
}

int VACOddsRecorder::getMinUsed () const {
  return nbMinUsed;
}

int VACOddsRecorder::getMaxUsed () const {
  return nbMaxUsed;
}

int VACOddsRecorder::getNbRationalCalls () const {
  return nbRationalCalls;
}

int VACOddsRecorder::getNbIntegerCalls () const {
  return nbIntegerCalls;
}
