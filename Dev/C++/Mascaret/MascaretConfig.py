#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
## LI2/ENIB - Mascaret
##----------------------------------------------------------------------------
## file        : MascaretConfig, based on TransProgConfig.py
## author(s)   : F. Harrouet
##----------------------------------------------------------------------------
## Copyright (C) 2002-2006  LI2/ENIB
##   Laboratoire d'Ingenierie Informatique (LI2)
##   Ecole Nationale d'Ingenieurs de Brest (ENIB)
##   CS 73862 - 29238 BREST Cedex 3 - France
##   Tel: +33(0)298 05 89 89, Fax: +33(0)298 05 89 79, e-mail: li2@enib.fr
##
## This software package is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public License as
## published by the Free Software Foundation; either version 2.1 of the
## License, or (at your option) any later version.
## This software package is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
## General Public License (file LICENSE-LGPL) for more details.
##----------------------------------------------------------------------------

"""MascaretConfig

MascaretConfig specific configuration tool to generate a makefile
"""

import sys
import os
import platform
import subprocess
import re
import glob
import copy
import traceback

#-----------------------------------------------------------------------------

class Tool(object):
  """Tool: main class to describe a Mascaret project"""

  class Error(Exception):
    def __init__(self,*args,**kw):
      Exception.__init__(self,*args,**kw)

  def __init__(self,useMascaret=True):
    """Tool creation, must be called once, this is a singleton"""
    if type(self)._instance:
      raise type(self).Error,'Only one instance of %s is allowed' \
                             % (type(self).__name__)
    type(self)._instance=self
    object.__init__(self)
    self._useMascaret=useMascaret
    self._stdIncRE=re.compile(r'^[ \t]*#[ \t]*include[ \t]*<([^>]+)>')
    self._locIncRE=re.compile(r'^[ \t]*#[ \t]*include[ \t]*"([^"]+)"')
    self._optRE=re.compile(r'^[ \t]*([a-zA-Z_][0-9a-zA-Z_]*)[ \t]*=(.*)$')
    self._programs=[]
    self._plugins=[]
    self._libraries=[]
    self._generators=[]
    self._cleanList=[]
    self._cleaning=False
    self._options={}
    self._system=platform.system()
    if self._system=='Linux':
      macName=platform.machine()
      if re.match(r'^i.86$',macName):
        self._system+='_x86_32'
      else:
        self._system+='_'+macName
    
    if type(self) is Tool:
      self.manageOptions(os.path.join(os.path.dirname(
                                      os.path.realpath(__file__)),
                                      'MascaretConfig.options'))
      self.settings=self.createDefaultSettings()

  def sysName(self):
    """Returns the system name (=platform.system())"""
    return self._system

  def manageOptions(self,optFile):
    """Parses option in option file and in command line"""
    try:
      self.parseOptionFile(optFile)
    except:
      pass
    self._cmdLine=[i for i in sys.argv if not self.parseOption(i)]

  def parseOptionFile(self,optFile):
    """Parses option file"""
    if isinstance(optFile,str):
      optFile=file(optFile,'r')
    self._options['self']=self
    try:
      exec optFile in self._options
    except:
      traceback.print_exc()
    self._options.pop('__builtins__',None)
    self._options.pop('self',None)

  def parseOption(self,optString):
    """Parses and integrates the option, returns success"""
    m=self._optRE.match(optString)
    if not m:
      return False
    name=m.string[m.start(1):m.end(1)]
    value=m.string[m.start(2):m.end(2)].strip()
    self._options['self']=self
    try:
      self._options[name]=eval(value,self._options)
    except Exception, exc:
      print 'Cannot evaluate option `%s\':' % (name)
      print '    `%s\'' % (value)
      print '    %s' % (exc)
      self._options[name]=None
    self._options.pop('__builtins__',None)
    self._options.pop('self',None)
    return True

  def getOption(self,opt):
    """Retreives option's value or None"""
    return self._options.get(opt)

  def testBoolOption(self,opt):
    """Tests if an option, considered as boolean, is set"""
    v=self._options.get(opt)
    if not v:
      return False
    if isinstance(v,bool):
      return v
    if isinstance(v,str):
      try:
        v=int(v)
      except:
        try:
          v=long(v)
        except:
          try:
            v=float(v)
          except:
            pass
    if isinstance(v,int) or isinstance(v,long) or isinstance(v,float):
      return v!=0
    if isinstance(v,str):
      v=v.lower()
      return (v=='true') or (v=='yes') or (v=='y') or (v=='on')
    return False

  def createDefaultSettings(self):
    """Create new settings with default values"""
    settings={}
    topDir=os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
    m=re.match(r'.*Mascaret_([0-9]{4})m([0-9]{2})d([0-9]{2}).*',
               os.path.basename(topDir))
    if m:
      mascaretVersion=m.string[m.start(1):m.end(1)]+ \
                       m.string[m.start(2):m.end(2)]+ \
                       m.string[m.start(3):m.end(3)]
    else:
      mascaretVersion='0'
    settings['MASCARET_TOP_DIR']=topDir
    settings['MASCARET_VERSION']=mascaretVersion
    settings['MAKE']=[]
    settings['FLEX']=[]
    settings['BISON']=[]
    settings['CC']=[]
    settings['DBGFLAGS']=[]
    settings['OPTFLAGS']=[]
    settings['CCFLAGS']=[]
    settings['DEFINES']={}
    settings['INCDIRS']=[os.path.curdir]
    settings['LD']=[]
    settings['LDFLAGS']=[]
    settings['LIBDIRS']=[os.path.curdir]
    settings['LIBS']=[]
    settings['USE_RPATH']=False
    if self._useMascaret:
      settings['DEFINES']['MASCARET_VERSION']=mascaretVersion
      settings['DEFINES']['MASCARET_DIR']='"%s"' % topDir
      settings['INCDIRS']+=[os.path.join(topDir,'include')]
      settings['LIBDIRS']+=[os.path.join(topDir,'lib')]
      settings['LIBS']+=['Mascaret']
    if self._system=='Windows':
      if self.getOption('toolset') == 'msvc' or self.getOption('toolset') == None:
        settings['MAKE']=['nmake.exe','/nologo','/k']
        settings['FLEX']=['flex.exe']
        settings['BISON']=['bison.exe']
        settings['CC']=['cl.exe','/nologo']
        settings['DBGFLAGS']+=['/Z7']
        #settings['OPTFLAGS']+=['/O2','/G7']
        settings['OPTFLAGS']+=['/O2']
        settings['CCFLAGS']+=['/W3','/EHsc']
        #settings['CCFLAGS']+=['/W4','/EHsc']
        settings['LD']+=['link.exe','/nologo']
        #settings['LIBS']+=['WS2_32']
      elif self.getOption('toolset') == 'mingw':
        settings['MAKE']=['mingw32-make.exe']
        settings['FLEX']=['flex.exe']
        settings['BISON']=['bison.exe'] 
        settings['CC']=['g++.exe']
        settings['OPTFLAGS']+=['-O3','-DNDEBUG','-ffast-math',
                               '-s','-fomit-frame-pointer']
        settings['CCFLAGS']+=['-W','-Wall','-Wno-long-long',
                              '-pedantic','-pipe']
        self._gccOptimizerWorkAround(settings)
        self._gccMathInlineWorkAround(settings)
        settings['LD']=settings['CC']
        settings['DEFINES']['_REENTRANT']=None
        settings['DBGFLAGS']+=['-g']
      else:
        print 'Toolset '+self.getOption('toolset')+' unknowned'
        exit(-1)
    else: # posix system
      settings['MAKE']=['make','-k']
      settings['FLEX']=['flex']
      settings['BISON']=['bison']
      if self._system=='IRIX':
        settings['CC']=['CC','-LANG:std','-n32']
        # nb: disable optimization for compilation time
        settings['OPTFLAGS']+=['-O0','-OPT:Olimit=0','-DNDEBUG']
        settings['CCFLAGS']+=['-KPIC','-ptv','-ptused','-fullwarn',
                              '-woff','1375','-woff','1424','-woff','3201',
                              '-woff','1110','-woff','1209']
        settings['DEFINES']['_SGI_MP_SOURCE']=None
        settings['LDFLAGS']+=['-Wl,-woff,15,-woff,84,-woff,85']
      elif self.testBoolOption('use_icc'):
        settings['CC']=['icc']
        settings['OPTFLAGS']+=['-O3','-DNDEBUG']
        settings['CCFLAGS']+=['-w1','-wd1476,1572']
      elif self.testBoolOption('use_mingw'):
        settings['CC']=['i586-mingw32msvc-g++']
        settings['OPTFLAGS']+=['-O3','-DNDEBUG','-ffast-math',
                               '-s','-fomit-frame-pointer']
        settings['CCFLAGS']+=['-W','-Wall','-Wno-long-long',
                              '-pedantic','-pipe']
      else:
        settings['CC']=['g++']
        settings['OPTFLAGS']+=['-O3','-DNDEBUG','-ffast-math',
                               '-s','-fomit-frame-pointer']
        settings['CCFLAGS']+=['-W','-Wall','-Wno-long-long',
                              '-pedantic','-pipe']
        self._gccOptimizerWorkAround(settings)
        self._gccMathInlineWorkAround(settings)
      settings['LD']=settings['CC']
      if not self.testBoolOption('use_mingw'):
        settings['INCDIRS']+=['/usr/X11R6/include','/usr/local/include']
      else:
        settings['INCDIRS']+=[os.environ['HOME']+'/windows_lib/local/include'] # modif baudouin

      settings['DEFINES']['_REENTRANT']=None
      settings['DBGFLAGS']+=['-g']
      if (self._system!='IRIX') and not self.testBoolOption('use_icc') and not self.testBoolOption('use_mingw'):
        settings['CCFLAGS']+=['-fPIC']
      if self.testBoolOption('use_mingw'):
        settings['INCDIRS']+=[os.environ['HOME']+ '/windows_lib/local/include'] # modif baudouin

      else:
        if self._system=='Linux_x86_64':
          settings['LIBDIRS']+=['/usr/X11R6/lib64']
        settings['LIBDIRS']+=['/usr/X11R6/lib','/usr/local/lib']
        if (self._system!='FreeBSD') and (self._system!='IRIX'):
          settings['LIBS']+=['dl']
        if self._system!='FreeBSD':
          settings['LIBS']+=['pthread']

      if not self.testBoolOption('use_mingw'):
        settings['LIBS']+=['m','GLU','GL','X11']
      if self._system!='Darwin':
        settings['USE_RPATH']=True
    return settings

  def copySettings(self,settings=None):
    """Returns a deep copy of settings"""
    if settings is None:
      settings=self.settings
    return copy.deepcopy(settings)

  def importFlags(self,flags,settings=None):
    """Parses cc-like flags and fills settings accordingly, returns success"""
    #----
    def unquote(str):
      if (str.startswith('"') and str.endswith('"')) or \
         (str.startswith("'") and str.endswith("'")):
        return str[1:-1]
      else:
        return str.replace('\\ ',' ')
    #----
    def moreArg(flags,i,arg):
      args=[arg]
      nb=len(flags)
      while i<nb:
        f=flags[i]
        if not f.startswith('-'):
          args.append(f)
          i+=1
        else:
          break
      return (i,unquote(' '.join(args).strip()))
    #----
    if flags.startswith('!'):
      try:
        p=subprocess.Popen(flags[1:],shell=True,stdout=subprocess.PIPE)
        flags=p.communicate()[0]
        if p.returncode:
          return False
      except:
        # traceback.print_exc()
        return False
    if settings is None:
      settings=self.settings
    flags=flags.replace('\n',' ').split(' ')
    nb=len(flags)
    i=0
    while i<nb:
      f=flags[i]
      i+=1
      if f.startswith('-I'):
        (i,arg)=moreArg(flags,i,f[2:])
        settings['INCDIRS'].append(arg)
      elif f.startswith('-L'):
        (i,arg)=moreArg(flags,i,f[2:])
        settings['LIBDIRS'].append(arg)
      elif f.startswith('-l'):
        (i,arg)=moreArg(flags,i,f[2:])
        settings['LIBS'].append(arg)
      elif f.startswith('-D'):
        (i,arg)=moreArg(flags,i,f[2:])
        macro=arg.split('=',1)
        if macro and macro[0]:
          key=macro[0]
          value=None
          if len(macro)>1 and macro[1]:
            value=macro[1]
          else:
            value=None
          settings['DEFINES'][key]=value
      elif f.startswith('-Wl,'):
        (i,arg)=moreArg(flags,i,f)
        settings['LDFLAGS'].append(arg)
      else:
        args=[f]
        (i,arg)=moreArg(flags,i,f)
        settings['CCFLAGS'].append(arg)
    return True

  dependancyCache={}

  def collectCppDeps(self,filename,settings=None):
    """Finds filename dependencies according to #include, returns success"""
    #----
    def collectInCache(tool,filename,deps):
      for d in tool.dependancyCache[filename]:
        if d not in deps:
          deps.append(d)
          collectInCache(tool,d,deps)

    def collect(tool,filename,deps,settings):
      if filename not in deps:
        try:
          f=file(filename,'r')
        except:
          return False
        deps.append(filename)
        if not filename in tool.dependancyCache:
          tool.dependancyCache[filename]=[]
        for l in f:
          m=tool._stdIncRE.match(l)
          if m: # incdirs --> (stddirs) --> localdir
            inc=m.string[m.start(1):m.end(1)]
            done=False
            for d in settings['INCDIRS']:
              header=os.path.normpath(os.path.join(d,inc))
              if os.path.isfile(header):
                if not header in tool.dependancyCache[filename]:
                  tool.dependancyCache[filename].append(header)
                  if not collect(tool,header,deps,settings):
                    print 'Enable to read the file '+header
                else:
                  collectInCache(tool,header,deps)
                done=True
                break
            if not done:
              header=os.path.normpath(os.path.join(os.path.dirname(filename),
                                                   inc))
              if os.path.isfile(header):
                if not header in tool.dependancyCache[filename]:
                  tool.dependancyCache[filename].append(header)
                  if not collect(tool,header,deps,settings):
                    print 'Enable to read the file '+header
                else:
                  collectInCache(tool,header,deps)
          else:
            m=tool._locIncRE.match(l)
            if m: # localdir --> incdirs --> (stddirs)
              inc=m.string[m.start(1):m.end(1)]
              header=os.path.normpath(os.path.join(os.path.dirname(filename),
                                                   inc))
              if os.path.isfile(header):
                if not header in tool.dependancyCache[filename]:
                  tool.dependancyCache[filename].append(header)
                  if not collect(tool,header,deps,settings):
                    print 'Enable to read the file '+header
                else:
                  collectInCache(tool,header,deps)
              else:
                for d in settings['INCDIRS']:
                  header=os.path.normpath(os.path.join(d,inc))
                  if os.path.isfile(header):
                    if not header in tool.dependancyCache[filename]:
                      tool.dependancyCache[filename].append(header)
                      if not collect(tool,header,deps,settings):
                        print 'Enable to read the file '+header
                    else:
                      collectInCache(tool,header,deps)
                    break
      return True
    #----
    if settings is None:
      settings=self.settings
    deps=[]
    filename=os.path.normpath(filename)
    collect(self,filename,deps,settings)
    if not deps:
      deps=[filename]
    return deps

  def findExecutable(self,cmd):
    """Returns the full path of an executable searching in the path environment variable"""
    cmd=os.path.normpath(cmd)
    if os.path.isabs(cmd):
      dirs=['']
    else:
      dirs=[i or os.path.curdir
            for i in os.environ['PATH'].split(os.path.pathsep)]
    for i in dirs:
      i=os.path.join(i,cmd)
      if self._system=='Windows':
        execExt=['.com','.exe','.bat','.cmd']
        if os.path.splitext(i)[1].lower() in execExt:
          if os.path.isfile(i) and os.access(i,os.X_OK):
            return i
        else:
          for ext in ['.com','.exe','.bat','.cmd']:
            prog=i+ext
            if os.path.isfile(prog) and os.access(prog,os.X_OK):
              return prog
      else:
        if os.path.isfile(i) and os.access(i,os.X_OK):
          return i
    return None

  def findLibrary(self,lib, settings=None,debug=None):
    """Returns the full path of a library searching libdirs"""
    if debug==None:
      debug=self.testBoolOption("debug")
    if self._system == "Windows":
      filename=lib
      extensions=[".lib"]
      if debug:
        extensions.insert(0, "_d.lib")
    elif self._system == "Darwin":
      filename="lib"+lib
      extensions=[".dylib",".bundle"]
    else:
      filename="lib"+lib
      extensions=[".so"]
    if settings == None:
      settings=self.settings
    for i in settings['LIBDIRS']:
      for ext in extensions:
        j=os.path.join(i,filename+ext)
        if os.path.isfile(j) and os.access(j,os.X_OK):
          return j
    return None

  def addLibWithPKGConfig(self,lib,minVersion=None,maxVersion=None,exactVersion=None):
    """Add library flags using pkg-config"""
    cmd=self.findExecutable('pkg-config')
    if cmd == None:
      print "'pkg-config' not found"
      return False
    p=subprocess.Popen([cmd,'--exists',lib])
    p.wait()
    if p.returncode:
      print "pkg-config can't find "+lib
      return False
    if minVersion:
      p=subprocess.Popen([cmd,'--atleast-version',minVersion,lib])
      p.wait()
      if p.returncode:
        print lib+" version is not at least "+minVersion
        return False
    if maxVersion:
      p=subprocess.Popen([cmd,'--max-version',maxVersion,lib])
      p.wait()
      if p.returncode:
        print lib+" version is later than the max expected: "+maxVersion
        return False
    if exactVersion:
      p=subprocess.Popen([cmd,'--exact-version',exactVersion,lib])
      p.wait()
      if p.returncode:
        print lib+" version is not "+exactVersion
        return False
    flagsCmd=[cmd]+['--cflags',lib]
    if self.sysName() == "Windows":
      flagsCmd+=['--msvc-syntax']
    p=subprocess.Popen(flagsCmd,
                             stdout=subprocess.PIPE)
    gtkFlags=p.communicate()[0].split('\n')[0].strip().split(' ')
    self.settings['CCFLAGS']+=gtkFlags
    
    flagsCmd=[cmd]+['--libs',lib]
    if self.sysName() == "Windows" and (self.getOption('toolset') == 'msvc' or self.getOption('toolset') == None):
      flagsCmd+=['--msvc-syntax']
    p=subprocess.Popen(flagsCmd,
                             stdout=subprocess.PIPE)
    gtkFlags=p.communicate()[0].split('\n')[0].strip().split(' ')
    self.settings['LDFLAGS']+=gtkFlags
    return True

  def compilerCmd(self,target,source,settings=None):
    """Returns compile command according to the settings"""
    if settings is None:
      settings=self.settings
    cmd=[]
    cmd+=settings['CC']
    if self._system=='Windows' and (self.getOption('toolset') == 'msvc' or self.getOption('toolset') == None):
      cmd+=['/Fo%s' % (target),'/c']
    else: # posix system
      cmd+=['-o',target,'-c']
    cmd+=[source]
    cmd+=self.showSettings(['--cflags'],settings)[0]
    return cmd

  def linkerCmd(self,kind,target,objects,settings=None):
    """Returns link command according to the settings"""
    if settings is None:
      settings=self.settings
    if not isinstance(objects,list):
      objects=[objects]
    cmd=[]
    cmd+=settings['LD']
    if self._system=='Windows' and (self.getOption('toolset') == 'msvc' or self.getOption('toolset') == None):
      cmd+=['/out:%s' % (target)]
      if (kind=='plug') or (kind=='lib'):
        lib=os.path.splitext(target)[0]+'.lib'
        cmd+=['/dll','/implib:%s' % (lib)]
      if self.testBoolOption('debug'):
        cmd+=['/PDB:%s' % (os.path.splitext(target)[0]+'.pdb')]
        cmd+=['/DEBUG']
    else: # posix system
      cmd+=['-o',target]
      if self._system=='Darwin':
        cmd+=['-flat_namespace']
        if kind=='prog':
          cmd+=['-bind_at_load']
        elif kind=='plug':
          cmd+=['-bundle']
        elif kind=='lib':
          lib=os.path.basename(target)
          cmd+=['-dynamiclib','-install_name',lib]
      else:
        if (kind=='plug') or (kind=='lib'):
          cmd+=['-shared']
    cmd+=objects
    cmd+=self.showSettings(['--libs'],settings)[0]
    if (kind=='prog') and settings['USE_RPATH']:
      for i in settings['LIBDIRS']:
        cmd.append('-Wl,-rpath,%s' % (i))
    return cmd

  def versionInfo(self,settings=None):
    """Gives version informations"""
    if settings is None:
      settings=self.settings
    return 'Mascaret version %s (under %s)\nin `%s\'\n' \
           % (settings['MASCARET_VERSION'],
              self.sysName(),
              settings['MASCARET_TOP_DIR'])

  def showSettings(self,args,settings=None):
    """Expands settings according to args, returns (out_list,error_list)"""
    if settings is None:
      settings=self.settings
    if args is None:
      args=self._cmdLine[1:]
    if not isinstance(args,list):
      args=[args]
    out=[]
    err=[]
    cc=False
    ld=False
    cflags=''
    libs=''
    help=False
    for i in args:
      if i=='--cc':
        cc=True
      elif i=='--ld':
        ld=True
      elif (i=='--cflags') or (i=='--cflags-only-I') or \
           (i=='--cflags-only-D') or (i=='--cflags-only-other'):
        cflags=i
      elif (i=='--libs') or (i=='--libs-only-L') or \
           (i=='--libs-only-l') or (i=='--libs-only-other'):
        libs=i
      elif i=='--help':
        help=True
      else:
        err.append('%s: unknown option' % (i))
    if help:
      cc=' '.join(settings['CC'])
      ld=' '.join(settings['LD'])
      help=self.versionInfo(settings)
      help+='  --cc                   C++ compiler (%s)\n' % cc
      help+='  --ld                   Linker (%s)\n' % ld
      help+='  --cflags               All compiler flags\n'
      help+='  --cflags-only-I        Only -I compiler flags\n'
      help+='  --cflags-only-D        Only -D compiler flags\n'
      help+='  --cflags-only-other    Neither -I nor -D compiler flags\n'
      help+='  --libs                 All linker flags\n'
      help+='  --libs-only-L          Only -L linker flags\n'
      help+='  --libs-only-l          Only -l linker flags\n'
      help+='  --libs-only-other      Neither -L nor -l linker flags\n'
      help+='  --help                 This help message'
      out.append(help)
    else:
      for s in ['INCDIRS','LIBDIRS']:
        unique=[]
        for i in settings[s]:
          i=os.path.normpath(i)
          if i not in unique and os.path.isdir(i):
            unique.append(i)
        settings[s]=unique
      unique=[]
      for i in settings['LIBS']:
        if i not in unique:
          unique.append(i)
      settings['LIBS']=unique
      if cc:
        out+=settings['CC']
      if ld:
        out+=settings['LD']
      if (cflags=='--cflags') or (cflags=='--cflags-only-other'):
        if self.testBoolOption('debug'):
          for i in settings['DBGFLAGS']:
            out.append(i)
        else:
          for i in settings['OPTFLAGS']:
            out.append(i)
        for i in settings['CCFLAGS']:
          out.append(i)
      if (cflags=='--cflags') or (cflags=='--cflags-only-D'):
        if self._system=='Windows' and (self.getOption('toolset') == 'msvc' or self.getOption('toolset') == None):
          opt='/D'
        else:
          opt='-D'
        for i in settings['DEFINES']:
          v=settings['DEFINES'][i]
          if v is None:
            out.append('%s%s' % (opt,i))
          else:
            out.append('%s%s=%s' % (opt,i,v))
      if (cflags=='--cflags') or (cflags=='--cflags-only-I'):
        if self._system=='Windows' and (self.getOption('toolset') == 'msvc' or self.getOption('toolset') == None):
          opt='/I'
        else:
          opt='-I'
        for i in settings['INCDIRS']:
          out.append('%s%s' % (opt,os.path.normpath(i)))
      if (libs=='--libs') or (libs=='--libs-only-other'):
        for i in settings['LDFLAGS']:
          out.append(i)
      if (libs=='--libs') or (libs=='--libs-only-L'):
        if self._system=='Windows' and (self.getOption('toolset') == 'msvc' or self.getOption('toolset') == None):
          opt='/LIBPATH:'
        else:
          opt='-L'
        for i in settings['LIBDIRS']:
          out.append('%s%s' % (opt,os.path.normpath(i)))
      if (libs=='--libs') or (libs=='--libs-only-l'):
        if self._system=='Windows' and (self.getOption('toolset') == 'msvc' or self.getOption('toolset') == None):
          preOpt=''
          postOpt='.lib'
        else:
          preOpt='-l'
          postOpt=''
        for i in settings['LIBS']:
          out.append('%s%s%s' % (preOpt,i,postOpt))
    return (out,err)

  def Program(self,target,sources,cb=None,cbArg=None,settings=None):
    """Declares a executable target"""
    if settings is None:
      settings=self.settings
    settings=copy.deepcopy(settings)
    if not isinstance(sources,list):
      sources=[sources]
    target=os.path.normpath(target)
    prog=os.path.basename(target)
    if self._system=='Windows' or self.testBoolOption('use_mingw'):
      prog+='.exe'
    prog=os.path.join(os.path.dirname(target),prog)
    if self._findTarget(prog):
      raise type(self).Error,'Multiple occurences for the target `%s\'' \
                             % (prog)
    sources=[os.path.normpath(i) for i in sources]
    self._programs.append(_BinTarget(self,'prog',prog,sources,
                                     cb,cbArg,settings))
    return prog

  def Plugin(self,target,sources,cb=None,cbArg=None,settings=None):
    """Declares a plugin target"""
    if settings is None:
      settings=self.settings
    settings=copy.deepcopy(settings)
    if not isinstance(sources,list):
      sources=[sources]
    target=os.path.normpath(target)
    plug=os.path.basename(target)
    if self._system=='Windows' or self.testBoolOption('use_mingw'):
      plug+='.dll'
    elif self._system=='Darwin':
      plug='lib%s.bundle' % (plug)
    else:
      plug='lib%s.so' % (plug)
    plug=os.path.join(os.path.dirname(target),plug)
    if self._findTarget(plug):
      raise type(self).Error,'Multiple occurences for the target `%s\'' \
                             % (plug)
    sources=[os.path.normpath(i) for i in sources]
    self._plugins.append(_BinTarget(self,'plug',plug,sources,
                                    cb,cbArg,settings))
    return plug

  def Library(self,target,sources,cb=None,cbArg=None,settings=None):
    """Declares a dynamic library target"""
    if settings is None:
      settings=self.settings
    settings=copy.deepcopy(settings)
    if not isinstance(sources,list):
      sources=[sources]
    target=os.path.normpath(target)
    lib=os.path.basename(target)
    if self._system=='Windows' or self.testBoolOption('use_mingw'):
      lib+='.dll'
    elif self._system=='Darwin':
      lib='lib%s.dylib' % (lib)
    else:
      lib='lib%s.so' % (lib)
    lib=os.path.join(os.path.dirname(target),lib)
    if self._findTarget(lib):
      raise type(self).Error,'Multiple occurences for the target `%s\'' \
                             % (lib)
    sources=[os.path.normpath(i) for i in sources]
    self._libraries.append(_BinTarget(self,'lib',lib,sources,
                                      cb,cbArg,settings))
    return lib

  def Generator(self,target,sources,cb,cbArg,settings=None):
    if settings is None:
      settings=self.settings
    settings=copy.deepcopy(settings)
    if not isinstance(sources,list):
      sources=[sources]
    target=os.path.normpath(target)
    if self._findTarget(target):
      raise type(self).Error,'Multiple occurences for the target `%s\'' \
                             % (target)
    sources=[os.path.normpath(i) for i in sources]
    self._generators.append(_Target(self,'gen',target,sources,
                                    cb,cbArg,settings))
    return target

  def FlexBisonParser(self,target,cb=None,cbArg=None,settings=None):
    """Declares a flex/bison target"""
    if settings is None:
      settings=self.settings
    settings=copy.deepcopy(settings)
    target=os.path.normpath(target)
    if self._findTarget(target):
      raise type(self).Error,'Multiple occurences for the target `%s\'' \
                             % (target)
    prefix=os.path.splitext(target)[0]
    sources=[prefix+'.y',prefix+'.l']
    self._generators.append(_FlexBisonTarget(self,'gen',target,sources,
                                             cb,cbArg,settings))
    return target

  def Clean(self,files):
    """Cleans files"""
    if not isinstance(files,list):
      files=[files]
    self._cleanList+=files

  def programs(self):
    """Returns executable targets name"""
    return [i.name for i in self._programs]

  def plugins(self):
    """Returns plugins targets name"""
    return [i.name for i in self._plugins]

  def libraries(self):
    """Returns dynamic libraries targets name"""
    return [i.name for i in self._libraries]

  def generators(self):
    return [i.name for i in self._generators]

  def targets(self):
    """Returns all targets name"""
    return self.generators()+self.libraries()+self.plugins()+self.programs()

  def run(self,all=None):
    """Generates makefile with default targets or perform command line"""
    quiet=self.testBoolOption('quiet')
    if len(self._cmdLine)>1:
      status=0
      cmd=self._cmdLine[1]
      if cmd=='--make':
        cmdLine=self.settings['MAKE']+self._cmdLine[2:]
        cmdLine[0]=os.path.normpath(cmdLine[0])
        try:
          proc=subprocess.Popen(cmdLine)
          proc.wait()
          status=proc.returncode
        except:
          status=1
      elif cmd=='--clean':
        if len(self._cmdLine)!=2:
          sys.stderr.write('no argument expected for the `%s\' command\n'
                           % (cmd))
          status=1
        else:
          self._cleaning=True
          if not quiet:
            print 'Cleaning ...'
          for i in self._generators+self._libraries+ \
                   self._plugins+self._programs:
            i.clean()
          self.remove(self._cleanList)
          if self._system=='IRIX':
            self.remove('so_locations')
            for (root,dirs,files) in os.walk(os.path.curdir,topDown=False):
              if 'ii_files' in dirs:
                self.remove(os.path.join(root,'ii_files'))
          self.remove('makefile')
          self._cleaning=False
      elif cmd=='--callback':
        if len(self._cmdLine)!=3:
          sys.stderr.write('one argument is expected for the `%s\' command\n'
                           % (cmd))
          status=1
        else:
          arg=self._cmdLine[2]
          target=self._findTarget(arg)
          if not target:
            sys.stderr.write('no generator for the target `%s\'\n'
                             % (arg))
            status=1
          if not status:
            status=target.callback()
      else:
        sys.stderr.write('unknown command `%s\'\n' % (cmd))
        status=1
      sys.exit(status)
    else:
      #----
      if self._system=='Windows':
        def cmdProtect(s):
          s2=s.replace('"','\\"')
          if (s2==s) and (s.find(' ')<0):
            return s
          else:
            return '"%s"' % (s2)
        def mkProtect(s):
          if s.find(' ')<0:
            return s
          else:
            return '"%s"' % (s)
        def mkEcho(msg=''):
          return '\t@echo.%s\n' % (msg.replace('%','^%'))
      else: # posix system
        def cmdProtect(s):
          esc=['|','&',';','<','>','(',')','$','`','\\','"','\'',' ',
               '\t','\r','\n','*','?','~',]
          prot=False
          for i in s:
            if i in esc:
              prot=True
              break
          if prot:
            return '\'%s\'' % (s.replace('\'','\\\''))
          else:
            return s
        def mkProtect(s):
          return s.replace(' ','\\ ')
        def mkEcho(msg=''):
          return '\t@echo \'%s\'\n' % (msg.replace('\'','\\\''))
      #----
      if not quiet:
        print 'Generating makefile ...'
      mk=file('makefile','w')
      mk.write('\n')
      mk.write('# This file is generated by %s\n' % (__name__))
      mk.write('# Do not edit or your changes will be lost !')
      mk.write('\n')
      targets=self._generators+self._libraries+self._plugins+self._programs
      if all is None:
        all=[i.name for i in targets]
      elif not isinstance(all,list):
        all=[all]
      hint=''
      if self._programs:
        path=[]
        for i in self._programs:
          for j in i.settings['LIBDIRS']:
            j=os.path.normpath(j)
            if j not in path:
              path.append(j)
        if path:
          path=os.path.pathsep.join(path)
          if self._system=='Windows':
            var='PATH'
            setCmd='set %s=%s'
            resetCmd='set %s=%%%s%%%s%s'
          else: # posix system
            if self._system=='Darwin':
              var='DYLD_LIBRARY_PATH'
            else:
              var='LD_LIBRARY_PATH'
            setCmd='export %s="%s"'
            resetCmd='export %s="$%s%s%s"'
          if var in os.environ and os.environ[var]!='':
            hint=resetCmd % (var,var,os.pathsep,path)
          else:
            hint=setCmd % (var,path)
      mk.write('\n__ALL_DEFAULT_TARGETS__ : ')
      for i in all:
        mk.write('\\\n    %s ' % (mkProtect(os.path.normpath(i))))
      if hint:
        mk.write('\\\n    __DYNAMIC_LIB_HINT__')
        mk.write('\n')
        mk.write('\n__DYNAMIC_LIB_HINT__ :\n')
        mk.write(mkEcho())
        mk.write(mkEcho('If any dynamic library is not found at run-time,'))
        mk.write(mkEcho('you may try the following command:'))
        mk.write(mkEcho())
        mk.write(mkEcho('  %s' % (hint)))
        mk.write(mkEcho())
      mk.write('\n')
      doneList=[]
      for i in targets:
        if i in doneList:
          mk.write('\n# `%s\' already generated above\n\n' % (i))
        else:
          if not quiet:
            print '    %s' % (i.name)
          i.generateMake(mk,cmdProtect,mkProtect,mkEcho)
          doneList.append(i)
          for j in i.sub:
            if j in doneList:
              mk.write('\n# `%s\' already generated above\n\n' % (j))
            else:
              if not quiet:
                print '    %s' % (j)
              i.generateMake(mk,cmdProtect,mkProtect,mkEcho,j)
              doneList.append(j)
      mk.write('\nclean :\n')
      mk.write('\t@%s %s --clean\n'
               % (cmdProtect(sys.executable),
                  cmdProtect(self._cmdLine[0])))
      mk.write('\n.c.o :\n')
      mk.write(mkEcho('NO .c.o DEFAULT RULE FOR $<'))
      mk.write('\n.cpp.o :\n')
      mk.write(mkEcho('NO .cpp.o DEFAULT RULE FOR $<'))
      mk.write('\n')
      mk.close()
      if not quiet:
        print 'Python is `%s\' (%s)' % (sys.executable,
                                        platform.python_version())
        m=sys.modules[type(self).__module__]
        print '%s is `%s\'' % (m.__name__,os.path.realpath(m.__file__))
        print 'According to PATH:'
        make=self.settings['MAKE'][0]
        exe=self.findExecutable(make)
        if exe:
          print '    the make program is `%s\'' % (exe)
        else:
          print '    the make program (%s) cannot be found' % (make)
        cc=self.settings['CC'][0]
        exe=self.findExecutable(cc)
        if exe:
          print '    the compiler is `%s\'' % (exe)
        else:
          print '    the compiler (%s) cannot be found' % (cc)
        ld=self.settings['LD'][0]
        if ld!=cc:
          exe=self.findExecutable(ld)
          if exe:
            print '    the linker is `%s\'' % (exe)
          else:
            print '    the linker (%s) cannot be found' % (ld)
        for i in self._generators:
          if isinstance(i,_FlexBisonTarget):
            flex=self.settings['FLEX'][0]
            exe=self.findExecutable(flex)
            if exe:
              print '    the flex program is `%s\'' % (exe)
            else:
              print '    the flex program (%s) cannot be found' % (flex)
            bison=self.settings['BISON'][0]
            exe=self.findExecutable(bison)
            if exe:
              print '    the bison program is `%s\'' % (exe)
            else:
              print '    the bison program (%s) cannot be found' % (bison)
            break

  def remove(self,files):
    display=self._cleaning and not self.testBoolOption('quiet')
    if not isinstance(files,list):
      files=[files]
    for i in files:
      for j in glob.glob(i):
        try:
          if os.path.isdir(j):
            for k in os.listdir(j):
              self.remove(os.path.join(j,k))
            os.rmdir(j)
          else:
            os.unlink(j)
          if display:
            print '    %s' % (j)
        except:
          pass

  def _gccOptimizerWorkAround(self,settings):
    workAround=False
    try:
      p=subprocess.Popen(settings['CC']+['--version'],
                         stdout=subprocess.PIPE)
      line=p.communicate()[0].split('\n')[0]
    except:
      line=''
    m=re.match(r'^.*\(GCC\)[ ]+([0-9]+)\.([0-9]+)\.([0-9]+).*$',line)
    if m:
      maj=int(m.string[m.start(1):m.end(1)])
      min=int(m.string[m.start(2):m.end(2)])
      ver=int(m.string[m.start(3):m.end(3)])
      if (maj>4) or ((maj==4) and (min>=1)): # optimizer bug in gcc >= 4.1 ?
        workAround=True
    if workAround:
      flags=[]
      for i in settings['OPTFLAGS']:
        if i=='-03':
          flags+=['-O2','-fno-strict-aliasing']
        else:
          flags.append(i)
      settings['OPTFLAGS']=flags

  def _gccMathInlineWorkAround(self,settings):
    workAround=False
    try:
      for l in file('/usr/include/bits/mathinline.h','r'):
        if l.find('?:')>=0:
          workAround=True
          break
    except:
      pass
    if workAround:
      settings['CCFLAGS']=[i for i in settings['CCFLAGS'] if i!='-pedantic']

  def _findTarget(self,target,candidates=None):
    if candidates is None:
      candidates=self._generators+self._libraries+self._plugins+self._programs
    for i in candidates:
      if i.name==target:
        return i
    return None

  def _main(self):
    (out,err)=self.showSettings(None)
    if err:
      for i in err:
        sys.stderr.write('%s\n' % (i))
      sys.exit(1)
    if out:
      sys.stdout.write('%s\n' % (' '.join(out)))

  _instance=None

