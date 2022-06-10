import requests, zipfile, tarfile, io, os

LLVM_DOWNLOAD_URL = 'https://github.com/llvm/llvm-project/releases/download/llvmorg-14.0.0/llvm-14.0.0.src.tar.xz'
CLANG_DOWNLOAD_URL = 'https://github.com/llvm/llvm-project/releases/download/llvmorg-14.0.0/clang-14.0.0.src.tar.xz'
#CLANG_EXTRA_TOOLS_DOWNLOAD_URL = 
FMT_DOWNLOAD_URL = 'https://github.com/fmtlib/fmt/releases/download/8.1.1/fmt-8.1.1.zip'

LLVM_FILES_TO_EXTRACT = [
    'llvm-14.0.0.src/cmake/',
    'llvm-14.0.0.src/include/',
    'llvm-14.0.0.src/lib/',
    'llvm-14.0.0.src/projects/',
    'llvm-14.0.0.src/resources/',
    'llvm-14.0.0.src/runtimes/',
    'llvm-14.0.0.src/utils/TableGen/',
    'llvm-14.0.0.src/tools/lto/',
    'llvm-14.0.0.src/tools/llvm-ar/',
    'llvm-14.0.0.src/tools/llvm-config/',
    'llvm-14.0.0.src/tools/llvm-lto/',
    'llvm-14.0.0.src/tools/llvm-profdata/',
    'llvm-14.0.0.src/.clang-format',
    'llvm-14.0.0.src/.clang-tidy',
    'llvm-14.0.0.src/CMakeLists.txt',
    'llvm-14.0.0.src/LICENSE.TXT',
    'llvm-14.0.0.src/llvm.spec.in',
    'llvm-14.0.0.src/README.txt',
    'cmake/'
]

CLANG_FILES_TO_EXTRACT = [
    'clang-14.0.0.src/cmake/',
    'clang-14.0.0.src/include/',
    'clang-14.0.0.src/INPUTS/',
    'clang-14.0.0.src/lib/',
    'clang-14.0.0.src/runtime/',
    'clang-14.0.0.src/utils/',
    'clang-14.0.0.src/.clang-format',
    'clang-14.0.0.src/.clang-tidy',
    'clang-14.0.0.src/CMakeLists.txt',
    'clang-14.0.0.src/LICENSE.TXT',
    'clang-14.0.0.src/ModuleInfo.txt',
    'clang-14.0.0.src/README.txt',
]

def member(t : tarfile.TarFile, prefixes):
    for member in t.getmembers():
        if any(member.path.startswith(prefix) for prefix in prefixes):
            if member.path.startswith('llvm-14.0.0.src'):
                member.path = 'llvm' + member.path[len('llvm-14.0.0.src'):]
            elif member.path.startswith('clang-14.0.0.src'):
                member.path = 'clang' + member.path[len('clang-14.0.0.src'):]
            yield member

def downloadTar(url, path='', files_to_extract=None):
    r = requests.get(url, stream=True)
    if r.ok:
        t = tarfile.open(fileobj=io.BytesIO(r.content))
        if files_to_extract:
            t.extractall(members=member(t, files_to_extract), path=path)
        else:
            t.extractall(path=path)

def downloadZip(url, path=None):
    r = requests.get(url, stream=True)
    if r.ok:
        z = zipfile.ZipFile(io.BytesIO(r.content))
        z.extractall(path)

## Download llvm and clang
downloadTar(LLVM_DOWNLOAD_URL, files_to_extract=LLVM_FILES_TO_EXTRACT)
downloadTar(CLANG_DOWNLOAD_URL, files_to_extract=CLANG_FILES_TO_EXTRACT)

## Download fmt library
downloadZip(FMT_DOWNLOAD_URL, path='clang-tools-extra/ApexReflectTool/')

## 
