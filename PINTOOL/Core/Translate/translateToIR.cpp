#include "translateToIR.h"

// CONSTRUCTEUR
TranslateToIR::TranslateToIR(): 
    _formula(std::ostringstream::ate),
    _iAssert(0),
    _iTbit(0), _iTb(0), _iTw(0), _iTdw(0), _iTqw(0), _iTdqw(0) {}

void TranslateToIR::declareObject(const TaintPtr &tPtr) 
{
    // traitement uniquement si la variable n'a jamais �t� d�clar�e 
    if ( !tPtr->isDeclared()) 
    {
        // d�claration de la variable
        tPtr->setDeclared();

        // r�cup�ration des sources, et d�clarations r�cursives de celles ci
        // jusqu'a arriver � un objet "source"
        const std::vector<ObjectSource> sources = tPtr->getSources();
        for (auto it = sources.begin() ; it != sources.end() ; ++it) 
        {
            if ((*it).isSrcTainted()) this->declareObject((*it).getTaintedSource());
        }

        // traduction en repr�sentation interm�diaire de cet objet en fonction de sa relation
        switch (tPtr->getSourceRelation())
        {
        case BYTESOURCE:    this->translate_BYTESOURCE(tPtr); break;
        case EXTRACT:       this->translate_EXTRACT(tPtr); break;
        case CONCAT:        this->translate_CONCAT(tPtr); break;

        case X_ASSIGN:      this->translate_X_ASSIGN(tPtr); break;
        case X_SIGNEXTEND:  this->translate_X_SIGNEXTEND(tPtr); break;
        case X_ZEROEXTEND:  this->translate_X_ZEROEXTEND(tPtr); break;
        case X_ADD:         this->translate_X_ADD(tPtr); break;
        case X_INC:         this->translate_X_INC(tPtr); break;
        case X_SUB:         this->translate_X_SUB(tPtr); break;
        case X_DEC:         this->translate_X_DEC(tPtr); break;
        case X_NEG:         this->translate_X_NEG(tPtr); break;
        case X_MUL:         this->translate_X_MUL(tPtr); break;
        case X_IMUL:        this->translate_X_IMUL(tPtr); break;
        case X_DIV_QUO:     this->translate_X_DIV_QUO(tPtr); break;
        case X_DIV_REM:     this->translate_X_DIV_REM(tPtr); break;
        case X_IDIV_QUO:    this->translate_X_IDIV_QUO(tPtr); break;
        case X_IDIV_REM:    this->translate_X_IDIV_REM(tPtr); break;
        case X_AND:         this->translate_X_AND(tPtr); break;
        case X_OR:          this->translate_X_OR(tPtr); break;
        case X_XOR:         this->translate_X_XOR(tPtr); break;
        case X_NOT:         this->translate_X_NOT(tPtr); break;
        case X_SHL:         this->translate_X_SHL(tPtr); break;
        case X_SHR:         this->translate_X_SHR(tPtr); break;
        case X_SAR:         this->translate_X_SAR(tPtr); break;
        case X_ROR:         this->translate_X_ROR(tPtr); break;
        case X_ROL:         this->translate_X_ROL(tPtr); break;
        case X_RCR:         this->translate_X_RCR(tPtr); break;
        case X_RCL:         this->translate_X_RCL(tPtr); break;
        case X_SETCC:       this->translate_X_SETCC(tPtr); break;
        case X_COMPLEMENT_BIT:  this->translate_X_COMPLEMENT_BIT(tPtr); break;
        case X_SET_BIT:     this->translate_X_SET_BIT(tPtr); break;
        case X_CLEAR_BIT:   this->translate_X_CLEAR_BIT(tPtr); break;
        case X_BSF:         this->translate_X_BSF(tPtr); break;
        case X_BSR:         this->translate_X_BSR(tPtr); break;

        case F_LSB:         this->translate_F_LSB(tPtr); break;
        case F_MSB:         this->translate_F_MSB(tPtr); break;
        case F_CARRY_ADD:   this->translate_F_CARRY_ADD(tPtr); break;
        case F_CARRY_SUB:   this->translate_F_CARRY_SUB(tPtr); break;
        case F_CARRY_NEG:   this->translate_F_CARRY_NEG(tPtr); break;
        case F_CARRY_MUL:   this->translate_F_CARRY_MUL(tPtr); break;
        case F_CARRY_IMUL:  this->translate_F_CARRY_IMUL(tPtr); break;
        case F_CARRY_SHL:   this->translate_F_CARRY_SHL(tPtr); break;
        case F_CARRY_SHR:   this->translate_F_CARRY_SHR(tPtr); break; // SAR = idem
        case F_CARRY_RCL:   this->translate_F_CARRY_RCL(tPtr); break;
        case F_CARRY_RCR:   this->translate_F_CARRY_RCR(tPtr); break;
        case F_CARRY_BITBYTE:   this->translate_F_CARRY_BITBYTE(tPtr); break;  
        case F_PARITY:          this->translate_F_PARITY(tPtr); break;
        case F_IS_NULL:         this->translate_F_IS_NULL(tPtr); break;
        case F_ARE_EQUAL:       this->translate_F_ARE_EQUAL(tPtr); break;
        case F_CMPXCHG_8B16B:   this->translate_F_CMPXCHG_8B16B(tPtr); break;
        case F_OVERFLOW_ADD:    this->translate_F_OVERFLOW_ADD(tPtr); break;
        case F_OVERFLOW_SUB:    this->translate_F_OVERFLOW_SUB(tPtr); break;
        case F_OVERFLOW_INC:    this->translate_F_OVERFLOW_INC(tPtr); break;
        case F_OVERFLOW_DEC:    this->translate_F_OVERFLOW_DEC(tPtr); break; // NEG = idem
        case F_OVERFLOW_SHL:    this->translate_F_OVERFLOW_SHL(tPtr); break;
        case F_OVERFLOW_SHRD:   this->translate_F_OVERFLOW_SHRD(tPtr); break;
        case F_OVERFLOW_ROL:    this->translate_F_OVERFLOW_ROL(tPtr); break; 
        case F_OVERFLOW_ROR:    this->translate_F_OVERFLOW_ROR(tPtr); break; 
        case F_AUXILIARY_ADD:   this->translate_F_AUXILIARY_ADD(tPtr); break;
        case F_AUXILIARY_NEG:   this->translate_F_AUXILIARY_NEG(tPtr); break;
        case F_AUXILIARY_SUB:   this->translate_F_AUXILIARY_SUB(tPtr); break;
        case F_AUXILIARY_INC:   this->translate_F_AUXILIARY_INC(tPtr); break;
        case F_AUXILIARY_DEC:   this->translate_F_AUXILIARY_DEC(tPtr); break;
        }
    }
} // declareObject


void TranslateToIR::addConstraintJcc(TaintManager_Thread *pTmgrTls, PREDICATE pred, 
        bool isTaken, ADDRINT insAddress, ADDRINT flagsOrRegValue)
{
    // nouvelle contrainte => ajustement de son num�ro
    ++_iAssert;
    // entete (informations sur la contrainte)
    _formula << getConstraintHeader(insAddress, pred);
    // contrainte en elle-meme qui d�clare r�cursivement les objets impliqu�s
    _formula << getPredicateTranslation(pTmgrTls, pred, flagsOrRegValue);
    // final = valeur de la contrainte � l'ex�cution
    _formula << getConstraintFooter(isTaken);

    // Si le nombre maximal de contraintes est atteint : quitter le pintool 
    // via la fonction "Fini" avec le code EXIT_MAX_CONSTRAINTS
    // si g_maxConstraints est nul, ce cas n'arrive jamais (la premi�re contrainte est la n�1)
    if (_iAssert == g_maxConstraints)  PIN_ExitApplication(EXIT_MAX_CONSTRAINTS);
}

    