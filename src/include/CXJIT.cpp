#include "CXJIT.h"

namespace llvm {
namespace orc {

const DataLayout &CXJIT::get_data_layout() const { 
    return dl; 
}

JITDylib &CXJIT::get_main_jit_dylib() { 
    return main_jd; 
}

Error CXJIT::add_module(ThreadSafeModule tsm, ResourceTrackerSP rt) {
    if (!rt)
        rt = main_jd.getDefaultResourceTracker();
    return compile_layer.add(rt, std::move(tsm));
}

Expected<JITEvaluatedSymbol> CXJIT::lookup(StringRef name) {
    return es->lookup({&main_jd}, mangle(name.str()));
}

}
}