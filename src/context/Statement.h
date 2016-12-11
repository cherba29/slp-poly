#ifndef NTRP_CONTEXT_STATEMENT_H
#define NTRP_CONTEXT_STATEMENT_H

/**
 * @file Statement.h Definition of parsed context::Statement class
 */

namespace context {
class Processor;

class Statement {
public:
  virtual void accept(Processor& proc) const = 0;
  virtual ~Statement() {}
};

}  // namespace context

#endif // NTRP_CONTEXT_STATEMENT_H