#-----------------------------------------------------------------------------

class _Target(object):

  def __init__(self,tool,kind,name,sources,cb,cbArg,settings):
    object.__init__(self)
    self.tool=tool
    self.kind=kind
    self.name=name
    self.sources=[i for i in sources if sources.count(i)==1]
    self.sub=[]
    self.cb=cb
    self.cbArg=cbArg
    self.settings=settings

  def clean(self):
    if self.cb:
      self.cb(True,self.name,self.sources,self.cbArg,self.settings)
    self.tool.remove(self.sub+[self.name])
    #for i in self.sources:
    #  try:
    #    os.utime(i,None)
    #  except:
    #    pass

  def callback(self):
    if self.cb:
      return self.cb(False,self.name,self.sources,self.cbArg,self.settings)
    else:
      return 0 # success

  def writeMake(self,mk,cmdProtect,mkProtect,mkEcho,target,deps,cmd):
    mk.write('\n%s : ' % (mkProtect(target)))
    for i in deps:
      mk.write('\\\n    %s ' % (mkProtect(i)))
    mk.write('\n')
    mk.write(mkEcho())
    if self.tool._system=='Windows':
      mk.write(mkEcho('==== %s ====' % (target)))
    else:
      mk.write(mkEcho('\033[32;1m==== %s ====\033[00m' % (target)))
    if cmd:
      mk.write('\t%s\n' % (' '.join([cmdProtect(i) for i in cmd])))
    if (target==self.name) and self.cb:
      mk.write('\t@%s %s --callback %s\n'
               % (cmdProtect(sys.executable),
                  cmdProtect(self.tool._cmdLine[0]),
                  cmdProtect(target)))
    mk.write('\n')

  def generateMake(self,mk,cmdProtect,mkProtect,mkEcho,subTarget=None):
    if subTarget:
      target=subTarget
    else:
      target=self.name
    if subTarget:
      return # nothing to be done at this level of abstraction
    self.writeMake(mk,cmdProtect,mkProtect,mkEcho,target,self.sources,[])

