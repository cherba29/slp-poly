#ifndef NTRP_CONTEXT_PROCESSOR_H
#define NTRP_CONTEXT_PROCESSOR_H

/**
 * @file Processor.h Definition of context::Processor class
 */

namespace context {
class Assignment;
class Determinant;
class Function;
class Id;
class Integer;
class Matrix;
class NamedValue;
class Negation;
class Power;
class Product;
class RealNumber;
class String;
class Sum;
class Table;
class ValueList;

/**
 * @brief Processor implimentes Visitor pattern for context::Value
 * @sa context::Value
 */
class Processor {
 public:
  virtual void process(const Assignment& v) = 0;
  virtual void process(const Determinant& v) = 0;
  virtual void process(const Function& v) = 0;
  virtual void process(const Id& v) = 0;
  virtual void process(const Integer& v) = 0;
  virtual void process(const Matrix& v) = 0;
  virtual void process(const NamedValue& v) = 0;
  virtual void process(const Negation& v) = 0;
  virtual void process(const Power& v) = 0;
  virtual void process(const Product& v) = 0;
  virtual void process(const RealNumber& v) = 0;
  virtual void process(const String& v) = 0;
  virtual void process(const Sum& v) = 0;
  virtual void process(const Table& v) = 0;
  virtual void process(const ValueList& v) = 0;
};

}  // namespace context

#endif  // NTRP_CONTEXT_PROCESSOR_H
