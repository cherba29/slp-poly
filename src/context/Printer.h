#ifndef NTRP_CONTEXT_PRINTER_H
#define NTRP_CONTEXT_PRINTER_H

/**
 * @file Printer.h Definition of context::Printer class
 */

#include "context.h"

#include <ostream>

namespace context {

class Printer : public Processor {
  std::ostream& os_;

  virtual void process(const context::Assignment& v);
  virtual void process(const context::Determinant& v);
  virtual void process(const context::Function& v);
  virtual void process(const context::Id& v);
  virtual void process(const context::Integer& v);
  virtual void process(const context::Matrix& v);
  virtual void process(const context::NamedValue& v);
  virtual void process(const context::Negation& v);
  virtual void process(const context::Power& v);
  virtual void process(const context::Product& v);
  virtual void process(const context::RealNumber& v);
  virtual void process(const context::String& v);
  virtual void process(const context::Sum& v);
  virtual void process(const context::Table& v);
  virtual void process(const context::ValueList& v);

 public:
  Printer(std::ostream& os) : os_(os) {}
};

}  // namespace context

#endif  // NTRP_CONTEXT_PRINTER_H
