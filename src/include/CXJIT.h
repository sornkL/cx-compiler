#ifndef __CX_JIT_H__
#define __CX_JIT_H__

#include <llvm/ADT/StringRef.h>
#include <llvm/ExecutionEngine/JITSymbol.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/Core.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/ExecutorProcessControl.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/LLVMContext.h>
#include <memory>

namespace llvm {
namespace orc {

class CXJIT {
private:
    std::unique_ptr<ExecutionSession> es;
    DataLayout dl;
    MangleAndInterner mangle;

    RTDyldObjectLinkingLayer object_layer;
    IRCompileLayer compile_layer;

    JITDylib &main_jd;

public:
    CXJIT(JITTargetMachineBuilder jtmb, DataLayout dl) 
            : dl(std::move(dl)), mangle(*es, this->dl), 
            object_layer(*es, []() { return std::make_unique<SectionMemoryManager>(); }),
            compile_layer(*es, object_layer, std::make_unique<ConcurrentIRCompiler>(std::move(jtmb))),
            main_jd(es->createBareJITDylib("<main>")) {
        main_jd.addGenerator(cantFail(DynamicLibrarySearchGenerator::GetForCurrentProcess(dl.getGlobalPrefix())));
        if (jtmb.getTargetTriple().isOSBinFormatCOFF()) {
            object_layer.setOverrideObjectFlagsWithResponsibilityFlags(true);
            object_layer.setAutoClaimResponsibilityForObjectSymbols(true);
        }
    }

    ~CXJIT() {
        if (auto err = es->endSession()) {
            es->reportError(std::move(err));
        }
    }

    static Expected<std::unique_ptr<CXJIT>> create() {
        auto epc = SelfExecutorProcessControl::Create();
        if (!epc) {
            return epc.takeError();
        }

        es = std::make_unique<ExecutionSession>(std::move(*epc));

        JITTargetMachineBuilder jtmb(es->getExecutorProcessControl().getTargetTriple());

        auto dl = jtmb.getDefaultDataLayoutForTarget();
        if (!dl) {
            return dl.takeError();
    }

    return std::make_unique<CXJIT>(std::move(jtmb), std::move(*dl));
}

    const DataLayout &get_data_layout() const;

    JITDylib &get_main_jit_dylib();

    Error add_module(ThreadSafeModule tsm, ResourceTrackerSP rt = nullptr);

    Expected<JITEvaluatedSymbol> lookup(StringRef name);
};

}
}

#endif