#-----------------------------------------------------------------------------

class _BinTarget(_Target):

  def __init__(self,tool,kind,name,sources,cb,cbArg,settings):
    _Target.__init__(self,tool,kind,name,sources,cb,cbArg,settings)
    if self.tool._system=='Windows':
      ext='.obj'
    else:
      ext='.o'
    self.sub=[os.path.splitext(s)[0]+ext for s in self.sources]
    self.dependLibs=[]
    for i in settings["LIBS"]:
      lib=self.tool.findLibrary(i,settings)
      if lib != None:
        self.dependLibs+=[lib]

  def clean(self):
    _Target.clean(self)
    if self.tool._system=='Windows' and \
       ((self.kind=='plug') or (self.kind=='lib')):
      prefix=os.path.splitext(self.name)[0]
      self.tool.remove([prefix+'.lib',prefix+'.exp'])

  def generateMake(self,mk,cmdProtect,mkProtect,mkEcho,subTarget=None):
    if subTarget:
      target=subTarget
    else:
      target=self.name
    deps=None
    if subTarget:
      if len(self.sources) > self.sub.index(target):
        src=self.sources[self.sub.index(target)]
        deps=self.tool.collectCppDeps(src,self.settings)
        cmd=self.tool.compilerCmd(target,src,self.settings)
    else:
      deps=self.sub+self.dependLibs
      cmd=self.tool.linkerCmd(self.kind,target,self.sub,self.settings)
    if deps != None:
      self.writeMake(mk,cmdProtect,mkProtect,mkEcho,target,deps,cmd)

