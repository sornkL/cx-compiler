#include "ExprAST.h"
#include "BlockAST.h"
#include "../utils/Logger.h"
#include "../cx/CX.h"
#include <vector>

class SwitchExprAST : public ExprAST {
private:
    std::unique_ptr<ExprAST> condition;
    std::vector<std::pair<std::unique_ptr<ExprAST>, std::unique_ptr<BlockAST>>> cases;
    std::unique_ptr<BlockAST> default_case;

public:
    SwitchExprAST(std::unique_ptr<ExprAST> condition, 
                  std::vector<std::pair<std::unique_ptr<ExprAST>, std::unique_ptr<BlockAST>>> cases, 
                  std::unique_ptr<BlockAST> default_case)
        : condition(std::move(condition)), cases(std::move(cases)), default_case(std::move(default_case)) {}

    llvm::Value *codegen() override;
};