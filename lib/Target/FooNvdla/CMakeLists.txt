
include_directories(.)
include_directories(include)

add_libonnc_src(
    CodeEmitVisitor.cpp
    FooNvdlaBackend.cpp
    Loadable.cpp
    NvDlaDefine.cpp
    NvDlaMeta.cpp
    NvDlaUtil.cpp
    NvDlaMemInfoPass.cpp
    NvDlaTaskSubmitPass.cpp
    NvDlaFileGenPass.cpp
    GraphvizONNCIRPass.cpp
    UpsampleAsConvTransposePass.cpp
    Config/NvFull.cpp
    TargetInfo/FooNvdlaTargetInfo.cpp
    TargetInfo/FooNvdlaTargetMemInfo.cpp)