#-----------------------------------------------------------------------------

class _FlexBisonTarget(_Target):

  def __init__(self,tool,kind,name,sources,cb,cbArg,settings):
    #----
    def bounceCB(cleaning,target,sources,arg,settings):
      (cb,cbArg)=arg
      if cb:
        return cb(cleaning,target,sources,cbArg,settings)
      else:
        return 0 # success
    #----
    _Target.__init__(self,tool,kind,name,sources,bounceCB,(cb,cbArg),settings)

  def clean(self):
    _Target.clean(self)
    prefix=os.path.splitext(self.name)[0]
    self.tool.remove([prefix+'.output',prefix+'.cpp.output'])

  def callback(self):
    try:
      cmd=self.settings['BISON']+['-v','-o%s' % (self.name),self.sources[0]]
      print ' '.join(cmd)
      proc=subprocess.Popen(cmd)
      proc.wait()
      if proc.returncode:
        return proc.returncode # failure
      pattern1=re.compile(r'^int yyparse .*$')
      pattern2=re.compile(r'^class istream;.*$')
      pattern3=re.compile(r'^#include <FlexLexer\.h>.*$')
      pattern4=re.compile(r'^extern "C" int isatty .*$')
      cppLines=[l for l in file(self.name,'r') if not re.match(pattern1,l)]
      cmd=self.settings['FLEX']+['-Cfa','-t',self.sources[1]]
      print ' '.join(cmd)
      proc=subprocess.Popen(cmd,stdout=subprocess.PIPE)
      cppLines+=[l for l in proc.stdout if not re.match(pattern2,l) and
                                           not re.match(pattern3,l) and
                                           not re.match(pattern4,l)]
      proc.wait()
      if proc.returncode:
        return proc.returncode # failure
      file(self.name,'w').writelines(cppLines)
    except:
      traceback.print_exc()
      return 1 # failure
    if self.cb:
      return self.cb(False,self.name,self.sources,self.cbArg,self.settings)
    return 0 # success

  def generateMake(self,mk,cmdProtect,mkProtect,mkEcho,subTarget=None):
    if subTarget:
      target=subTarget
    else:
      target=self.name
    if subTarget:
      return # nothing to be done
    deps=[]
    for i in self.sources:
      deps+=self.tool.collectCppDeps(i,self.settings)
    self.writeMake(mk,cmdProtect,mkProtect,mkEcho,target,deps,[])

#-----------------------------------------------------------------------------

if __name__=='__main__':
  Tool()._main()

#-----------------------------------------------------------------------------
