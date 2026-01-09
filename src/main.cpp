#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include <iostream>
#include <memory>
#include <vector>


// Global LLVM variables
static std::unique_ptr<llvm::LLVMContext> TheContext;
static std::unique_ptr<llvm::Module> TheModule;
static std::unique_ptr<llvm::IRBuilder<>> Builder;

void InitializeModule(){
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("my_cool_jit", *TheContext);
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
}

class ExprAST {
    public:
        virtual ~ExprAST() = default;
        // Turn into a LLVM value
        virtual llvm::Value *codegen() = 0;
};

class NumberExprAST : public ExprAST{
    int val;
    public:
        NumberExprAST(int val){
            this->val = val;
        }
        llvm::Value *codegen() override;
};

class BinaryExprAST : public ExprAST {
    std::unique_ptr<ExprAST> e1, e2;
    char op;
    public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> e1, std::unique_ptr<ExprAST> e2)
        : op(op), e1(std::move(e1)), e2(std::move(e2)) {}
    llvm::Value *codegen() override;
};

llvm::Value *NumberExprAST::codegen() {
    // "Get me a 32-bit integer constant with this value"
    return llvm::ConstantInt::get(*TheContext, llvm::APInt(32, val, true));
}

llvm::Value *BinaryExprAST::codegen() {
    // get the pointer to the left
    llvm::Value *l = e1->codegen();

    // get the pointer to the right;
    llvm::Value *r = e2->codegen();

    // check if both are valid
    if(l == nullptr || r == nullptr){
        return nullptr;
    }

    switch (op) {
        case '+':
            return Builder->CreateAdd(l, r, "addtmp");
        case '-':
            return Builder->CreateSub(l, r, "subtmp");
        case '*':
            return Builder->CreateMul(l, r, "multmp");
        default:
            return nullptr; 
    }
}

int main() {
    InitializeModule();

    // AST
    auto lhs = std::make_unique<NumberExprAST>(4);
    auto rhs = std::make_unique<NumberExprAST>(5);
    auto root = std::make_unique<BinaryExprAST>('+', std::move(lhs), std::move(rhs));

    // Main function
    llvm::FunctionType *mainFunctionType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), false);
    llvm::Function *mainFunction = llvm::Function::Create(mainFunctionType, llvm::Function::ExternalLinkage, "main", TheModule.get());

    // Basic block
    llvm::BasicBlock *mainBlock = llvm::BasicBlock::Create(*TheContext, "test", mainFunction);
    Builder->SetInsertPoint(mainBlock);

    // Return vale
    llvm::Value *RetVal = root->codegen();

    // Return statement
    Builder->CreateRet(RetVal);

    // Verify and print
    llvm::verifyFunction(*mainFunction);
    TheModule->print(llvm::outs(), nullptr);

    
    return 0;
}