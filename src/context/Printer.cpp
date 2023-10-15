/**
 * @file Printer.cpp Implementation of constext::Printer
 *
 */

#include "Printer.h"

namespace context {

void Printer::process(const context::Sum& v) {
  size_t nTerms = v.getNumberOfNodes();
  this->os_ << "(";
  for (size_t i = 0; i < nTerms; ++i) {
    if (v.isPos(i)) {
      if (i) this->os_ << " + ";
    } else {
      this->os_ << " - ";
    }
    v.getNode(i).accept(*this);
  }
  this->os_ << ")";
}

void Printer::process(const context::Assignment& v) {
  os_ << v.getId() << " <-- ";
  v.getValue().accept(*this);
}
void Printer::process(const context::Determinant& v) {
  const context::Matrix& mtx = v.getMatrix();
  os_ << "det(" << mtx.getName() << ")"
      << "# " << mtx.getNRows() << "x" << mtx.getNCols();
  mtx.accept(*this);
}

void Printer::process(const context::Function& /* v */) {}

void Printer::process(const context::Id& v) { os_ << v.getName(); }

void Printer::process(const context::Integer& v) { os_ << v.getInt(); }

void Printer::process(const context::Matrix& /* v */) {}

void Printer::process(const context::Product& v) {
  size_t nTerms = v.getNumberOfNodes();
  for (size_t i = 0; i < nTerms; ++i) {
    if (v.isPos(i)) {
      if (i) os_ << "*";
    } else {
      if (i)
        os_ << "/";
      else
        os_ << "1/";
    }
    v.getNode(i).accept(*this);
  }
}

void Printer::process(const context::NamedValue& /* v */) {}

void Printer::process(const context::Negation& v) {
  os_ << "-";
  v.getValue().accept(*this);
}

void Printer::process(const context::Power& v) {
  v.getLeftValue().accept(*this);
  os_ << "^" << v.getExponent();
}

void Printer::process(const context::RealNumber& /* v */) {}

void Printer::process(const context::String& /* v */) {}

void Printer::process(const context::Table& /* v */) {}

void Printer::process(const context::ValueList& /* v */) {}

}  // namespace context
