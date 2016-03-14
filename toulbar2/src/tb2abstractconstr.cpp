/*
 * **************** Abstract constraints of predefined arities **************
 */

#include "tb2abstractconstr.hpp"

/*
 * Constructors and misc.
 * 
 */

/// \return size of the cartesian product of all initial domains in the constraint scope.
/// \warning use deprecated MAX_DOMAIN_SIZE for performance.
Long AbstractNaryConstraint::getDomainInitSizeProduct()
{
    if (arity()==0) return 0;
    Long cartesianProduct = 1;
    for (int i=0; i<arity(); i++) {
        // trap overflow numbers
        if (cartesianProduct > LONGLONG_MAX / MAX_DOMAIN_SIZE) return LONGLONG_MAX;
        cartesianProduct *= scope[i]->getDomainInitSize();
    }
    return cartesianProduct;
}

// sorts scope variables by increasing DAC order
int cmpDAC(const void *p1, const void *p2)
{
    EnumeratedVariable *var1 = *((EnumeratedVariable **) p1);
    EnumeratedVariable *var2 = *((EnumeratedVariable **) p2);
    int v1 = var1->getDACOrder();
    int v2 = var2->getDACOrder();
    if (v1 > v2) return 1;
    else if (v1 < v2) return -1;
    else return 0;
}

// projects n-ary cost function of arity less than 3 into a unary/binary/ternary cost function in extension before the search
void AbstractNaryConstraint::projectNaryBeforeSearch()
{
    assert(arity()<=3);
    deconnect();   // Warning! It assumes the default cost is not used if the cost function has zero arity
    if (arity()==3) {
        vector<Cost> costs;
        EnumeratedVariable *x = (EnumeratedVariable *) getVar(0);
        EnumeratedVariable *y = (EnumeratedVariable *) getVar(1);
        EnumeratedVariable *z = (EnumeratedVariable *) getVar(2);
        unsigned int sizeX = x->getDomainInitSize();
        unsigned int sizeY = y->getDomainInitSize();
        unsigned int sizeZ = z->getDomainInitSize();
        for (unsigned int a = 0; a < sizeX; a++) {
            for (unsigned int b = 0; b < sizeY; b++) {
                for (unsigned int c = 0; c < sizeZ; c++) {
                    costs.push_back(getDefCost());
                }
            }
        }
        Cost cost;
        String t;
        first();
        while(next(t,cost)) {
            unsigned int a = t[0]-CHAR_FIRST;
            unsigned int b = t[1]-CHAR_FIRST;
            unsigned int c = t[2]-CHAR_FIRST;
            costs[a * sizeY * sizeZ + b * sizeZ + c] = cost;
        }
        wcsp->postTernaryConstraint(x->wcspIndex, y->wcspIndex, z->wcspIndex, costs);
    } else if (arity()==2) {
        vector<Cost> costs;
        EnumeratedVariable *x = (EnumeratedVariable *) getVar(0);
        EnumeratedVariable *y = (EnumeratedVariable *) getVar(1);
        unsigned int sizeX = x->getDomainInitSize();
        unsigned int sizeY = y->getDomainInitSize();
        for (unsigned int a = 0; a < sizeX; a++) {
            for (unsigned int b = 0; b < sizeY; b++) {
                costs.push_back(getDefCost());
            }
        }
        Cost cost;
        String t;
        first();
        while(next(t,cost)) {
            unsigned int a = t[0]-CHAR_FIRST;
            unsigned int b = t[1]-CHAR_FIRST;
            costs[a * sizeY + b] = cost;
        }
        wcsp->postBinaryConstraint(x->wcspIndex, y->wcspIndex, costs);
    } else if (arity()==1) {
        vector<Cost> costs;
        EnumeratedVariable *x = (EnumeratedVariable *) getVar(0);
        unsigned int sizeX = x->getDomainInitSize();
        for (unsigned int a = 0; a < sizeX; a++) {
            costs.push_back(getDefCost());
        }
        Cost cost;
        String t;
        first();
        while(next(t,cost)) {
            unsigned int a = t[0]-CHAR_FIRST;
            costs[a] = cost;
        }
        wcsp->postUnaryConstraint(x->wcspIndex, costs);
    }
}

/* Local Variables: */
/* c-basic-offset: 4 */
/* tab-width: 4 */
/* indent-tabs-mode: nil */
/* c-default-style: "k&r" */
/* End: */

