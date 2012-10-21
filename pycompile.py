
import os
import pickle
import subprocess as sp
CPPOX = "-std=c++0x "
CPPSD = ""

def Clean(exname):
    try:
        os.remove(exname + ".cache")
    except:
        pass

def CompileSharedLibrary(exname, src_dir,obj_dir,filelist, libraries, dbg, opt, cppv=CPPOX, libpath=[], incpath=[]):
    cachename = exname + ".cache"	

    def bind_symbols(symbollist,symhandle):
        print "SYMB:",type(symbollist)
        out = ""
        for i in symbollist:
            if i != "":
                out = out + symhandle(i) + " "
        if out != "":
            out = out[:-1]
        return out

    incpath = bind_symbols(incpath,lambda x: "-I{0}".format(x))
    print "INCLUDE PATH: ", incpath
    def load_cache():
        try:
            with open(cachename, "r") as fptr:
                pycache = pickle.load(fptr)
        except:
            pycache = {}
            try:
                os.mkdir("./obj")
            except:
                pass
        return pycache

    def save_cache(pycache):
        try:
            with open(cachename, "w") as fptr:
                pickle.dump(pycache,fptr)
        except:
            print "Error saving cache"

    def fix_dir(path):
        dirs = path.split("/")[:-1]
        acc_dir = ""
        for i in dirs:
            acc_dir = os.path.join(acc_dir,i)
            try:
                os.mkdir(acc_dir)
            except:
                pass
    
    def do_call(call):
        call_split = call.split(" ")
        call_split = filter(lambda x: x != "", call_split)
        p = sp.check_call(call_split)
        return p
    pycache = load_cache()

    includes = ""
    lib_path=""
    join = os.path.join
    object_files = []
    for i in filelist:
        code_path = join(src_dir,i)
        chtime = os.path.getmtime(code_path)
        dot_point = i.rfind('.')
        i_stripped = i[:dot_point]
        object_path = join(obj_dir,i_stripped) + ".o"
        fix_dir(object_path)
        object_files.append(object_path)
        print ">",i_stripped
        if i not in pycache.keys() or pycache[i] < chtime:
            call = ("{0} -fPIC -c {1} {2}-O{3} -g{4} -std=c++0x {6} -o {5}").format("g++", code_path,includes,opt,dbg,object_path,incpath)
            rmcall ="rm " + object_path
            
            try:
                try:
                    print rmcall
                    do_call(rmcall)
                except:
                    pass
                print call
                do_call(call)
                pycache[i] = os.path.getmtime(object_path)
                save_cache(pycache)
            except:
                raise "Error during compiling.. quitting"
                
        
    
                
    link_inc = bind_symbols(libpath,lambda x: "-L{0}".format(x))
    libraries = bind_symbols(libraries,lambda x: "-l{0}".format(x))
    
    link_args = ""
    for i in object_files:
        if(i != ""):
            link_args += i + " "
            
    link_args = link_args[:-1]

    if(libraries != ""):
        link_args += " " + libraries
    link_call = "clang -shared {0} -O{1} -g{2} {4} -std=c++0x -o {3}".format(link_args,opt,dbg,exname,link_inc)
    print link_call
    do_call(link_call)


def Compile(exname, src_dir,obj_dir,filelist, libraries, dbg, opt, cppv=CPPOX, libpath=[], incpath=[], linkargs=[]):
    cachename = exname + ".cache"	

    def bind_symbols(symbollist,symhandle):
        print "SYMB:",type(symbollist)
        out = ""
        for i in symbollist:
            if i != "":
                out = out + symhandle(i) + " "
        if out != "":
            out = out[:-1]
        return out

    incpath = bind_symbols(incpath,lambda x: "-I{0}".format(x))
    print "INCLUDE PATH: ", incpath
    def load_cache():
        try:
            with open(cachename, "r") as fptr:
                pycache = pickle.load(fptr)
        except:
            pycache = {}
            try:
                os.mkdir("./obj")
            except:
                pass
        return pycache

    def save_cache(pycache):
        try:
            with open(cachename, "w") as fptr:
                pickle.dump(pycache,fptr)
        except:
            print "Error saving cache"

    def fix_dir(path):
        dirs = path.split("/")[:-1]
        acc_dir = ""
        for i in dirs:
            acc_dir = os.path.join(acc_dir,i)
            try:
                os.mkdir(acc_dir)
            except:
                pass
    
    def do_call(call):
        call_split = call.split(" ")
        call_split = filter(lambda x: x != "", call_split)
        p = sp.check_call(call_split)
        return p
    pycache = load_cache()

    includes = ""
    lib_path=""
    join = os.path.join
    object_files = []
    for i in filelist:
        code_path = join(src_dir,i)
        chtime = os.path.getmtime(code_path)
        dot_point = i.rfind('.')
        i_stripped = i[:dot_point]
        object_path = join(obj_dir,i_stripped) + ".o"
        fix_dir(object_path)
        object_files.append(object_path)
        print ">",i_stripped
        if i not in pycache.keys() or pycache[i] < chtime:
            call = ("{0} -c {1} {2}-O{3} -g{4} -std=c++0x {6} -o {5}").format("clang++", code_path,includes,opt,dbg,object_path,incpath)
            rmcall ="rm " + object_path
            
            try:
                try:
                    print rmcall
                    do_call(rmcall)
                except:
                    pass
                print call
                do_call(call)
                pycache[i] = os.path.getmtime(object_path)
                save_cache(pycache)
            except:
                raise "Error during compiling.. quitting"
                
        
    
                
    link_inc = bind_symbols(libpath,lambda x: "-L{0}".format(x))
    libraries = bind_symbols(libraries,lambda x: "-l{0}".format(x))
    
    link_args = ""
    for i in object_files:
        if(i != ""):
            link_args += i + " "
    
    for i in linkargs:
        link_args += i + " "
            
    link_args = link_args[:-1]

    if(libraries != ""):
        link_args += " " + libraries
    link_call = "clang++ {0} -O{1} -g{2} {4} -std=c++0x -o {3}".format(link_args,opt,dbg,exname,link_inc)
    print link_call
    do_call(link_call)

if __name__ == "__main__":
    test()
